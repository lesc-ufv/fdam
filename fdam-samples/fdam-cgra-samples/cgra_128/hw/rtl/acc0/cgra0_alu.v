module cgra0_alu #
(
  parameter DATA_WIDTH = 16
)
(
  input clk,
  input [4-1:0] op,
  input branch_in,
  output branch_out,
  input  [DATA_WIDTH-1:0] ina,
  input  [DATA_WIDTH-1:0] inb,
  output [DATA_WIDTH-1:0] out
);

  localparam PASS_A = 0;
  localparam PASS_B = 1;
  localparam MIN = 2;
  localparam MAX = 3;
  localparam BEQ = 0;
  localparam BNE = 1;
  localparam SLT = 2;
  localparam SGT = 3;
  localparam ADD = 4;
  localparam SUB = 5;
  localparam MULT = 6;
  localparam XOR = 7;
  localparam AND = 8;
  localparam OR = 9;
  localparam NOT = 10;
  localparam SHL = 11;
  localparam SHR = 12;
  localparam MUX = 13;
  localparam ABS = 14;
  reg [DATA_WIDTH-1:0] result [0:2**4-1];
  reg [4-1:0] result_branch;
  reg [4-1:0] op_reg;
  assign out = result[op_reg];
  assign branch_out = result_branch[op_reg[1:0]];

  always @(posedge clk) begin
    op_reg <= op;
    result[PASS_A] <= ina;
    result[PASS_B] <= inb;
    result[ADD] <= ina + inb;
    result[SUB] <= ina - inb;
    result[MULT] <= ina * inb;
    result[AND] <= ina & inb;
    result[OR] <= ina | inb;
    result[NOT] <= ~ina;
    result[XOR] <= ina ^ inb;
    result[SHL] <= ina << inb;
    result[SHR] <= ina >> inb;
    result[MIN] <= (ina < inb)? ina : inb;
    result[MAX] <= (ina > inb)? ina : inb;
    result[MUX] <= (branch_in)? ina : inb;
    result[ABS] <= (ina[DATA_WIDTH - 1])? -ina : ina;
  end


  always @(posedge clk) begin
    result_branch[BEQ] <= ina == inb;
    result_branch[BNE] <= ina != inb;
    result_branch[SLT] <= (ina < inb)? 1'b1 : 1'b0;
    result_branch[SGT] <= (ina > inb)? 1'b1 : 1'b0;
  end

endmodule
