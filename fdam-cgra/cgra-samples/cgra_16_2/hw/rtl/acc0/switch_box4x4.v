
module switch_box4x4 #
(
  parameter WIDTH = 16
)
(
  input [8-1:0] sel,
  input [WIDTH-1:0] in0,
  input [WIDTH-1:0] in1,
  input [WIDTH-1:0] in2,
  input [WIDTH-1:0] in3,
  output [WIDTH-1:0] out0,
  output [WIDTH-1:0] out1,
  output [WIDTH-1:0] out2,
  output [WIDTH-1:0] out3
);


  mux4x1
  #(
    .WIDTH(WIDTH)
  )
  mux0
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .sel(sel[1:0]),
    .out(out0)
  );


  mux4x1
  #(
    .WIDTH(WIDTH)
  )
  mux1
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .sel(sel[3:2]),
    .out(out1)
  );


  mux4x1
  #(
    .WIDTH(WIDTH)
  )
  mux2
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .sel(sel[5:4]),
    .out(out2)
  );


  mux4x1
  #(
    .WIDTH(WIDTH)
  )
  mux3
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .sel(sel[7:6]),
    .out(out3)
  );


endmodule
