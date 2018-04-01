
module acc_user_0 #
(
  parameter DATA_WIDTH = 32,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1
)
(
  input clk,
  input rst,
  input start,
  input [1-1:0] acc_user_done_rd_data,
  input [1-1:0] acc_user_done_wr_data,
  input [1-1:0] acc_user_available_read,
  input [512-1:0] acc_user_read_data,
  output [1-1:0] acc_user_request_read,
  input [1-1:0] acc_user_read_data_valid,
  input [1-1:0] acc_user_available_write,
  output [512-1:0] acc_user_write_data,
  output [1-1:0] acc_user_request_write,
  output reg acc_user_done
);

  wire [1-1:0] n0;
  wire [1-1:0] n1;
  wire [34-1:0] n2;
  wire [2-1:0] streams_ready;
  wire en;
  assign en = &streams_ready;
  wire [1-1:0] done_wire;

  always @(posedge clk) begin
    acc_user_done <= &done_wire;
  end


  AsyncIn
  i0
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .rd_done(acc_user_done_rd_data[0]),
    .rd_available(acc_user_available_read[0]),
    .rd_valid(acc_user_read_data_valid[0]),
    .rd_data(acc_user_read_data[511:0]),
    .rd_en(acc_user_request_read[0]),
    .ackr0(n0),
    .reqr0(n1),
    .dout0(n2)
  );


  AsyncOut
  i1
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .wr_available(acc_user_available_write[0]),
    .wr_data(acc_user_write_data[511:0]),
    .wr_en(acc_user_request_write[0]),
    .done(done_wire[0]),
    .ackl0(n0),
    .reql0(n1),
    .din0(n2)
  );


endmodule
