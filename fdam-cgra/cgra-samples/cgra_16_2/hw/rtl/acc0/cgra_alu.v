module cgra_alu #
(
  parameter DATA_WIDTH = 16
)
(
  input clk,
  input [4-1:0] op,
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
  localparam MOD = 6;
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

  assign out = result[op];

  always @(posedge clk) begin
    result[PASS_A] <= ina;
    result[PASS_B] <= inb;
    result[ADD] <= ina + inb;
    result[SUB] <= ina - inb;
    result[MULT] <= ina * inb;
    result[DIV] <= 0;
    result[MOD] <= 0;
    result[AND] <= ina & inb;
    result[OR] <= ina | inb;
    result[NOT] <= ~ina;
    result[SHL] <= ina << inb;
    result[SHR] <= ina >> inb;
    result[SLT] <= (ina < inb)? 1 : 0;
    result[BEQ] <= 0;
    result[BNE] <= 0;
    result[MERGE] <= 0;
  end

  integer i;

  initial begin
    for(i=0; i<2**4; i=i+1) begin
      result[i] = 0;
    end
  end

endmodule