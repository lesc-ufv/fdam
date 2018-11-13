
module cgra0_alu_out_decode_io
(
  input [2-1:0] sel,
  output [4-1:0] out
);

  assign out[0] = ~sel[1] & ~sel[0];
  assign out[1] = ~sel[1] & sel[0];
  assign out[2] = sel[1] & ~sel[0];
  assign out[3] = sel[1] & sel[0];

endmodule
