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

`define PRINT_CL

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
    logic [2:0]reset_pipe;
    always @(posedge clk)
    begin
        reset_pipe[0] <= fiu.reset;
        reset_pipe[1] <= reset_pipe[0];
        reset_pipe[2] <= reset_pipe[1];
        reset <= reset_pipe[2];
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
    typedef enum logic[3:0] {
      REG_CLOCK_COUNT,
      REG_CL_WR_COUNT,
      REG_CL_RD_COUNT,
      REG_INF_1,
      REG_INF_2,
      REG_INF_3,
      REG_INF_4,
      REG_INF_5,
      REG_INF_6,
      REG_INF_7,
      REG_INF_8
    }CSR_RD;
    logic [511:0] info;
    logic [63:0]total_clocks;
    logic [63:0]total_cl_rd;
    logic [63:0]total_cl_wr;
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
        csrs.cpu_rd_csrs[REG_CLOCK_COUNT].data = 64'(total_clocks);
        csrs.cpu_rd_csrs[REG_CL_WR_COUNT].data = 64'(total_cl_wr);
        csrs.cpu_rd_csrs[REG_CL_RD_COUNT].data = 64'(total_cl_rd);        
        csrs.cpu_rd_csrs[REG_INF_1].data = 64'(info[63:0]);
        csrs.cpu_rd_csrs[REG_INF_2].data = 64'(info[127:64]);
        csrs.cpu_rd_csrs[REG_INF_3].data = 64'(info[191:128]);
        csrs.cpu_rd_csrs[REG_INF_4].data = 64'(info[255:192]);
        csrs.cpu_rd_csrs[REG_INF_5].data = 64'(info[319:256]);
        csrs.cpu_rd_csrs[REG_INF_6].data = 64'(info[383:320]);
        csrs.cpu_rd_csrs[REG_INF_7].data = 64'(info[447:384]);
        csrs.cpu_rd_csrs[REG_INF_8].data = 64'(info[511:448]);  
        
    end    
    
    //
    // Consume configuration CSR writes
    //
    typedef enum logic[3:0]{
      REG_CONF_DSM_LOW,
      REG_CONF_DSM_HIGH,
      REG_CONF_IN_LOW,
      REG_CONF_IN_HIGH,
      REG_CONF_OUT_LOW,
      REG_CONF_OUT_HIGH,
      REG_START_AFUs,
      REG_STOP_AFUs,
      REG_RESET_AFUs
    }CSR_WR;
    
    localparam CONF_TYPE_IN_DATA = 1;
    localparam CONF_TYPE_OUT_DATA = 2;
    localparam CONF_TYPE_OUT_DSM = 3;
    
    logic [63:0] start_afus;
    logic [63:0] rst_afus;
    logic [127:0] conf;
    logic [1:0]conf_valid;

    always_ff @(posedge clk)
    begin
        if(reset)
        begin
            start_afus <= 64'd0;
            rst_afus <= 64'd0;
            conf_valid <= 2'd0;
            conf <= 128'd0;
        end 
        else begin
            conf_valid <= 2'd0;
            if(csrs.cpu_wr_csrs[REG_CONF_IN_LOW].en)
            begin
              conf[63:0] <= csrs.cpu_wr_csrs[REG_CONF_IN_LOW].data;
            end 
            if(csrs.cpu_wr_csrs[REG_CONF_IN_HIGH].en)
            begin
              conf[127:64] <= csrs.cpu_wr_csrs[REG_CONF_IN_HIGH].data;
              conf_valid <= CONF_TYPE_IN_DATA;
            end   
            if(csrs.cpu_wr_csrs[REG_CONF_OUT_LOW].en)
            begin
              conf[63:0] <= csrs.cpu_wr_csrs[REG_CONF_OUT_LOW].data;
            end 
            if(csrs.cpu_wr_csrs[REG_CONF_OUT_HIGH].en)
            begin
              conf[127:64] <= csrs.cpu_wr_csrs[REG_CONF_OUT_HIGH].data;
              conf_valid <= CONF_TYPE_OUT_DATA;
            end 
            if(csrs.cpu_wr_csrs[REG_CONF_DSM_LOW].en)
            begin
              conf[63:0] <= csrs.cpu_wr_csrs[REG_CONF_DSM_LOW].data;
            end 
            if(csrs.cpu_wr_csrs[REG_CONF_DSM_HIGH].en)
            begin
              conf[127:64] <= csrs.cpu_wr_csrs[REG_CONF_DSM_HIGH].data;
              conf_valid <= CONF_TYPE_OUT_DSM;
            end 
            if (csrs.cpu_wr_csrs[REG_START_AFUs].en)
            begin       
                start_afus <= start_afus | csrs.cpu_wr_csrs[REG_START_AFUs].data;
            end 
            if(csrs.cpu_wr_csrs[REG_STOP_AFUs].en)
            begin
               start_afus <= start_afus & ~csrs.cpu_wr_csrs[REG_STOP_AFUs].data;
            end 
            if (csrs.cpu_wr_csrs[REG_RESET_AFUs].en)
            begin
                rst_afus <= csrs.cpu_wr_csrs[REG_RESET_AFUs].data;
            end
        end 
    end
        
    logic req_rd_en;
    t_byteAddr req_rd_addr;
    t_cci_mdata req_rd_mdata;
    
    logic req_wr_en;
    t_byteAddr  req_wr_addr;
    t_cci_clData req_wr_data;
    t_cci_mdata req_wr_mdata;
    
    t_cci_mpf_c0_ReqMemHdr rd_hdr;
    t_cci_mpf_c1_ReqMemHdr wr_hdr;
    t_cci_mpf_ReqMemHdrParams rd_hdr_params,wr_hdr_params;
    
    logic [2-1:0]sop_count;
    
    always_ff @(posedge clk)
    begin
        if(reset)
        begin
          sop_count <= 2'd0;
        end else begin
           if(req_wr_en)begin
              sop_count <= sop_count + 2'd1;
           end
        end 
    end 
    
    always_comb
    begin
        // Use virtual addresses
        rd_hdr_params = cci_mpf_defaultReqHdrParams(1);
        // Let the FIU pick the channel
        rd_hdr_params.vc_sel = eVC_VA;
        // Read 1 line
        rd_hdr_params.cl_len = eCL_LEN_4;
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
        wr_hdr_params.cl_len = eCL_LEN_4;
        
        wr_hdr_params.sop = (sop_count == 2'd0);
        
        wr_hdr = cci_mpf_c1_genReqHdr(eREQ_WRLINE_I, req_wr_addr,req_wr_mdata, wr_hdr_params);
        
        fiu.c1Tx = cci_mpf_genC1TxWriteReq(wr_hdr,req_wr_data,req_wr_en);
    end 
    
    
    acc_management acc_management(
    
      .clk(clk),
      .rst(reset),
     
      .rst_accs(rst_afus),  
      .start_accs(start_afus),
      
      .conf_valid(conf_valid),
      .conf(conf),
        
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
      .resp_wr_mdata(fiu.c1Rx.hdr.mdata),
      
      .info(info)
    );
    
    
    always_ff @(posedge clk)
    begin
       if(reset)begin
          total_clocks <= 64'd0;
          total_cl_wr <= 64'd0;
          total_cl_rd <= 64'd0;
       end 
       else begin
          if(|start_afus)begin 
             total_clocks <= total_clocks + 64'd1;
          end
          if(cci_c1Rx_isWriteRsp(fiu.c1Rx)) begin 
             total_cl_wr <= total_cl_wr + 64'd4;
          end
          if(cci_c0Rx_isReadRsp(fiu.c0Rx))
          begin
             total_cl_rd <= total_cl_rd + 64'd1;
          end
       end 
    end 
`ifdef PRINT_CL
  //synthesis translate_off
  always_ff @(posedge clk)  begin          
          if(fiu.c1Tx.valid) begin 
             $display("%d:REQ WR:%d %x DATA %x",total_clocks,fiu.c1Tx.hdr.base.mdata,clAddrToByteAddr(fiu.c1Tx.hdr.base.address), fiu.c1Tx.data); 
          end       
          if(fiu.c0Tx.valid) begin
             $display("%d:REQ READ:%d  %x - %x",total_clocks,fiu.c0Tx.hdr.base.mdata,clAddrToByteAddr(fiu.c0Tx.hdr.base.address),req_rd_addr);
          end 
          if(cci_c1Rx_isWriteRsp(fiu.c1Rx)) begin 
             $display("%d:RESP WR:%d",total_clocks,fiu.c1Rx.hdr.mdata); 
          end
          if(cci_c0Rx_isReadRsp(fiu.c0Rx))
          begin
            $display("%d:RESP READ:%d %x",total_clocks,fiu.c0Rx.hdr.mdata,fiu.c0Rx.data);
          end
  end 
  //synthesis translate_on 
`endif
    //
    // This AFU never handles MMIO reads.  MMIO is managed in the CSR module.
    //    
    assign fiu.c2Tx.mmioRdValid = 1'b0;
  
endmodule // app_afu
