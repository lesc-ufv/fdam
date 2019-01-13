module reg_tree_4_18_2 #
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
  output [DATA_WIDTH-1:0] out_17
);

  reg [DATA_WIDTH-1:0] r [0:62-1];
  integer i;

  initial begin
    for(i=0; i<62; i=i+1) begin
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
    r[8] <= r[3];
    r[9] <= r[3];
    r[10] <= r[3];
    r[11] <= r[3];
    r[12] <= r[4];
    r[13] <= r[4];
    r[14] <= r[4];
    r[15] <= r[4];
    r[16] <= r[5];
    r[17] <= r[5];
    r[18] <= r[5];
    r[19] <= r[5];
    r[20] <= r[6];
    r[21] <= r[6];
    r[22] <= r[6];
    r[23] <= r[6];
    r[24] <= r[7];
    r[25] <= r[7];
    r[26] <= r[8];
    r[27] <= r[9];
    r[28] <= r[10];
    r[29] <= r[11];
    r[30] <= r[12];
    r[31] <= r[13];
    r[32] <= r[14];
    r[33] <= r[15];
    r[34] <= r[16];
    r[35] <= r[17];
    r[36] <= r[18];
    r[37] <= r[19];
    r[38] <= r[20];
    r[39] <= r[21];
    r[40] <= r[22];
    r[41] <= r[23];
    r[42] <= r[24];
    r[43] <= r[25];
    r[44] <= r[26];
    r[45] <= r[27];
    r[46] <= r[28];
    r[47] <= r[29];
    r[48] <= r[30];
    r[49] <= r[31];
    r[50] <= r[32];
    r[51] <= r[33];
    r[52] <= r[34];
    r[53] <= r[35];
    r[54] <= r[36];
    r[55] <= r[37];
    r[56] <= r[38];
    r[57] <= r[39];
    r[58] <= r[40];
    r[59] <= r[41];
    r[60] <= r[42];
    r[61] <= r[43];

  end

  assign out_0 = r[44];
  assign out_1 = r[45];
  assign out_2 = r[46];
  assign out_3 = r[47];
  assign out_4 = r[48];
  assign out_5 = r[49];
  assign out_6 = r[50];
  assign out_7 = r[51];
  assign out_8 = r[52];
  assign out_9 = r[53];
  assign out_10 = r[54];
  assign out_11 = r[55];
  assign out_12 = r[56];
  assign out_13 = r[57];
  assign out_14 = r[58];
  assign out_15 = r[59];
  assign out_16 = r[60];
  assign out_17 = r[61];

endmodule