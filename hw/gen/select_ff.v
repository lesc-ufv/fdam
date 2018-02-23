
module select_ff #
(
  parameter DATA_WIDTH = 32
)
(
  input clk,
  input rst,
  input [4-1:0] data_in_valid,
  input [DATA_WIDTH-1:0] data_in_0,
  input [DATA_WIDTH-1:0] data_in_1,
  input [DATA_WIDTH-1:0] data_in_2,
  input [DATA_WIDTH-1:0] data_in_3,
  output [DATA_WIDTH-1:0] data_out,
  output data_out_valid
);


  select_4
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  sel_0_0
  (
    .clk(clk),
    .rst(rst),
    .data_in_valid(data_in_valid),
    .data_in_0(data_in_0),
    .data_in_1(data_in_1),
    .data_in_2(data_in_2),
    .data_in_3(data_in_3),
    .data_out_valid(data_out_valid),
    .data_out(data_out)
  );


endmodule
