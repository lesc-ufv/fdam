module cgra_mux4x1 #
(
  parameter WIDTH = 16
)
(
  input [2-1:0] sel,
  input [WIDTH-1:0] in0,
  input [WIDTH-1:0] in1,
  input [WIDTH-1:0] in2,
  input [WIDTH-1:0] in3,
  output [WIDTH-1:0] out
);

  wire [WIDTH-1:0] ins [0:4-1];
  assign ins[0] = in0;
  assign ins[1] = in1;
  assign ins[2] = in2;
  assign ins[3] = in3;
  assign out = ins[sel];

endmodule