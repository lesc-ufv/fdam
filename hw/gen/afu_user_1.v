
module afu_user_1 #
(
  parameter DATA_WIDTH = 512,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1
)
(
  input clk,
  input rst,
  input start,
  input [NUM_INPUT_QUEUES-1:0] afu_user_done_rd_data,
  input [NUM_OUTPUT_QUEUES-1:0] afu_user_done_wr_data,
  input [NUM_INPUT_QUEUES-1:0] afu_user_available_read,
  output reg [NUM_INPUT_QUEUES-1:0] afu_user_request_read,
  input [NUM_INPUT_QUEUES-1:0] afu_user_read_data_valid,
  input [DATA_WIDTH*NUM_INPUT_QUEUES-1:0] afu_user_read_data,
  input [NUM_OUTPUT_QUEUES-1:0] afu_user_available_write,
  output reg [NUM_OUTPUT_QUEUES-1:0] afu_user_request_write,
  output reg [DATA_WIDTH*NUM_OUTPUT_QUEUES-1:0] afu_user_write_data,
  output reg afu_user_done
);


  /* user code here! */


endmodule
