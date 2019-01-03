module select_top_8 #
(
  parameter DATA_WIDTH = 32
)
(
  input clk,
  input rst,
  input [8-1:0] data_in_valid,
  input [DATA_WIDTH-1:0] data_in_0,
  input [DATA_WIDTH-1:0] data_in_1,
  input [DATA_WIDTH-1:0] data_in_2,
  input [DATA_WIDTH-1:0] data_in_3,
  input [DATA_WIDTH-1:0] data_in_4,
  input [DATA_WIDTH-1:0] data_in_5,
  input [DATA_WIDTH-1:0] data_in_6,
  input [DATA_WIDTH-1:0] data_in_7,
  output [DATA_WIDTH-1:0] data_out,
  output data_out_valid
);

  wire [2-1:0] data_out_valid_wires_0;
  wire [DATA_WIDTH-1:0] data_out_wire_0 [0:2-1];

  select_4
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  sel_0_0
  (
    .clk(clk),
    .rst(rst),
    .data_in_valid(data_in_valid[3:0]),
    .data_in_0(data_in_0),
    .data_in_1(data_in_1),
    .data_in_2(data_in_2),
    .data_in_3(data_in_3),
    .data_out_valid(data_out_valid_wires_0[0]),
    .data_out(data_out_wire_0[0])
  );


  select_4
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  sel_0_1
  (
    .clk(clk),
    .rst(rst),
    .data_in_valid(data_in_valid[7:4]),
    .data_in_0(data_in_4),
    .data_in_1(data_in_5),
    .data_in_2(data_in_6),
    .data_in_3(data_in_7),
    .data_out_valid(data_out_valid_wires_0[1]),
    .data_out(data_out_wire_0[1])
  );


  select_2
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  sel_1_0
  (
    .clk(clk),
    .rst(rst),
    .data_in_valid(data_out_valid_wires_0),
    .data_in_0(data_out_wire_0[0]),
    .data_in_1(data_out_wire_0[1]),
    .data_out_valid(data_out_valid),
    .data_out(data_out)
  );

endmodule