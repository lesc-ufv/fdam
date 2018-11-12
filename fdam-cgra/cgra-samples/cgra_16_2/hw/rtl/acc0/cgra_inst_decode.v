
module cgra_inst_decode
(
  input [16-1:0] instruction,
  output [4-1:0] alu_op,
  output [2-1:0] alu_a,
  output [2-1:0] alu_b,
  output [2-1:0] alu_out,
  output [3-1:0] rf_raddr,
  output [3-1:0] rf_waddr
);

  assign alu_op = instruction[3:0];
  assign alu_a = instruction[5:4];
  assign alu_b = instruction[7:6];
  assign alu_out = instruction[9:8];
  assign rf_raddr = instruction[12:10];
  assign rf_waddr = instruction[15:13];

endmodule
