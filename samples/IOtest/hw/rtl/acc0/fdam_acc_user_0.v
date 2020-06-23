module fdam_acc_user_0
(
  input clk,
  input rst,
  input start,
  input [32-1:0] acc_user_done_rd_data,
  input [32-1:0] acc_user_done_wr_data,
  input [32-1:0] acc_user_available_read,
  output reg [32-1:0] acc_user_request_read,
  input [32-1:0] acc_user_read_data_valid,
  input [16384-1:0] acc_user_read_data,
  input [32-1:0] acc_user_available_write,
  output reg [32-1:0] acc_user_request_write,
  output reg [16384-1:0] acc_user_write_data,
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