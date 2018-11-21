module reg_tree_4_34_2 #
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
  output [DATA_WIDTH-1:0] out_15,
  output [DATA_WIDTH-1:0] out_16,
  output [DATA_WIDTH-1:0] out_17,
  output [DATA_WIDTH-1:0] out_18,
  output [DATA_WIDTH-1:0] out_19,
  output [DATA_WIDTH-1:0] out_20,
  output [DATA_WIDTH-1:0] out_21,
  output [DATA_WIDTH-1:0] out_22,
  output [DATA_WIDTH-1:0] out_23,
  output [DATA_WIDTH-1:0] out_24,
  output [DATA_WIDTH-1:0] out_25,
  output [DATA_WIDTH-1:0] out_26,
  output [DATA_WIDTH-1:0] out_27,
  output [DATA_WIDTH-1:0] out_28,
  output [DATA_WIDTH-1:0] out_29,
  output [DATA_WIDTH-1:0] out_30,
  output [DATA_WIDTH-1:0] out_31,
  output [DATA_WIDTH-1:0] out_32,
  output [DATA_WIDTH-1:0] out_33
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
  reg [DATA_WIDTH-1:0] r85;
  reg [DATA_WIDTH-1:0] r86;
  reg [DATA_WIDTH-1:0] r87;
  reg [DATA_WIDTH-1:0] r88;
  reg [DATA_WIDTH-1:0] r89;
  reg [DATA_WIDTH-1:0] r90;
  reg [DATA_WIDTH-1:0] r91;
  reg [DATA_WIDTH-1:0] r92;
  reg [DATA_WIDTH-1:0] r93;
  reg [DATA_WIDTH-1:0] r94;
  reg [DATA_WIDTH-1:0] r95;
  reg [DATA_WIDTH-1:0] r96;
  reg [DATA_WIDTH-1:0] r97;
  reg [DATA_WIDTH-1:0] r98;
  reg [DATA_WIDTH-1:0] r99;
  reg [DATA_WIDTH-1:0] r100;
  reg [DATA_WIDTH-1:0] r101;
  reg [DATA_WIDTH-1:0] r102;
  reg [DATA_WIDTH-1:0] r103;
  reg [DATA_WIDTH-1:0] r104;
  reg [DATA_WIDTH-1:0] r105;
  reg [DATA_WIDTH-1:0] r106;
  reg [DATA_WIDTH-1:0] r107;
  reg [DATA_WIDTH-1:0] r108;
  reg [DATA_WIDTH-1:0] r109;
  reg [DATA_WIDTH-1:0] r110;
  reg [DATA_WIDTH-1:0] r111;
  reg [DATA_WIDTH-1:0] r112;
  reg [DATA_WIDTH-1:0] r113;
  reg [DATA_WIDTH-1:0] r114;

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
    r85 = 0;
    r86 = 0;
    r87 = 0;
    r88 = 0;
    r89 = 0;
    r90 = 0;
    r91 = 0;
    r92 = 0;
    r93 = 0;
    r94 = 0;
    r95 = 0;
    r96 = 0;
    r97 = 0;
    r98 = 0;
    r99 = 0;
    r100 = 0;
    r101 = 0;
    r102 = 0;
    r103 = 0;
    r104 = 0;
    r105 = 0;
    r106 = 0;
    r107 = 0;
    r108 = 0;
    r109 = 0;
    r110 = 0;
    r111 = 0;
    r112 = 0;
    r113 = 0;
    r114 = 0;

  end



  always @(posedge clk) begin
    r0 <= in;
    r1 <= r0;
    r2 <= r0;
    r3 <= r0;
    r4 <= r1;
    r5 <= r1;
    r6 <= r1;
    r7 <= r1;
    r8 <= r2;
    r9 <= r2;
    r10 <= r2;
    r11 <= r2;
    r12 <= r3;
    r13 <= r4;
    r14 <= r4;
    r15 <= r4;
    r16 <= r4;
    r17 <= r5;
    r18 <= r5;
    r19 <= r5;
    r20 <= r5;
    r21 <= r6;
    r22 <= r6;
    r23 <= r6;
    r24 <= r6;
    r25 <= r7;
    r26 <= r7;
    r27 <= r7;
    r28 <= r7;
    r29 <= r8;
    r30 <= r8;
    r31 <= r8;
    r32 <= r8;
    r33 <= r9;
    r34 <= r9;
    r35 <= r9;
    r36 <= r9;
    r37 <= r10;
    r38 <= r10;
    r39 <= r10;
    r40 <= r10;
    r41 <= r11;
    r42 <= r11;
    r43 <= r11;
    r44 <= r11;
    r45 <= r12;
    r46 <= r12;
    r47 <= r13;
    r48 <= r14;
    r49 <= r15;
    r50 <= r16;
    r51 <= r17;
    r52 <= r18;
    r53 <= r19;
    r54 <= r20;
    r55 <= r21;
    r56 <= r22;
    r57 <= r23;
    r58 <= r24;
    r59 <= r25;
    r60 <= r26;
    r61 <= r27;
    r62 <= r28;
    r63 <= r29;
    r64 <= r30;
    r65 <= r31;
    r66 <= r32;
    r67 <= r33;
    r68 <= r34;
    r69 <= r35;
    r70 <= r36;
    r71 <= r37;
    r72 <= r38;
    r73 <= r39;
    r74 <= r40;
    r75 <= r41;
    r76 <= r42;
    r77 <= r43;
    r78 <= r44;
    r79 <= r45;
    r80 <= r46;
    r81 <= r47;
    r82 <= r48;
    r83 <= r49;
    r84 <= r50;
    r85 <= r51;
    r86 <= r52;
    r87 <= r53;
    r88 <= r54;
    r89 <= r55;
    r90 <= r56;
    r91 <= r57;
    r92 <= r58;
    r93 <= r59;
    r94 <= r60;
    r95 <= r61;
    r96 <= r62;
    r97 <= r63;
    r98 <= r64;
    r99 <= r65;
    r100 <= r66;
    r101 <= r67;
    r102 <= r68;
    r103 <= r69;
    r104 <= r70;
    r105 <= r71;
    r106 <= r72;
    r107 <= r73;
    r108 <= r74;
    r109 <= r75;
    r110 <= r76;
    r111 <= r77;
    r112 <= r78;
    r113 <= r79;
    r114 <= r80;

  end

  assign out_0 = r81;
  assign out_1 = r82;
  assign out_2 = r83;
  assign out_3 = r84;
  assign out_4 = r85;
  assign out_5 = r86;
  assign out_6 = r87;
  assign out_7 = r88;
  assign out_8 = r89;
  assign out_9 = r90;
  assign out_10 = r91;
  assign out_11 = r92;
  assign out_12 = r93;
  assign out_13 = r94;
  assign out_14 = r95;
  assign out_15 = r96;
  assign out_16 = r97;
  assign out_17 = r98;
  assign out_18 = r99;
  assign out_19 = r100;
  assign out_20 = r101;
  assign out_21 = r102;
  assign out_22 = r103;
  assign out_23 = r104;
  assign out_24 = r105;
  assign out_25 = r106;
  assign out_26 = r107;
  assign out_27 = r108;
  assign out_28 = r109;
  assign out_29 = r110;
  assign out_30 = r111;
  assign out_31 = r112;
  assign out_32 = r113;
  assign out_33 = r114;

endmodule