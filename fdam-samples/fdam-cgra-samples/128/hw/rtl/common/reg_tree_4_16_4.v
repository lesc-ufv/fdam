module reg_tree_4_16_4 #
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
  output [DATA_WIDTH-1:0] out_7,
  output [DATA_WIDTH-1:0] out_8,
  output [DATA_WIDTH-1:0] out_9,
  output [DATA_WIDTH-1:0] out_10,
  output [DATA_WIDTH-1:0] out_11,
  output [DATA_WIDTH-1:0] out_12,
  output [DATA_WIDTH-1:0] out_13,
  output [DATA_WIDTH-1:0] out_14,
  output [DATA_WIDTH-1:0] out_15
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
  reg [DATA_WIDTH-1:0] r27;
  reg [DATA_WIDTH-1:0] r28;
  reg [DATA_WIDTH-1:0] r29;
  reg [DATA_WIDTH-1:0] r30;
  reg [DATA_WIDTH-1:0] r31;
  reg [DATA_WIDTH-1:0] r32;
  reg [DATA_WIDTH-1:0] r33;
  reg [DATA_WIDTH-1:0] r34;
  reg [DATA_WIDTH-1:0] r35;
  reg [DATA_WIDTH-1:0] r36;
  reg [DATA_WIDTH-1:0] r37;
  reg [DATA_WIDTH-1:0] r38;
  reg [DATA_WIDTH-1:0] r39;
  reg [DATA_WIDTH-1:0] r40;
  reg [DATA_WIDTH-1:0] r41;
  reg [DATA_WIDTH-1:0] r42;
  reg [DATA_WIDTH-1:0] r43;
  reg [DATA_WIDTH-1:0] r44;
  reg [DATA_WIDTH-1:0] r45;
  reg [DATA_WIDTH-1:0] r46;
  reg [DATA_WIDTH-1:0] r47;
  reg [DATA_WIDTH-1:0] r48;
  reg [DATA_WIDTH-1:0] r49;
  reg [DATA_WIDTH-1:0] r50;
  reg [DATA_WIDTH-1:0] r51;
  reg [DATA_WIDTH-1:0] r52;
  reg [DATA_WIDTH-1:0] r53;
  reg [DATA_WIDTH-1:0] r54;
  reg [DATA_WIDTH-1:0] r55;
  reg [DATA_WIDTH-1:0] r56;
  reg [DATA_WIDTH-1:0] r57;
  reg [DATA_WIDTH-1:0] r58;
  reg [DATA_WIDTH-1:0] r59;
  reg [DATA_WIDTH-1:0] r60;
  reg [DATA_WIDTH-1:0] r61;
  reg [DATA_WIDTH-1:0] r62;
  reg [DATA_WIDTH-1:0] r63;
  reg [DATA_WIDTH-1:0] r64;
  reg [DATA_WIDTH-1:0] r65;
  reg [DATA_WIDTH-1:0] r66;
  reg [DATA_WIDTH-1:0] r67;
  reg [DATA_WIDTH-1:0] r68;
  reg [DATA_WIDTH-1:0] r69;
  reg [DATA_WIDTH-1:0] r70;
  reg [DATA_WIDTH-1:0] r71;
  reg [DATA_WIDTH-1:0] r72;
  reg [DATA_WIDTH-1:0] r73;
  reg [DATA_WIDTH-1:0] r74;
  reg [DATA_WIDTH-1:0] r75;
  reg [DATA_WIDTH-1:0] r76;
  reg [DATA_WIDTH-1:0] r77;
  reg [DATA_WIDTH-1:0] r78;
  reg [DATA_WIDTH-1:0] r79;
  reg [DATA_WIDTH-1:0] r80;
  reg [DATA_WIDTH-1:0] r81;
  reg [DATA_WIDTH-1:0] r82;
  reg [DATA_WIDTH-1:0] r83;
  reg [DATA_WIDTH-1:0] r84;

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
    r27 = 0;
    r28 = 0;
    r29 = 0;
    r30 = 0;
    r31 = 0;
    r32 = 0;
    r33 = 0;
    r34 = 0;
    r35 = 0;
    r36 = 0;
    r37 = 0;
    r38 = 0;
    r39 = 0;
    r40 = 0;
    r41 = 0;
    r42 = 0;
    r43 = 0;
    r44 = 0;
    r45 = 0;
    r46 = 0;
    r47 = 0;
    r48 = 0;
    r49 = 0;
    r50 = 0;
    r51 = 0;
    r52 = 0;
    r53 = 0;
    r54 = 0;
    r55 = 0;
    r56 = 0;
    r57 = 0;
    r58 = 0;
    r59 = 0;
    r60 = 0;
    r61 = 0;
    r62 = 0;
    r63 = 0;
    r64 = 0;
    r65 = 0;
    r66 = 0;
    r67 = 0;
    r68 = 0;
    r69 = 0;
    r70 = 0;
    r71 = 0;
    r72 = 0;
    r73 = 0;
    r74 = 0;
    r75 = 0;
    r76 = 0;
    r77 = 0;
    r78 = 0;
    r79 = 0;
    r80 = 0;
    r81 = 0;
    r82 = 0;
    r83 = 0;
    r84 = 0;

  end



  always @(posedge clk) begin
    r0 <= in;
    r1 <= r0;
    r2 <= r0;
    r3 <= r0;
    r4 <= r0;
    r5 <= r1;
    r6 <= r1;
    r7 <= r1;
    r8 <= r1;
    r9 <= r2;
    r10 <= r2;
    r11 <= r2;
    r12 <= r2;
    r13 <= r3;
    r14 <= r3;
    r15 <= r3;
    r16 <= r3;
    r17 <= r4;
    r18 <= r4;
    r19 <= r4;
    r20 <= r4;
    r21 <= r5;
    r22 <= r6;
    r23 <= r7;
    r24 <= r8;
    r25 <= r9;
    r26 <= r10;
    r27 <= r11;
    r28 <= r12;
    r29 <= r13;
    r30 <= r14;
    r31 <= r15;
    r32 <= r16;
    r33 <= r17;
    r34 <= r18;
    r35 <= r19;
    r36 <= r20;
    r37 <= r21;
    r38 <= r22;
    r39 <= r23;
    r40 <= r24;
    r41 <= r25;
    r42 <= r26;
    r43 <= r27;
    r44 <= r28;
    r45 <= r29;
    r46 <= r30;
    r47 <= r31;
    r48 <= r32;
    r49 <= r33;
    r50 <= r34;
    r51 <= r35;
    r52 <= r36;
    r53 <= r37;
    r54 <= r38;
    r55 <= r39;
    r56 <= r40;
    r57 <= r41;
    r58 <= r42;
    r59 <= r43;
    r60 <= r44;
    r61 <= r45;
    r62 <= r46;
    r63 <= r47;
    r64 <= r48;
    r65 <= r49;
    r66 <= r50;
    r67 <= r51;
    r68 <= r52;
    r69 <= r53;
    r70 <= r54;
    r71 <= r55;
    r72 <= r56;
    r73 <= r57;
    r74 <= r58;
    r75 <= r59;
    r76 <= r60;
    r77 <= r61;
    r78 <= r62;
    r79 <= r63;
    r80 <= r64;
    r81 <= r65;
    r82 <= r66;
    r83 <= r67;
    r84 <= r68;

  end

  assign out_0 = r69;
  assign out_1 = r70;
  assign out_2 = r71;
  assign out_3 = r72;
  assign out_4 = r73;
  assign out_5 = r74;
  assign out_6 = r75;
  assign out_7 = r76;
  assign out_8 = r77;
  assign out_9 = r78;
  assign out_10 = r79;
  assign out_11 = r80;
  assign out_12 = r81;
  assign out_13 = r82;
  assign out_14 = r83;
  assign out_15 = r84;

endmodule