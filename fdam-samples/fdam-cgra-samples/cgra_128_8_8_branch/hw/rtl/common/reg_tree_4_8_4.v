module reg_tree_4_8_4 #
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

  reg [DATA_WIDTH-1:0] r [0:43-1];
  integer i;

  initial begin
    for(i=0; i<43; i=i+1) begin
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
    r[12] <= r[4];
    r[13] <= r[5];
    r[14] <= r[6];
    r[15] <= r[7];
    r[16] <= r[8];
    r[17] <= r[9];
    r[18] <= r[10];
    r[19] <= r[11];
    r[20] <= r[12];
    r[21] <= r[13];
    r[22] <= r[14];
    r[23] <= r[15];
    r[24] <= r[16];
    r[25] <= r[17];
    r[26] <= r[18];
    r[27] <= r[19];
    r[28] <= r[20];
    r[29] <= r[21];
    r[30] <= r[22];
    r[31] <= r[23];
    r[32] <= r[24];
    r[33] <= r[25];
    r[34] <= r[26];
    r[35] <= r[27];
    r[36] <= r[28];
    r[37] <= r[29];
    r[38] <= r[30];
    r[39] <= r[31];
    r[40] <= r[32];
    r[41] <= r[33];
    r[42] <= r[34];

  end

  assign out_0 = r[35];
  assign out_1 = r[36];
  assign out_2 = r[37];
  assign out_3 = r[38];
  assign out_4 = r[39];
  assign out_5 = r[40];
  assign out_6 = r[41];
  assign out_7 = r[42];

endmodule