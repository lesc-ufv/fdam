module cgra0_inst_decode
(
  input [16-1:0] instruction,
  output [4-1:0] alu_op,
  output [1-1:0] mux_a,
  output [1-1:0] mux_b,
  output [1-1:0] rf_re,
  output [1-1:0] rf_we,
  output [1-1:0] fifo_re,
  output [1-1:0] fifo_we,
  output [1-1:0] out_a_en,
  output [1-1:0] out_b_en,
  output [4-1:0] rf_raddr,
  output [4-1:0] rf_waddr,
  output [1-1:0] branch_in_en,
  output [1-1:0] branch_out_en
);

  wire [4-1:0] control;
  assign control = instruction[7:4];
  assign alu_op = instruction[3:0];
  assign rf_raddr = instruction[11:8];
  assign rf_waddr = instruction[15:12];
  assign mux_a = control[1];
  assign fifo_re = control[1];
  assign mux_b = ~control[1] & control[0] | control[3] & control[1] & ~control[0];
  assign rf_re = ~control[1] & control[0] | control[3] & control[1] & ~control[0];
  assign out_a_en = ~control[3] & ~control[2];
  assign out_b_en = ~control[3] & control[2];
  assign rf_we = control[3] & ~control[2];
  assign fifo_we = control[3] & control[2];
  assign branch_in_en = alu_op[3] & alu_op[2] & ~alu_op[1];
  assign branch_out_en = alu_op[3] & alu_op[2] & alu_op[1] & alu_op[0];

endmodule