module reg_tree_4_10_2 #
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
  output [DATA_WIDTH-1:0] out_9
);

  reg [DATA_WIDTH-1:0] r [0:34-1];
  integer i;

  initial begin
    for(i=0; i<34; i=i+1) begin
      r[i] = 0;
    end
  end



  always @(posedge clk) begin
    r[0] <= in;
    r[1] <= r[0];
    r[2] <= r[0];
    r[3] <= r[0];
    r[4] <= r[1];
    r[5] <= r[1];
    r[6] <= r[1];
    r[7] <= r[1];
    r[8] <= r[2];
    r[9] <= r[2];
    r[10] <= r[2];
    r[11] <= r[2];
    r[12] <= r[3];
    r[13] <= r[3];
    r[14] <= r[4];
    r[15] <= r[5];
    r[16] <= r[6];
    r[17] <= r[7];
    r[18] <= r[8];
    r[19] <= r[9];
    r[20] <= r[10];
    r[21] <= r[11];
    r[22] <= r[12];
    r[23] <= r[13];
    r[24] <= r[14];
    r[25] <= r[15];
    r[26] <= r[16];
    r[27] <= r[17];
    r[28] <= r[18];
    r[29] <= r[19];
    r[30] <= r[20];
    r[31] <= r[21];
    r[32] <= r[22];
    r[33] <= r[23];

  end

  assign out_0 = r[24];
  assign out_1 = r[25];
  assign out_2 = r[26];
  assign out_3 = r[27];
  assign out_4 = r[28];
  assign out_5 = r[29];
  assign out_6 = r[30];
  assign out_7 = r[31];
  assign out_8 = r[32];
  assign out_9 = r[33];

endmodule