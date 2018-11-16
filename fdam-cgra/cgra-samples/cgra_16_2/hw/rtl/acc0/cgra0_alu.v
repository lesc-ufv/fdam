
module cgra0_alu #
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

  wire [DATA_WIDTH-1:0] in_a_rep [0:15-1];
  wire [DATA_WIDTH-1:0] in_b_rep [0:14-1];

  reg_tree_4_15_1
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  ina_regs
  (
    .clk(clk),
    .in(ina),
    .out_0(in_a_rep[0]),
    .out_1(in_a_rep[1]),
    .out_2(in_a_rep[2]),
    .out_3(in_a_rep[3]),
    .out_4(in_a_rep[4]),
    .out_5(in_a_rep[5]),
    .out_6(in_a_rep[6]),
    .out_7(in_a_rep[7]),
    .out_8(in_a_rep[8]),
    .out_9(in_a_rep[9]),
    .out_10(in_a_rep[10]),
    .out_11(in_a_rep[11]),
    .out_12(in_a_rep[12]),
    .out_13(in_a_rep[13]),
    .out_14(in_a_rep[14])
  );


  reg_tree_4_14_1
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  inb_regs
  (
    .clk(clk),
    .in(inb),
    .out_0(in_b_rep[0]),
    .out_1(in_b_rep[1]),
    .out_2(in_b_rep[2]),
    .out_3(in_b_rep[3]),
    .out_4(in_b_rep[4]),
    .out_5(in_b_rep[5]),
    .out_6(in_b_rep[6]),
    .out_7(in_b_rep[7]),
    .out_8(in_b_rep[8]),
    .out_9(in_b_rep[9]),
    .out_10(in_b_rep[10]),
    .out_11(in_b_rep[11]),
    .out_12(in_b_rep[12]),
    .out_13(in_b_rep[13])
  );

  reg [DATA_WIDTH-1:0] result [0:2**4-1];

  assign out = result[op];

  always @(posedge clk) begin
    result[PASS_A] <= in_a_rep[0];
    result[PASS_B] <= in_b_rep[0];
    result[ADD] <= in_a_rep[1] + in_b_rep[1];
    result[SUB] <= in_a_rep[2] - in_b_rep[2];
    result[MULT] <= in_a_rep[3] * in_b_rep[3];
    result[AND] <= in_a_rep[4] & in_b_rep[4];
    result[OR] <= in_a_rep[5] | in_b_rep[5];
    result[NOT] <= ~in_a_rep[6];
    result[SHL] <= in_a_rep[7] << in_b_rep[7];
    result[SHR] <= in_a_rep[8] >> in_b_rep[8];
    result[SLT] <= (in_a_rep[9] < in_b_rep[9])? 1 : 0;
    result[DIV] <= 0;
    result[MOD] <= 0;
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
