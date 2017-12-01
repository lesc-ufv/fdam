
module buffer_controller
(
  input clk,
  input rst,
  input rst_parc,
  input start,
  input [64-1:0] workspace_addr_base,
  output reg req_rd_en,
  output reg [64-1:0] req_rd_addr,
  output reg [16-1:0] req_rd_mdata,
  input req_rd_available,
  input resp_rd_valid,
  input [512-1:0] resp_rd_data,
  input [16-1:0] resp_rd_mdata,
  input req_wr_available,
  output reg req_wr_en,
  output reg [64-1:0] req_wr_addr,
  output reg [16-1:0] req_wr_mdata,
  output reg [512-1:0] req_wr_data,
  input resp_wr_valid,
  input [16-1:0] resp_wr_mdata
);

  reg [512-1:0] resp_rd_data_tmp;
  wire done;
  reg [512-1:0] dsm_word;
  reg [512-1:0] dsm_word_tmp;
  wire [1-1:0] done_uut_interface;
  wire [2-1:0] done_read;
  wire [1-1:0] done_write_back;
  localparam FIFO_IN_FULL = 32;
  localparam FSM_RD_REQ_READ_CONF = 0;
  localparam FSM_RD_REQ_READ_DATA = 1;
  reg [2-1:0] fsm_rd;
  localparam FSM_WR_REQ_IDLE = 0;
  localparam FSM_WR_REQ_WRITE_DATA = 1;
  reg [2-1:0] fsm_wr;
  reg [512-1:0] confs [0:4-1];
  reg [3-1:0] counter_received_conf;
  reg [3-1:0] addr_offset_conf;
  wire [128-1:0] qtde_data_in_all;
  wire [128-1:0] addr_in_all;
  reg [64-1:0] counter_received_data_in [0:2-1];
  reg [64-1:0] addr_offset_data_in [0:2-1];
  wire [64-1:0] qtde_data_in [0:2-1];
  wire [64-1:0] addr_in [0:2-1];
  wire [64-1:0] qtde_data_out_all;
  wire [64-1:0] addr_out_all;
  reg [64-1:0] counter_sent_data_out [0:1-1];
  reg [64-1:0] addr_offset_data_out [0:1-1];
  wire [64-1:0] qtde_data_out [0:1-1];
  wire [64-1:0] addr_out [0:1-1];
  wire [2-1:0] fifos_in_read_request;
  wire [2-1:0] fifos_out_read_request;
  reg [2-1:0] we_fifo_in;
  reg [1-1:0] re_fifo_out;
  wire [512-1:0] dout_fifo_out [0:1-1];
  wire [1-1:0] almostempty_fifo_out;
  wire [5-1:0] count_fifo_out [0:1-1];
  reg start_rd_conf;
  reg start_rd_wr_data;
  wire rst_parc_restart;
  assign rst_parc_restart = rst_parc | rst;
  assign done_read[0] = counter_received_data_in[0] >= qtde_data_in[0];
  assign done_read[1] = counter_received_data_in[1] >= qtde_data_in[1];
  assign done_write_back[0] = counter_sent_data_out[0] >= qtde_data_out[0];
  assign done = &done_write_back;
  assign addr_in_all = confs[0];
  assign qtde_data_in_all = confs[1];
  genvar i;

  generate for(i=0; i<2; i=i+1) begin : gen_1
    assign addr_in[i] = addr_in_all[i*64+64-1:i*64];
  end
  endgenerate

  genvar j;

  generate for(j=0; j<2; j=j+1) begin : gen_2
    assign qtde_data_in[j] = qtde_data_in_all[j*64+64-1:j*64];
  end
  endgenerate

  assign addr_out_all = confs[2];
  assign qtde_data_out_all = confs[3];
  genvar k;

  generate for(k=0; k<1; k=k+1) begin : gen_3
    assign addr_out[k] = addr_out_all[k*64+64-1:k*64];
  end
  endgenerate

  genvar l;

  generate for(l=0; l<1; l=l+1) begin : gen_4
    assign qtde_data_out[l] = qtde_data_out_all[l*64+64-1:l*64];
  end
  endgenerate

  wire re_fifo_in0;
  wire almostempty_fifo_in0;
  wire empty_fifo_in0;
  wire almostfull_fifo_in0;
  wire full_fifo_in0;
  wire [5-1:0] count_fifo_in0;
  wire [512-1:0] dout_fifo_in0;
  assign fifos_in_read_request[0] = addr_offset_data_in[0] - counter_received_data_in[0] + count_fifo_in0 < FIFO_IN_FULL;

  fifo
  #(
    .FIFO_WIDTH(512),
    .FIFO_DEPTH_BITS(5)
  )
  fifo_in0
  (
    .clk(clk),
    .rst(rst_parc_restart),
    .we(we_fifo_in[0]),
    .din(resp_rd_data_tmp),
    .re(re_fifo_in0),
    .dout(dout_fifo_in0),
    .count(count_fifo_in0),
    .empty(empty_fifo_in0),
    .almostempty(almostempty_fifo_in0),
    .full(full_fifo_in0),
    .almostfull(almostfull_fifo_in0)
  );

  wire re_fifo_in1;
  wire almostempty_fifo_in1;
  wire empty_fifo_in1;
  wire almostfull_fifo_in1;
  wire full_fifo_in1;
  wire [5-1:0] count_fifo_in1;
  wire [512-1:0] dout_fifo_in1;
  assign fifos_in_read_request[1] = addr_offset_data_in[1] - counter_received_data_in[1] + count_fifo_in1 < FIFO_IN_FULL;

  fifo
  #(
    .FIFO_WIDTH(512),
    .FIFO_DEPTH_BITS(5)
  )
  fifo_in1
  (
    .clk(clk),
    .rst(rst_parc_restart),
    .we(we_fifo_in[1]),
    .din(resp_rd_data_tmp),
    .re(re_fifo_in1),
    .dout(dout_fifo_in1),
    .count(count_fifo_in1),
    .empty(empty_fifo_in1),
    .almostempty(almostempty_fifo_in1),
    .full(full_fifo_in1),
    .almostfull(almostfull_fifo_in1)
  );

  wire we_fifo_out0;
  wire empty_fifo_out0;
  wire almostfull_fifo_out0;
  wire full_fifo_out0;
  wire [512-1:0] din_fifo_out0;
  assign fifos_out_read_request[0] = ~empty_fifo_out0;

  fifo
  #(
    .FIFO_WIDTH(512),
    .FIFO_DEPTH_BITS(5)
  )
  fifo_out0
  (
    .clk(clk),
    .rst(rst_parc_restart),
    .we(we_fifo_out0),
    .din(din_fifo_out0),
    .re(re_fifo_out[0]),
    .dout(dout_fifo_out[0]),
    .count(count_fifo_out[0]),
    .empty(empty_fifo_out0),
    .almostempty(almostempty_fifo_out[0]),
    .full(full_fifo_out0),
    .almostfull(almostfull_fifo_out0)
  );

  wire [2-1:0] grant_in;
  wire grant_in_valid;
  wire grant_in_index;

  arbiter
  #(
    .PORTS(2),
    .TYPE("ROUND_ROBIN"),
    .BLOCK("NONE"),
    .LSB_PRIORITY("LOW")
  )
  arbiter_in
  (
    .clk(clk),
    .rst(rst),
    .request(fifos_in_read_request),
    .acknowledge(2'd0),
    .grant(grant_in),
    .grant_valid(grant_in_valid),
    .grant_encoded(grant_in_index)
  );


  always @(*) begin
    if(rst) begin
      dsm_word <= 512'd0;
    end else begin
      dsm_word[0] <= done;
      dsm_word[2:1] <= done_read;
      dsm_word[3:3] <= done_write_back;
      dsm_word[4:4] <= done_uut_interface;
    end
  end

  integer rst_counter_index;

  always @(posedge clk or posedge rst_parc_restart) begin
    if(rst) begin
      start_rd_conf <= 1'b0;
      addr_offset_conf <= 3'd1;
      req_rd_en <= 1'b0;
      req_rd_addr <= 64'b0;
      req_rd_mdata <= 16'b0;
      fsm_rd <= FSM_RD_REQ_READ_CONF;
      for(rst_counter_index=0; rst_counter_index<2; rst_counter_index=rst_counter_index+1) begin
        addr_offset_data_in[rst_counter_index] <= 64'd0;
      end
    end else begin
      if(rst_parc_restart) begin
        req_rd_en <= 1'b0;
        req_rd_addr <= 64'b0;
        req_rd_mdata <= 16'b0;
        fsm_rd <= FSM_RD_REQ_READ_DATA;
        for(rst_counter_index=0; rst_counter_index<2; rst_counter_index=rst_counter_index+1) begin
          addr_offset_data_in[rst_counter_index] <= 64'd0;
        end
      end else begin
        if(start) begin
          req_rd_en <= 1'b0;
          case(fsm_rd)
            FSM_RD_REQ_READ_CONF: begin
              if(addr_offset_conf <= 4) begin
                start_rd_conf <= 1'b1;
                if(req_rd_available) begin
                  addr_offset_conf <= addr_offset_conf + 3'd1;
                  req_rd_addr <= workspace_addr_base[63:6] + addr_offset_conf;
                  req_rd_mdata <= addr_offset_conf;
                  req_rd_en <= 1'b1;
                end 
              end else if(start_rd_wr_data) begin
                fsm_rd <= FSM_RD_REQ_READ_DATA;
                start_rd_conf <= 1'b0;
              end 
            end
            FSM_RD_REQ_READ_DATA: begin
              if(req_rd_available && grant_in_valid && (addr_offset_data_in[grant_in_index] < qtde_data_in[grant_in_index])) begin
                addr_offset_data_in[grant_in_index] <= addr_offset_data_in[grant_in_index] + 64'd1;
                req_rd_addr <= addr_in[grant_in_index][63:6] + addr_offset_data_in[grant_in_index];
                req_rd_en <= 1'b1;
                req_rd_mdata <= grant_in_index;
              end 
            end
          endcase
        end 
      end
    end
  end

  reg counter_write_dsm;
  reg counter_written_dsm;

  always @(posedge clk or posedge rst_parc_restart) begin
    if(rst_parc_restart) begin
      counter_write_dsm <= 1'b0;
      req_wr_en <= 1'd0;
      req_wr_addr <= 64'd0;
      req_wr_mdata <= 16'd0;
      req_wr_data <= 512'd0;
      re_fifo_out <= 1'd0;
      dsm_word_tmp <= dsm_word;
      fsm_wr <= FSM_WR_REQ_IDLE;
      for(rst_counter_index=0; rst_counter_index<2; rst_counter_index=rst_counter_index+1) begin
        addr_offset_data_out[rst_counter_index] <= 64'd0;
      end
    end else begin
      if(start_rd_wr_data) begin
        req_wr_en <= 1'b0;
        re_fifo_out <= 1'd0;
        case(fsm_wr)
          FSM_WR_REQ_IDLE: begin
            if(req_wr_available) begin
              if((dsm_word_tmp != dsm_word) && (counter_written_dsm == counter_write_dsm)) begin
                dsm_word_tmp <= dsm_word;
                req_wr_addr <= workspace_addr_base[63:6];
                req_wr_mdata <= 16'hffff;
                req_wr_data <= dsm_word;
                req_wr_en <= 1'b1;
                counter_write_dsm <= ~counter_write_dsm;
              end else if(fifos_out_read_request && (count_fifo_out[1'b0] > 5'b0)) begin
                re_fifo_out[1'b0] <= 1'b1;
                fsm_wr <= FSM_WR_REQ_WRITE_DATA;
              end 
            end 
          end
          FSM_WR_REQ_WRITE_DATA: begin
            addr_offset_data_out[1'b0] <= addr_offset_data_out[1'b0] + 64'd1;
            req_wr_addr <= addr_out[1'b0][63:6] + addr_offset_data_out[1'b0];
            req_wr_mdata <= 16'b0;
            req_wr_data <= dout_fifo_out[1'b0];
            req_wr_en <= 1'b1;
            if(fifos_out_read_request && !almostempty_fifo_out[1'b0] && req_wr_available && (dsm_word_tmp == dsm_word)) begin
              re_fifo_out[1'b0] <= 1'b1;
              fsm_wr <= FSM_WR_REQ_WRITE_DATA;
            end else begin
              fsm_wr <= FSM_WR_REQ_IDLE;
            end
          end
        endcase
      end 
    end
  end


  always @(posedge clk or posedge rst) begin
    if(rst) begin
      start_rd_wr_data <= 1'b0;
      counter_received_conf <= 1'b0;
      we_fifo_in <= 2'd0;
      resp_rd_data_tmp <= 512'd0;
      for(rst_counter_index=0; rst_counter_index<2; rst_counter_index=rst_counter_index+1) begin
        counter_received_data_in[rst_counter_index] <= 64'd0;
      end
      for(rst_counter_index=0; rst_counter_index<4; rst_counter_index=rst_counter_index+1) begin
        confs[rst_counter_index] <= 512'd0;
      end
    end else begin
      if(start_rd_conf && !start_rd_wr_data) begin
        if(counter_received_conf < 4) begin
          if(resp_rd_valid) begin
            counter_received_conf <= counter_received_conf + 3'd1;
            confs[counter_received_conf] <= resp_rd_data;
          end 
        end else begin
          start_rd_wr_data <= 1'b1;
        end
      end else if(start_rd_wr_data) begin
        we_fifo_in <= 2'd0;
        resp_rd_data_tmp <= resp_rd_data;
        if(resp_rd_valid) begin
          counter_received_data_in[resp_rd_mdata] <= counter_received_data_in[resp_rd_mdata] + 64'd1;
          we_fifo_in[resp_rd_mdata] <= 1'd1;
        end 
      end 
    end
  end


  always @(posedge clk or posedge rst_parc_restart) begin
    if(rst_parc_restart) begin
      counter_written_dsm <= 1'b0;
      for(rst_counter_index=0; rst_counter_index<2; rst_counter_index=rst_counter_index+1) begin
        counter_sent_data_out[rst_counter_index] <= 64'd0;
      end
    end else begin
      if(start_rd_wr_data) begin
        if(resp_wr_valid) begin
          if(resp_wr_mdata == 16'hffff) begin
            counter_written_dsm <= ~counter_written_dsm;
          end else begin
            counter_sent_data_out[resp_wr_mdata] <= counter_sent_data_out[resp_wr_mdata] + 64'd1;
          end
        end 
      end 
    end
  end


  uut_interface2_1_0
  uut_interface2_1_0
  (
    .clk(clk),
    .rst(rst),
    .rst_parc(rst_parc),
    .start(start_rd_wr_data),
    .available_read0(~empty_fifo_in0),
    .almost_empty_read0(almostempty_fifo_in0),
    .req_rd_data0(re_fifo_in0),
    .rd_data0(dout_fifo_in0),
    .available_read1(~empty_fifo_in1),
    .almost_empty_read1(almostempty_fifo_in1),
    .req_rd_data1(re_fifo_in1),
    .rd_data1(dout_fifo_in1),
    .available_write0(~almostfull_fifo_out0),
    .req_wr_data0(we_fifo_out0),
    .wr_data0(din_fifo_out0),
    .done(done_uut_interface[0])
  );


endmodule
