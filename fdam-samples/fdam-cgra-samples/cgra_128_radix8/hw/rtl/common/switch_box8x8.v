module switch_box8x8 #
(
  parameter WIDTH = 16
)
(
  input [24-1:0] sel,
  input [WIDTH-1:0] in0,
  input [WIDTH-1:0] in1,
  input [WIDTH-1:0] in2,
  input [WIDTH-1:0] in3,
  input [WIDTH-1:0] in4,
  input [WIDTH-1:0] in5,
  input [WIDTH-1:0] in6,
  input [WIDTH-1:0] in7,
  output [WIDTH-1:0] out0,
  output [WIDTH-1:0] out1,
  output [WIDTH-1:0] out2,
  output [WIDTH-1:0] out3,
  output [WIDTH-1:0] out4,
  output [WIDTH-1:0] out5,
  output [WIDTH-1:0] out6,
  output [WIDTH-1:0] out7
);


  mux8x1
  #(
    .WIDTH(WIDTH)
  )
  mux0
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel[2:0]),
    .out(out0)
  );


  mux8x1
  #(
    .WIDTH(WIDTH)
  )
  mux1
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel[5:3]),
    .out(out1)
  );


  mux8x1
  #(
    .WIDTH(WIDTH)
  )
  mux2
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel[8:6]),
    .out(out2)
  );


  mux8x1
  #(
    .WIDTH(WIDTH)
  )
  mux3
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel[11:9]),
    .out(out3)
  );


  mux8x1
  #(
    .WIDTH(WIDTH)
  )
  mux4
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel[14:12]),
    .out(out4)
  );


  mux8x1
  #(
    .WIDTH(WIDTH)
  )
  mux5
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel[17:15]),
    .out(out5)
  );


  mux8x1
  #(
    .WIDTH(WIDTH)
  )
  mux6
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel[20:18]),
    .out(out6)
  );


  mux8x1
  #(
    .WIDTH(WIDTH)
  )
  mux7
  (
    .in0(in0),
    .in1(in1),
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel[23:21]),
    .out(out7)
  );

endmodule