
module uut_interface2_1_0
(
  input clk,
  input rst,
  input rst_parc,
  input start,
  input available_write0,
  input available_read0,
  input available_read1,
  input almost_empty_read0,
  input almost_empty_read1,
  output req_rd_data0,
  output req_rd_data1,
  input [512-1:0] rd_data0,
  input [512-1:0] rd_data1,
  output req_wr_data0,
  output [512-1:0] wr_data0,
  output done
);


endmodule
