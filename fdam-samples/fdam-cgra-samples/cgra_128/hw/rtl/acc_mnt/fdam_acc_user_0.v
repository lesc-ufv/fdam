module fdam_acc_user_0
(
  input clk,
  input rst,
  input start,
  input [8-1:0] acc_user_done_rd_data,
  input [8-1:0] acc_user_done_wr_data,
  input [8-1:0] acc_user_available_read,
  output reg [8-1:0] acc_user_request_read,
  input [8-1:0] acc_user_read_data_valid,
  input [4096-1:0] acc_user_read_data,
  input [8-1:0] acc_user_available_write,
  output reg [8-1:0] acc_user_request_write,
  output reg [4096-1:0] acc_user_write_data,
  output reg acc_user_done
);


  cgra0_acc
  cgra0_acc
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .acc_user_done_rd_data(acc_user_done_rd_data),
    .acc_user_done_wr_data(acc_user_done_wr_data),
    .acc_user_available_read(acc_user_available_read),
    .acc_user_request_read(acc_user_request_read),
    .acc_user_read_data_valid(acc_user_read_data_valid),
    .acc_user_read_data(acc_user_read_data),
    .acc_user_available_write(acc_user_available_write),
    .acc_user_request_write(acc_user_request_write),
    .acc_user_write_data(acc_user_write_data),
    .acc_user_done(acc_user_done)
  );

endmodule