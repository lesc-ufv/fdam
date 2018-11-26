module reg_tree_4_8_2 #
(
  parameter DATA_WIDTH = 16
)
(
  input clk,
  input [DATA_WIDTH-1:0] in,
  output [DATA_WIDTH-1:0] out_0,
  output [DATA_WIDTH-1:0] out_1,
  output [DATA_WIDTH-1:0] out_2,
  output [DATA_WIDTH-1:0] out_3,
  output [DATA_WIDTH-1:0] out_4,
  output [DATA_WIDTH-1:0] out_5,
  output [DATA_WIDTH-1:0] out_6,
  output [DATA_WIDTH-1:0] out_7
);

  reg [DATA_WIDTH-1:0] r0;
  reg [DATA_WIDTH-1:0] r1;
  reg [DATA_WIDTH-1:0] r2;
  reg [DATA_WIDTH-1:0] r3;
  reg [DATA_WIDTH-1:0] r4;
  reg [DATA_WIDTH-1:0] r5;
  reg [DATA_WIDTH-1:0] r6;
  reg [DATA_WIDTH-1:0] r7;
  reg [DATA_WIDTH-1:0] r8;
  reg [DATA_WIDTH-1:0] r9;
  reg [DATA_WIDTH-1:0] r10;
  reg [DATA_WIDTH-1:0] r11;
  reg [DATA_WIDTH-1:0] r12;
  reg [DATA_WIDTH-1:0] r13;
  reg [DATA_WIDTH-1:0] r14;
  reg [DATA_WIDTH-1:0] r15;
  reg [DATA_WIDTH-1:0] r16;
  reg [DATA_WIDTH-1:0] r17;
  reg [DATA_WIDTH-1:0] r18;
  reg [DATA_WIDTH-1:0] r19;
  reg [DATA_WIDTH-1:0] r20;
  reg [DATA_WIDTH-1:0] r21;
  reg [DATA_WIDTH-1:0] r22;
  reg [DATA_WIDTH-1:0] r23;
  reg [DATA_WIDTH-1:0] r24;
  reg [DATA_WIDTH-1:0] r25;
  reg [DATA_WIDTH-1:0] r26;

  initial begin
    r0 = 0;
    r1 = 0;
    r2 = 0;
    r3 = 0;
    r4 = 0;
    r5 = 0;
    r6 = 0;
    r7 = 0;
    r8 = 0;
    r9 = 0;
    r10 = 0;
    r11 = 0;
    r12 = 0;
    r13 = 0;
    r14 = 0;
    r15 = 0;
    r16 = 0;
    r17 = 0;
    r18 = 0;
    r19 = 0;
    r20 = 0;
    r21 = 0;
    r22 = 0;
    r23 = 0;
    r24 = 0;
    r25 = 0;
    r26 = 0;

  end



  always @(posedge clk) begin
    r0 <= in;
    r1 <= r0;
    r2 <= r0;
    r3 <= r1;
    r4 <= r1;
    r5 <= r1;
    r6 <= r1;
    r7 <= r2;
    r8 <= r2;
    r9 <= r2;
    r10 <= r2;
    r11 <= r3;
    r12 <= r4;
    r13 <= r5;
    r14 <= r6;
    r15 <= r7;
    r16 <= r8;
    r17 <= r9;
    r18 <= r10;
    r19 <= r11;
    r20 <= r12;
    r21 <= r13;
    r22 <= r14;
    r23 <= r15;
    r24 <= r16;
    r25 <= r17;
    r26 <= r18;

  end

  assign out_0 = r19;
  assign out_1 = r20;
  assign out_2 = r21;
  assign out_3 = r22;
  assign out_4 = r23;
  assign out_5 = r24;
  assign out_6 = r25;
  assign out_7 = r26;

endmodule