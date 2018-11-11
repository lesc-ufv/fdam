module cgra_alu_out_decode
(
  input [2-1:0] sel,
  output [3-1:0] out
);

  assign out[0] = ~sel[1] & ~sel[0];
  assign out[1] = ~sel[1] & sel[0];
  assign out[2] = sel[1] & ~sel[0];

endmodule