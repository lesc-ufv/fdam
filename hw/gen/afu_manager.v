
module afu_manager #
(
  parameter ADDR_WIDTH = 64,
  parameter QTD_WIDTH = 32,
  parameter DATA_WIDTH = 512,
  parameter CONF_ID_QUEUE_WIDTH = 32,
  parameter TAG_WIDTH = 16,
  parameter FIFO_DEPTH_BITS = 4,
  parameter FIFO_FULL = 2 ** FIFO_DEPTH_BITS
)
(
  input clk,
  input rst,
  input [1-1:0] rst_afus,
  input [1-1:0] start_afus,
  input [2-1:0] conf_valid,
  input [ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0] conf,
  output reg req_rd_en,
  output reg [ADDR_WIDTH-1:0] req_rd_addr,
  output reg [TAG_WIDTH-1:0] req_rd_mdata,
  input req_rd_available,
  input resp_rd_valid,
  input [DATA_WIDTH-1:0] resp_rd_data,
  input [TAG_WIDTH-1:0] resp_rd_mdata,
  input req_wr_available,
  output reg req_wr_en,
  output reg [ADDR_WIDTH-1:0] req_wr_addr,
  output reg [TAG_WIDTH-1:0] req_wr_mdata,
  output reg [DATA_WIDTH-1:0] req_wr_data,
  input resp_wr_valid,
  input [TAG_WIDTH-1:0] resp_wr_mdata,
  output reg [512-1:0] info
);

  reg [1-1:0] rd_req_arbiter_request;
  wire [1-1:0] rd_req_arbiter_grant;
  wire rd_req_arbiter_grant_valid;
  wire [1-1:0] rd_req_arbiter_grant_encoded;


  reg [1-1:0] wr_req_arbiter_request;
  wire [1-1:0] wr_req_arbiter_grant;
  wire wr_req_arbiter_grant_valid;
  wire [1-1:0] wr_req_arbiter_grant_encoded;


  genvar idx_req_rd_fifo;
  reg [1-1:0] req_rd_fifo_re;
  wire [1-1:0] req_rd_fifo_valid;
  wire [1-1:0] req_rd_fifo_empty;
  wire [1-1:0] req_rd_fifo_amostempty;
  wire [(ADDR_WIDTH+TAG_WIDTH)*1-1:0] req_rd_fifo_dout;
  wire [1-1:0] afu_req_rd_fifo_we;
  wire [(ADDR_WIDTH+TAG_WIDTH)*1-1:0] afu_req_rd_fifo_din;
  wire [1-1:0] afu_req_rd_fifo_full;
  wire [1-1:0] afu_req_rd_fifo_almostfull;
  wire [(FIFO_DEPTH_BITS+1)*1-1:0] req_rd_fifo_count;


  genvar idx_req_wr_fifo;
  reg [1-1:0] req_wr_fifo_re;
  wire [1-1:0] req_wr_fifo_valid;
  wire [1-1:0] req_wr_fifo_empty;
  wire [1-1:0] req_wr_fifo_amostempty;
  wire [(ADDR_WIDTH+DATA_WIDTH+TAG_WIDTH)*1-1:0] req_wr_fifo_dout;
  wire [1-1:0] afu_req_wr_fifo_we;
  wire [(ADDR_WIDTH+DATA_WIDTH+TAG_WIDTH)*1-1:0] afu_req_wr_fifo_din;
  wire [1-1:0] afu_req_wr_fifo_full;
  wire [1-1:0] afu_req_wr_fifo_almostfull;
  wire [(FIFO_DEPTH_BITS+1)*1-1:0] req_wr_fifo_count;


  wire [ADDR_WIDTH+TAG_WIDTH-1:0] req_rd_fifo_sel_dout;
  wire req_rd_fifo_sel_dout_valid;
  wire [DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH-1:0] req_wr_fifo_sel_dout;
  wire req_wr_fifo_sel_dout_valid;


  localparam FSM_REQ_RD_IDLE = 0;
  localparam FSM_REQ_RD_WAIT = 1;
  reg [1-1:0] fsm_req_rd;
  localparam FSM_REQ_WR_IDLE = 0;
  localparam FSM_REQ_WR_WAIT = 1;
  reg [1-1:0] fsm_req_wr;

  arbiter
  #(
    .PORTS(1),
    .TYPE("ROUND_ROBIN"),
    .BLOCK("NONE"),
    .LSB_PRIORITY("LOW")
  )
  rd_req_arbiter
  (
    .clk(clk),
    .rst(rst),
    .request(rd_req_arbiter_request),
    .acknowledge(1'd0),
    .grant(rd_req_arbiter_grant),
    .grant_valid(rd_req_arbiter_grant_valid),
    .grant_encoded(rd_req_arbiter_grant_encoded)
  );


  arbiter
  #(
    .PORTS(1),
    .TYPE("ROUND_ROBIN"),
    .BLOCK("NONE"),
    .LSB_PRIORITY("LOW")
  )
  wr_req_arbiter
  (
    .clk(clk),
    .rst(rst),
    .request(wr_req_arbiter_request),
    .acknowledge(1'd0),
    .grant(wr_req_arbiter_grant),
    .grant_valid(wr_req_arbiter_grant_valid),
    .grant_encoded(wr_req_arbiter_grant_encoded)
  );


  generate for(idx_req_rd_fifo=0; idx_req_rd_fifo<1; idx_req_rd_fifo=idx_req_rd_fifo+1) begin : gen_req_rd_fifos

    fifo
    #(
      .FIFO_WIDTH(ADDR_WIDTH + TAG_WIDTH),
      .FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
      .FIFO_ALMOSTFULL_THRESHOLD(FIFO_FULL - 2),
      .FIFO_ALMOSTEMPTY_THRESHOLD(2)
    )
    req_rd_fifo
    (
      .clk(clk),
      .rst(rst),
      .we(afu_req_rd_fifo_we[idx_req_rd_fifo]),
      .din(afu_req_rd_fifo_din[idx_req_rd_fifo*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH)-1:idx_req_rd_fifo*(ADDR_WIDTH+TAG_WIDTH)]),
      .re(req_rd_fifo_re[idx_req_rd_fifo]),
      .valid(req_rd_fifo_valid[idx_req_rd_fifo]),
      .dout(req_rd_fifo_dout[idx_req_rd_fifo*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH)-1:idx_req_rd_fifo*(ADDR_WIDTH+TAG_WIDTH)]),
      .count(req_rd_fifo_count[idx_req_rd_fifo*(FIFO_DEPTH_BITS+1)+(FIFO_DEPTH_BITS+1)-1:idx_req_rd_fifo*(FIFO_DEPTH_BITS+1)]),
      .empty(req_rd_fifo_empty[idx_req_rd_fifo]),
      .full(afu_req_rd_fifo_full[idx_req_rd_fifo]),
      .almostfull(afu_req_rd_fifo_almostfull[idx_req_rd_fifo]),
      .almostempty(req_rd_fifo_amostempty[idx_req_rd_fifo])
    );

  end
  endgenerate


  generate for(idx_req_wr_fifo=0; idx_req_wr_fifo<1; idx_req_wr_fifo=idx_req_wr_fifo+1) begin : gen_req_wr_fifos

    fifo
    #(
      .FIFO_WIDTH(ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH),
      .FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
      .FIFO_ALMOSTFULL_THRESHOLD(FIFO_FULL - 2),
      .FIFO_ALMOSTEMPTY_THRESHOLD(2)
    )
    req_wr_fifo
    (
      .clk(clk),
      .rst(rst),
      .we(afu_req_wr_fifo_we[idx_req_wr_fifo]),
      .din(afu_req_wr_fifo_din[idx_req_wr_fifo*(ADDR_WIDTH+DATA_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+DATA_WIDTH+TAG_WIDTH)-1:idx_req_wr_fifo*(ADDR_WIDTH+DATA_WIDTH+TAG_WIDTH)]),
      .re(req_wr_fifo_re[idx_req_wr_fifo]),
      .valid(req_wr_fifo_valid[idx_req_wr_fifo]),
      .dout(req_wr_fifo_dout[idx_req_wr_fifo*(ADDR_WIDTH+DATA_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+DATA_WIDTH+TAG_WIDTH)-1:idx_req_wr_fifo*(ADDR_WIDTH+DATA_WIDTH+TAG_WIDTH)]),
      .count(req_wr_fifo_count[idx_req_wr_fifo*(FIFO_DEPTH_BITS+1)+(FIFO_DEPTH_BITS+1)-1:idx_req_wr_fifo*(FIFO_DEPTH_BITS+1)]),
      .empty(req_wr_fifo_empty[idx_req_wr_fifo]),
      .full(afu_req_wr_fifo_full[idx_req_wr_fifo]),
      .almostfull(afu_req_wr_fifo_almostfull[idx_req_wr_fifo]),
      .almostempty(req_wr_fifo_amostempty[idx_req_wr_fifo])
    );

  end
  endgenerate


  afu_0
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(0),
    .INITIAL_OUTPUT_QUEUE_ID(0),
    .NUM_INPUT_QUEUES(1),
    .NUM_OUTPUT_QUEUES(1),
    .TAG_WIDTH(TAG_WIDTH),
    .FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
    .FIFO_FULL(FIFO_FULL)
  )
  afu_0_1x1
  (
    .clk(clk),
    .rst(rst | rst_afus[0]),
    .start(start_afus[0]),
    .conf_valid(conf_valid),
    .conf(conf),
    .available_read(~afu_req_rd_fifo_almostfull[0:0]),
    .request_read(afu_req_rd_fifo_we[0:0]),
    .request_data(afu_req_rd_fifo_din[1*(0*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:1*(0*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(~afu_req_wr_fifo_almostfull[0:0]),
    .request_write(afu_req_wr_fifo_we[0:0]),
    .write_data(afu_req_wr_fifo_din[1*(0*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:1*(0*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  select_ff
  #(
    .DATA_WIDTH(ADDR_WIDTH + TAG_WIDTH)
  )
  req_rd_fifo_sel
  (
    .clk(clk),
    .rst(rst),
    .data_in_valid(req_rd_fifo_valid),
    .data_in_0(req_rd_fifo_dout[(0*(ADDR_WIDTH+TAG_WIDTH)+ADDR_WIDTH+TAG_WIDTH)-1:0*(ADDR_WIDTH+TAG_WIDTH)]),
    .data_out_valid(req_rd_fifo_sel_dout_valid),
    .data_out(req_rd_fifo_sel_dout)
  );


  select_ff
  #(
    .DATA_WIDTH(DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH)
  )
  req_wr_fifo_sel
  (
    .clk(clk),
    .rst(rst),
    .data_in_valid(req_wr_fifo_valid),
    .data_in_0(req_wr_fifo_dout[(0*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)+DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)-1:0*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)]),
    .data_out_valid(req_wr_fifo_sel_dout_valid),
    .data_out(req_wr_fifo_sel_dout)
  );


  always @(posedge clk) begin
    if(rst) begin
      fsm_req_rd <= 0;
      rd_req_arbiter_request <= 0;
      req_rd_fifo_re <= 0;
    end else begin
      rd_req_arbiter_request <= 0;
      req_rd_fifo_re <= 0;
      case(fsm_req_rd)
        FSM_REQ_RD_IDLE: begin
          if(req_rd_available) begin
            rd_req_arbiter_request <= ~req_rd_fifo_empty;
          end 
          if(rd_req_arbiter_grant_valid) begin
            req_rd_fifo_re <= rd_req_arbiter_grant;
            fsm_req_rd <= FSM_REQ_RD_WAIT;
          end 
        end
        FSM_REQ_RD_WAIT: begin
          if(req_rd_en) begin
            fsm_req_rd <= FSM_REQ_RD_IDLE;
          end 
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      fsm_req_wr <= 0;
      wr_req_arbiter_request <= 0;
      req_wr_fifo_re <= 0;
    end else begin
      wr_req_arbiter_request <= 0;
      req_wr_fifo_re <= 0;
      case(fsm_req_wr)
        FSM_REQ_WR_IDLE: begin
          if(req_wr_available) begin
            wr_req_arbiter_request <= ~req_wr_fifo_empty;
          end 
          if(wr_req_arbiter_grant_valid) begin
            req_wr_fifo_re <= wr_req_arbiter_grant;
            fsm_req_wr <= FSM_REQ_WR_WAIT;
          end 
        end
        FSM_REQ_WR_WAIT: begin
          if(req_wr_en) begin
            fsm_req_wr <= FSM_REQ_WR_IDLE;
          end 
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      req_rd_en <= 1'b0;
      req_rd_addr <= 0;
      req_rd_mdata <= 0;
    end else begin
      req_rd_en <= 1'b0;
      if(req_rd_fifo_sel_dout_valid) begin
        req_rd_en <= 1'b1;
        req_rd_mdata <= req_rd_fifo_sel_dout[TAG_WIDTH-1:0];
        req_rd_addr <= req_rd_fifo_sel_dout[ADDR_WIDTH+TAG_WIDTH-1:TAG_WIDTH];
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      req_wr_en <= 1'b0;
      req_wr_addr <= 0;
      req_wr_mdata <= 0;
      req_wr_data <= 0;
    end else begin
      req_wr_en <= 1'b0;
      if(req_wr_fifo_sel_dout_valid) begin
        req_wr_en <= 1'b1;
        req_wr_mdata <= req_wr_fifo_sel_dout[TAG_WIDTH-1:0];
        req_wr_addr <= req_wr_fifo_sel_dout[ADDR_WIDTH+TAG_WIDTH-1:TAG_WIDTH];
        req_wr_data <= req_wr_fifo_sel_dout[DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH-1:ADDR_WIDTH+TAG_WIDTH];
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      info <= 512'd0;
    end else begin
      info[15:0] <= {8'd1, 8'd1};

    end
  end


endmodule
