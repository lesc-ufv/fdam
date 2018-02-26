
module arbiter_controller_top_16 #
(
  parameter DATA_WIDTH = 32,
  parameter INPUT_FIFO_DEPTH_BITS = 4,
  parameter OUTPUT_FIFO_DEPTH_BITS = 4
)
(
  input clk,
  input rst,
  input [16-1:0] req_wr_en_in,
  input [DATA_WIDTH*16-1:0] req_wr_data_in,
  output [16-1:0] req_wr_available_in,
  input req_wr_available_out,
  output req_wr_en_out,
  output [DATA_WIDTH-1:0] req_wr_data_out
);

  wire [4-1:0] req_wr_available_out_0;
  wire [4-1:0] req_wr_en_out_0;
  wire [DATA_WIDTH*4-1:0] req_wr_data_out_0;

  arbiter_controller_4
  #(
    .DATA_WIDTH(DATA_WIDTH),
    .INPUT_FIFO_DEPTH_BITS(INPUT_FIFO_DEPTH_BITS),
    .OUTPUT_FIFO_DEPTH_BITS(OUTPUT_FIFO_DEPTH_BITS)
  )
  arbiter_controller_0_0
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_wr_en_in[3:0]),
    .req_wr_data_in(req_wr_data_in[DATA_WIDTH*4 - 1:DATA_WIDTH*0]),
    .req_wr_available_in(req_wr_available_in[3:0]),
    .req_wr_available_out(req_wr_available_out_0[0]),
    .req_wr_en_out(req_wr_en_out_0[0]),
    .req_wr_data_out(req_wr_data_out_0[DATA_WIDTH*1 - 1:DATA_WIDTH*0])
  );


  arbiter_controller_4
  #(
    .DATA_WIDTH(DATA_WIDTH),
    .INPUT_FIFO_DEPTH_BITS(INPUT_FIFO_DEPTH_BITS),
    .OUTPUT_FIFO_DEPTH_BITS(OUTPUT_FIFO_DEPTH_BITS)
  )
  arbiter_controller_0_1
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_wr_en_in[7:4]),
    .req_wr_data_in(req_wr_data_in[DATA_WIDTH*8 - 1:DATA_WIDTH*4]),
    .req_wr_available_in(req_wr_available_in[7:4]),
    .req_wr_available_out(req_wr_available_out_0[1]),
    .req_wr_en_out(req_wr_en_out_0[1]),
    .req_wr_data_out(req_wr_data_out_0[DATA_WIDTH*2 - 1:DATA_WIDTH*1])
  );


  arbiter_controller_4
  #(
    .DATA_WIDTH(DATA_WIDTH),
    .INPUT_FIFO_DEPTH_BITS(INPUT_FIFO_DEPTH_BITS),
    .OUTPUT_FIFO_DEPTH_BITS(OUTPUT_FIFO_DEPTH_BITS)
  )
  arbiter_controller_0_2
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_wr_en_in[11:8]),
    .req_wr_data_in(req_wr_data_in[DATA_WIDTH*12 - 1:DATA_WIDTH*8]),
    .req_wr_available_in(req_wr_available_in[11:8]),
    .req_wr_available_out(req_wr_available_out_0[2]),
    .req_wr_en_out(req_wr_en_out_0[2]),
    .req_wr_data_out(req_wr_data_out_0[DATA_WIDTH*3 - 1:DATA_WIDTH*2])
  );


  arbiter_controller_4
  #(
    .DATA_WIDTH(DATA_WIDTH),
    .INPUT_FIFO_DEPTH_BITS(INPUT_FIFO_DEPTH_BITS),
    .OUTPUT_FIFO_DEPTH_BITS(OUTPUT_FIFO_DEPTH_BITS)
  )
  arbiter_controller_0_3
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_wr_en_in[15:12]),
    .req_wr_data_in(req_wr_data_in[DATA_WIDTH*16 - 1:DATA_WIDTH*12]),
    .req_wr_available_in(req_wr_available_in[15:12]),
    .req_wr_available_out(req_wr_available_out_0[3]),
    .req_wr_en_out(req_wr_en_out_0[3]),
    .req_wr_data_out(req_wr_data_out_0[DATA_WIDTH*4 - 1:DATA_WIDTH*3])
  );


  arbiter_controller_4
  #(
    .DATA_WIDTH(DATA_WIDTH),
    .INPUT_FIFO_DEPTH_BITS(INPUT_FIFO_DEPTH_BITS),
    .OUTPUT_FIFO_DEPTH_BITS(OUTPUT_FIFO_DEPTH_BITS)
  )
  arbiter_controller_1_0
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_wr_en_out_0),
    .req_wr_data_in(req_wr_data_out_0),
    .req_wr_available_in(req_wr_available_out_0),
    .req_wr_available_out(req_wr_available_out),
    .req_wr_en_out(req_wr_en_out),
    .req_wr_data_out(req_wr_data_out)
  );


endmodule
