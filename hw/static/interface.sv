//
// Copyright (c) 2017, Intel Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// Neither the name of the Intel Corporation nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

`include "cci_mpf_if.vh"
`include "csr_mgr.vh"


module app_afu(
    input  logic clk,
    // Connection toward the host.  Reset comes in here.
    cci_mpf_if.to_fiu fiu,
    // CSR connections
    app_csrs.app csrs,
    // MPF tracks outstanding requests.  These will be true as long as
    // reads or unacknowledged writes are still in flight.
    input  logic c0NotEmpty,
    input  logic c1NotEmpty
);
    // Local reset to reduce fan-out
    logic reset = 1'b1;
    always @(posedge clk)
    begin
        reset <= fiu.reset;
    end
    //
    // Convert between byte addresses and line addresses.  The conversion
    // is simple: adding or removing low zero bits.
    //
    localparam CL_BYTE_IDX_BITS = 6;
    typedef logic [$bits(t_cci_clAddr) + CL_BYTE_IDX_BITS - 1 : 0] t_byteAddr;

    function automatic t_cci_clAddr byteAddrToClAddr(t_byteAddr addr);
        return addr[CL_BYTE_IDX_BITS +: $bits(t_cci_clAddr)];
    endfunction

    function automatic t_byteAddr clAddrToByteAddr(t_cci_clAddr addr);
        return {addr, CL_BYTE_IDX_BITS'(0)};
    endfunction
    
    // ====================================================================
    //
    //  CSRs (simple connections to the external CSR management engine)
    //
    // ====================================================================
 
    logic [63:0]total_clocks;
    always_comb
    begin
        // The AFU ID is a unique ID for a given program.  Here we generated
        // one with the "uuidgen" program.
        csrs.afu_id = 128'h9f81ba12_1d38_4cc7_953a_dafeef45065b;
        // Default
        for (int i = 0; i < NUM_APP_CSRS; i = i + 1)
        begin
            csrs.cpu_rd_csrs[i].data = 64'(0);
        end
        // Exported counters.  The simple csrs interface used here has
        // no read request.  It expects the current CSR value to be
        // available every cycle.
        csrs.cpu_rd_csrs[0].data = 64'(total_clocks);
    end    
    
    //
    // Consume configuration CSR writes
    //
    
    typedef enum logic[2:0]{
      CFG_REG,
      ADDR_WORKSPACE_BASE
    }CSR_WR;
    
    t_byteAddr workspace_addr_base;
    logic start_acc;
    logic reset_parc;
    always_ff @(posedge clk)
    begin
        if(reset)
        begin
            start_acc <= 1'b0;
            reset_parc <= 1'b0;
            workspace_addr_base <= t_byteAddr'(0);
        end 
        else begin
            if (csrs.cpu_wr_csrs[CFG_REG].en)
            begin
                start_acc <= csrs.cpu_wr_csrs[CFG_REG].data[0];
                reset_parc <= csrs.cpu_wr_csrs[CFG_REG].data[1];
            end
            
            if (csrs.cpu_wr_csrs[ADDR_WORKSPACE_BASE].en)
            begin
                workspace_addr_base <= csrs.cpu_wr_csrs[ADDR_WORKSPACE_BASE].data;
            end
        end 
    end
        
    wire req_rd_en;
    t_byteAddr req_rd_addr;
    t_cci_mdata req_rd_mdata;
    
    wire req_wr_en;
    t_byteAddr  req_wr_addr;
    t_cci_clData req_wr_data;
    t_cci_mdata req_wr_mdata;
    
    t_cci_mpf_c0_ReqMemHdr rd_hdr;
    t_cci_mpf_c1_ReqMemHdr wr_hdr;
    t_cci_mpf_ReqMemHdrParams rd_hdr_params,wr_hdr_params;
    
    always_comb
    begin
        // Use virtual addresses
        rd_hdr_params = cci_mpf_defaultReqHdrParams(1);
        // Let the FIU pick the channel
        rd_hdr_params.vc_sel = eVC_VA;
        // Read 1 line
        rd_hdr_params.cl_len = eCL_LEN_1;
        // Generate the header
        rd_hdr = cci_mpf_c0_genReqHdr(eREQ_RDLINE_I, req_rd_addr,req_rd_mdata, rd_hdr_params);
        
        fiu.c0Tx = cci_mpf_genC0TxReadReq(rd_hdr,req_rd_en);
    end
    always_comb
    begin
        // Use virtual addresses
        wr_hdr_params = cci_mpf_defaultReqHdrParams();
        // Let the FIU pick the channel
        wr_hdr_params.vc_sel = eVC_VA;
        // Writer 1 line
        wr_hdr_params.cl_len = eCL_LEN_1; 
        
        wr_hdr = cci_mpf_c1_genReqHdr(eREQ_WRLINE_I, req_wr_addr,req_wr_mdata, wr_hdr_params);
        
        fiu.c1Tx = cci_mpf_genC1TxWriteReq(wr_hdr,req_wr_data,req_wr_en);
    end 
    
    buffer_controller buffer_controller(
    
      .clk(clk),
      .rst(reset),
      .rst_parc(reset_parc),
      .start(start_acc),
      .workspace_addr_base({(64-$bits(t_byteAddr))'(0),workspace_addr_base}),
      
      .req_rd_en(req_rd_en), 
      .req_rd_available(~fiu.c0TxAlmFull),
      .req_rd_addr(req_rd_addr),
      .req_rd_mdata(req_rd_mdata),
           
      .resp_rd_valid(cci_c0Rx_isReadRsp(fiu.c0Rx)),
      .resp_rd_data(fiu.c0Rx.data),
      .resp_rd_mdata(fiu.c0Rx.hdr.mdata),
      
      .req_wr_available(~fiu.c1TxAlmFull),
      .req_wr_en(req_wr_en),
      .req_wr_addr(req_wr_addr),
      .req_wr_mdata(req_wr_mdata),
      .req_wr_data(req_wr_data),
      
      .resp_wr_valid(cci_c1Rx_isWriteRsp(fiu.c1Rx)),
      .resp_wr_mdata(fiu.c1Rx.hdr.mdata)     
    );
    
    //
    // Count cycles for statistics
    //
    
    always_ff @(posedge clk)
    begin
       if(reset)begin
          total_clocks <= 64'd0;
       end 
       else if(start_acc)begin
          total_clocks <= total_clocks + 64'd1;
       end  
       
       if(fiu.c1Tx.valid) begin 
          $display("REQ WR:%d - %x DATA %x",fiu.c1Tx.hdr.base.mdata,clAddrToByteAddr(fiu.c1Tx.hdr.base.address), fiu.c1Tx.data); 
       end 
       
       if(cci_c1Rx_isWriteRsp(fiu.c1Rx)) begin 
          $display("RESP WR:%d",fiu.c1Rx.hdr.mdata); 
       end
       
       if(fiu.c0Tx.valid) begin
          $display("pediu: %x - %x",clAddrToByteAddr(fiu.c0Tx.hdr.base.address),req_rd_addr);
       end 
       
       if(cci_c0Rx_isReadRsp(fiu.c0Rx))
       begin
         $display("chegou:%d %x",fiu.c0Rx.hdr.mdata,fiu.c0Rx.data);
       end
       
    end 
    
    //
    // This AFU never handles MMIO reads.  MMIO is managed in the CSR module.
    //    
    assign fiu.c2Tx.mmioRdValid = 1'b0;
  
endmodule // app_afu
