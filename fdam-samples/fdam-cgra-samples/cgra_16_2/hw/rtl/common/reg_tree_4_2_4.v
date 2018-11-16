module reg_tree_4_2_4 #
(
  parameter DATA_WIDTH = 16
)
(
  input clk,
  input [DATA_WIDTH-1:0] in,
  output [DATA_WIDTH-1:0] out_0,
  output [DATA_WIDTH-1:0] out_1
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

  end



  always @(posedge clk) begin
    r0 <= in;
    r1 <= r0;
    r2 <= r0;
    r3 <= r1;
    r4 <= r2;
    r5 <= r3;
    r6 <= r4;
    r7 <= r5;
    r8 <= r6;
    r9 <= r7;
    r10 <= r8;

  end

  assign out_0 = r9;
  assign out_1 = r10;

endmodule