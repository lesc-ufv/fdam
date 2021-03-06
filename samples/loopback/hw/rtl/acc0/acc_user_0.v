
module acc_user_0 #
(
  parameter DATA_WIDTH = 512,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1
)
(
  input clk,
  input rst,
  input start,
  input [NUM_INPUT_QUEUES-1:0] acc_user_done_rd_data,
  input [NUM_OUTPUT_QUEUES-1:0] acc_user_done_wr_data,
  input [NUM_INPUT_QUEUES-1:0] acc_user_available_read,
  output reg [NUM_INPUT_QUEUES-1:0] acc_user_request_read,
  input [NUM_INPUT_QUEUES-1:0] acc_user_read_data_valid,
  input [DATA_WIDTH*NUM_INPUT_QUEUES-1:0] acc_user_read_data,
  input [NUM_OUTPUT_QUEUES-1:0] acc_user_available_write,
  output reg [NUM_OUTPUT_QUEUES-1:0] acc_user_request_write,
  output reg [DATA_WIDTH*NUM_OUTPUT_QUEUES-1:0] acc_user_write_data,
  output reg acc_user_done
);


  //Sample code for loopback:

  always @(posedge clk) begin
    if(rst) begin
      acc_user_request_read <= 0;
      acc_user_request_write <= 0;
      acc_user_write_data <= 0;
      acc_user_done <= 0;
    end else begin
      if(start) begin
        acc_user_done <= acc_user_done_rd_data & acc_user_done_wr_data;
        acc_user_request_read <= acc_user_available_write & acc_user_available_read;
        acc_user_write_data <= acc_user_read_data;
        acc_user_request_write <= acc_user_read_data_valid;
      end 
    end
  end


endmodule
