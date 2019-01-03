module cgra0_alu #
(
  parameter DATA_WIDTH = 16
)
(
  input clk,
  input [4-1:0] op,
  input branch_in,
  output branch_out,
  input [DATA_WIDTH-1:0] ina,
  input [DATA_WIDTH-1:0] inb,
  output [DATA_WIDTH-1:0] out
);

  localparam PASS_A = 0;
  localparam PASS_B = 1;
  localparam ADD = 2;
  localparam SUB = 3;
  localparam MULT = 4;
  localparam DIV = 5;
  localparam XOR = 6;
  localparam AND = 7;
  localparam OR = 8;
  localparam NOT = 9;
  localparam SHL = 10;
  localparam SHR = 11;
  localparam BEQ = 12;
  localparam BNE = 13;
  localparam SLT = 14;
  localparam MERGE = 15;
  reg [DATA_WIDTH-1:0] result [0:2**4-1];
  reg [4-1:0] op_reg;
  assign out = result[op_reg];
  assign branch_out = out[0];

  always @(posedge clk) begin
    op_reg <= op;
    result[PASS_A] <= ina;
    result[PASS_B] <= inb;
    result[ADD] <= ina + inb;
    result[SUB] <= ina - inb;
    result[MULT] <= ina * inb;
    result[DIV] <= ina - inb;
    result[AND] <= ina & inb;
    result[OR] <= ina | inb;
    result[NOT] <= ~ina;
    result[XOR] <= ina ^ inb;
    result[SHL] <= ina << inb;
    result[SHR] <= ina >> inb;
    result[SLT] <= (ina < inb)? 1 : 0;
    result[BEQ] <= ina == inb;
    result[BNE] <= ina != inb;
    result[MERGE] <= (branch_in)? ina : inb;
  end

endmodule