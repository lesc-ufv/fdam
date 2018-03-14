
module afu_manager #
(
  parameter ADDR_WIDTH = 48,
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
  input [64-1:0] rst_afus,
  input [64-1:0] start_afus,
  input [2-1:0] conf_valid,
  input [128-1:0] conf,
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

  localparam DSM_DATA_WIDTH = 512;

  wire [1-1:0] req_wr_en_in;
  wire [(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)*1-1:0] req_wr_data_in;
  wire [1-1:0] req_wr_available_in;

  wire req_wr_available_out;
  wire req_wr_en_out;
  wire [DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH-1:0] req_wr_data_out;

  wire [1-1:0] req_rd_en_in;
  wire [(ADDR_WIDTH+TAG_WIDTH)*1-1:0] req_rd_data_in;
  wire [1-1:0] req_rd_available_in;

  wire req_rd_available_out;
  wire req_rd_en_out;
  wire [ADDR_WIDTH+TAG_WIDTH-1:0] req_rd_data_out;


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
    .FIFO_FULL(FIFO_FULL),
    .DSM_DATA_WIDTH(DSM_DATA_WIDTH)
  )
  afu_0_1x1
  (
    .clk(clk),
    .rst(rst | rst_afus[0]),
    .start(start_afus[0]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[0:0]),
    .request_read(req_rd_en_in[0:0]),
    .request_data(req_rd_data_in[1*(0*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:1*(0*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[0:0]),
    .request_write(req_wr_en_in[0:0]),
    .write_data(req_wr_data_in[1*(0*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:1*(0*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  arbiter_controller_top_1
  #(
    .DATA_WIDTH(ADDR_WIDTH + TAG_WIDTH),
    .INPUT_FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
    .OUTPUT_FIFO_DEPTH_BITS(FIFO_DEPTH_BITS)
  )
  arbiter_controller_tree_in
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_rd_en_in),
    .req_wr_data_in(req_rd_data_in),
    .req_wr_available_in(req_rd_available_in),
    .req_wr_available_out(req_rd_available_out),
    .req_wr_en_out(req_rd_en_out),
    .req_wr_data_out(req_rd_data_out)
  );


  arbiter_controller_top_1
  #(
    .DATA_WIDTH(DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH),
    .INPUT_FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
    .OUTPUT_FIFO_DEPTH_BITS(FIFO_DEPTH_BITS)
  )
  arbiter_controller_tree_out
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_wr_en_in),
    .req_wr_data_in(req_wr_data_in),
    .req_wr_available_in(req_wr_available_in),
    .req_wr_available_out(req_wr_available_out),
    .req_wr_en_out(req_wr_en_out),
    .req_wr_data_out(req_wr_data_out)
  );

  assign req_rd_available_out = req_rd_available;
  assign req_wr_available_out = req_wr_available;

  always @(posedge clk) begin
    if(rst) begin
      req_rd_en <= 1'b0;
      req_rd_addr <= 0;
      req_rd_mdata <= 0;
    end else begin
      req_rd_en <= req_rd_en_out;
      req_rd_addr <= req_rd_data_out[TAG_WIDTH+ADDR_WIDTH-1:TAG_WIDTH];
      req_rd_mdata <= req_rd_data_out[TAG_WIDTH-1:0];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      req_wr_en <= 1'b0;
      req_wr_addr <= 0;
      req_wr_mdata <= 0;
      req_wr_data <= 0;
    end else begin
      req_wr_en <= req_wr_en_out;
      req_wr_data <= req_wr_data_out[DATA_WIDTH+TAG_WIDTH+ADDR_WIDTH-1:TAG_WIDTH+ADDR_WIDTH];
      req_wr_addr <= req_wr_data_out[TAG_WIDTH+ADDR_WIDTH-1:TAG_WIDTH];
      req_wr_mdata <= req_wr_data_out[TAG_WIDTH-1:0];
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
