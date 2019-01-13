module mux8x1 #
(
  parameter WIDTH = 16
)
(
  input [3-1:0] sel,
  input [WIDTH-1:0] in0,
  input [WIDTH-1:0] in1,
  input [WIDTH-1:0] in2,
  input [WIDTH-1:0] in3,
  input [WIDTH-1:0] in4,
  input [WIDTH-1:0] in5,
  input [WIDTH-1:0] in6,
  input [WIDTH-1:0] in7,
  output [WIDTH-1:0] out
);

  wire [WIDTH-1:0] ins [0:8-1];
  assign ins[0] = in0;
  assign ins[1] = in1;
  assign ins[2] = in2;
  assign ins[3] = in3;
  assign ins[4] = in4;
  assign ins[5] = in5;
  assign ins[6] = in6;
  assign ins[7] = in7;
  assign out = ins[sel];

endmodule