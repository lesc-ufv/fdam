module reg_tree_4_14_2 #
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
  output [DATA_WIDTH-1:0] out_13
);

  reg [DATA_WIDTH-1:0] r [0:47-1];
  integer i;

  initial begin
    for(i=0; i<47; i=i+1) begin
      r[i] = 0;
    end
  end



  always @(posedge clk) begin
    r[0] <= in;
    r[1] <= r[0];
    r[2] <= r[0];
    r[3] <= r[0];
    r[4] <= r[0];
    r[5] <= r[1];
    r[6] <= r[1];
    r[7] <= r[1];
    r[8] <= r[1];
    r[9] <= r[2];
    r[10] <= r[2];
    r[11] <= r[2];
    r[12] <= r[2];
    r[13] <= r[3];
    r[14] <= r[3];
    r[15] <= r[3];
    r[16] <= r[3];
    r[17] <= r[4];
    r[18] <= r[4];
    r[19] <= r[5];
    r[20] <= r[6];
    r[21] <= r[7];
    r[22] <= r[8];
    r[23] <= r[9];
    r[24] <= r[10];
    r[25] <= r[11];
    r[26] <= r[12];
    r[27] <= r[13];
    r[28] <= r[14];
    r[29] <= r[15];
    r[30] <= r[16];
    r[31] <= r[17];
    r[32] <= r[18];
    r[33] <= r[19];
    r[34] <= r[20];
    r[35] <= r[21];
    r[36] <= r[22];
    r[37] <= r[23];
    r[38] <= r[24];
    r[39] <= r[25];
    r[40] <= r[26];
    r[41] <= r[27];
    r[42] <= r[28];
    r[43] <= r[29];
    r[44] <= r[30];
    r[45] <= r[31];
    r[46] <= r[32];

  end

  assign out_0 = r[33];
  assign out_1 = r[34];
  assign out_2 = r[35];
  assign out_3 = r[36];
  assign out_4 = r[37];
  assign out_5 = r[38];
  assign out_6 = r[39];
  assign out_7 = r[40];
  assign out_8 = r[41];
  assign out_9 = r[42];
  assign out_10 = r[43];
  assign out_11 = r[44];
  assign out_12 = r[45];
  assign out_13 = r[46];

endmodule