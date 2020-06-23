module reg_tree_4_32_4 #
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
  output [DATA_WIDTH-1:0] out_31
);

  reg [DATA_WIDTH-1:0] r [0:171-1];
  integer i;

  initial begin
    for(i=0; i<171; i=i+1) begin
      r[i] = 0;
    end
  end



  always @(posedge clk) begin
    r[0] <= in;
    r[1] <= r[0];
    r[2] <= r[0];
    r[3] <= r[1];
    r[4] <= r[1];
    r[5] <= r[1];
    r[6] <= r[1];
    r[7] <= r[2];
    r[8] <= r[2];
    r[9] <= r[2];
    r[10] <= r[2];
    r[11] <= r[3];
    r[12] <= r[3];
    r[13] <= r[3];
    r[14] <= r[3];
    r[15] <= r[4];
    r[16] <= r[4];
    r[17] <= r[4];
    r[18] <= r[4];
    r[19] <= r[5];
    r[20] <= r[5];
    r[21] <= r[5];
    r[22] <= r[5];
    r[23] <= r[6];
    r[24] <= r[6];
    r[25] <= r[6];
    r[26] <= r[6];
    r[27] <= r[7];
    r[28] <= r[7];
    r[29] <= r[7];
    r[30] <= r[7];
    r[31] <= r[8];
    r[32] <= r[8];
    r[33] <= r[8];
    r[34] <= r[8];
    r[35] <= r[9];
    r[36] <= r[9];
    r[37] <= r[9];
    r[38] <= r[9];
    r[39] <= r[10];
    r[40] <= r[10];
    r[41] <= r[10];
    r[42] <= r[10];
    r[43] <= r[11];
    r[44] <= r[12];
    r[45] <= r[13];
    r[46] <= r[14];
    r[47] <= r[15];
    r[48] <= r[16];
    r[49] <= r[17];
    r[50] <= r[18];
    r[51] <= r[19];
    r[52] <= r[20];
    r[53] <= r[21];
    r[54] <= r[22];
    r[55] <= r[23];
    r[56] <= r[24];
    r[57] <= r[25];
    r[58] <= r[26];
    r[59] <= r[27];
    r[60] <= r[28];
    r[61] <= r[29];
    r[62] <= r[30];
    r[63] <= r[31];
    r[64] <= r[32];
    r[65] <= r[33];
    r[66] <= r[34];
    r[67] <= r[35];
    r[68] <= r[36];
    r[69] <= r[37];
    r[70] <= r[38];
    r[71] <= r[39];
    r[72] <= r[40];
    r[73] <= r[41];
    r[74] <= r[42];
    r[75] <= r[43];
    r[76] <= r[44];
    r[77] <= r[45];
    r[78] <= r[46];
    r[79] <= r[47];
    r[80] <= r[48];
    r[81] <= r[49];
    r[82] <= r[50];
    r[83] <= r[51];
    r[84] <= r[52];
    r[85] <= r[53];
    r[86] <= r[54];
    r[87] <= r[55];
    r[88] <= r[56];
    r[89] <= r[57];
    r[90] <= r[58];
    r[91] <= r[59];
    r[92] <= r[60];
    r[93] <= r[61];
    r[94] <= r[62];
    r[95] <= r[63];
    r[96] <= r[64];
    r[97] <= r[65];
    r[98] <= r[66];
    r[99] <= r[67];
    r[100] <= r[68];
    r[101] <= r[69];
    r[102] <= r[70];
    r[103] <= r[71];
    r[104] <= r[72];
    r[105] <= r[73];
    r[106] <= r[74];
    r[107] <= r[75];
    r[108] <= r[76];
    r[109] <= r[77];
    r[110] <= r[78];
    r[111] <= r[79];
    r[112] <= r[80];
    r[113] <= r[81];
    r[114] <= r[82];
    r[115] <= r[83];
    r[116] <= r[84];
    r[117] <= r[85];
    r[118] <= r[86];
    r[119] <= r[87];
    r[120] <= r[88];
    r[121] <= r[89];
    r[122] <= r[90];
    r[123] <= r[91];
    r[124] <= r[92];
    r[125] <= r[93];
    r[126] <= r[94];
    r[127] <= r[95];
    r[128] <= r[96];
    r[129] <= r[97];
    r[130] <= r[98];
    r[131] <= r[99];
    r[132] <= r[100];
    r[133] <= r[101];
    r[134] <= r[102];
    r[135] <= r[103];
    r[136] <= r[104];
    r[137] <= r[105];
    r[138] <= r[106];
    r[139] <= r[107];
    r[140] <= r[108];
    r[141] <= r[109];
    r[142] <= r[110];
    r[143] <= r[111];
    r[144] <= r[112];
    r[145] <= r[113];
    r[146] <= r[114];
    r[147] <= r[115];
    r[148] <= r[116];
    r[149] <= r[117];
    r[150] <= r[118];
    r[151] <= r[119];
    r[152] <= r[120];
    r[153] <= r[121];
    r[154] <= r[122];
    r[155] <= r[123];
    r[156] <= r[124];
    r[157] <= r[125];
    r[158] <= r[126];
    r[159] <= r[127];
    r[160] <= r[128];
    r[161] <= r[129];
    r[162] <= r[130];
    r[163] <= r[131];
    r[164] <= r[132];
    r[165] <= r[133];
    r[166] <= r[134];
    r[167] <= r[135];
    r[168] <= r[136];
    r[169] <= r[137];
    r[170] <= r[138];

  end

  assign out_0 = r[139];
  assign out_1 = r[140];
  assign out_2 = r[141];
  assign out_3 = r[142];
  assign out_4 = r[143];
  assign out_5 = r[144];
  assign out_6 = r[145];
  assign out_7 = r[146];
  assign out_8 = r[147];
  assign out_9 = r[148];
  assign out_10 = r[149];
  assign out_11 = r[150];
  assign out_12 = r[151];
  assign out_13 = r[152];
  assign out_14 = r[153];
  assign out_15 = r[154];
  assign out_16 = r[155];
  assign out_17 = r[156];
  assign out_18 = r[157];
  assign out_19 = r[158];
  assign out_20 = r[159];
  assign out_21 = r[160];
  assign out_22 = r[161];
  assign out_23 = r[162];
  assign out_24 = r[163];
  assign out_25 = r[164];
  assign out_26 = r[165];
  assign out_27 = r[166];
  assign out_28 = r[167];
  assign out_29 = r[168];
  assign out_30 = r[169];
  assign out_31 = r[170];

endmodule