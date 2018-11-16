
module memory #
(
  parameter DATA_WIDTH = 32,
  parameter ADDR_WIDTH = 8
)
(
  input clk,
  input we,
  input re,
  input [ADDR_WIDTH-1:0] raddr,
  input [ADDR_WIDTH-1:0] waddr,
  input [DATA_WIDTH-1:0] din,
  output reg [DATA_WIDTH-1:0] dout
);

  reg [DATA_WIDTH-1:0] mem [0:2**ADDR_WIDTH-1];
  integer i;

  initial begin
    dout = 0;
    for(i=0; i<2**ADDR_WIDTH; i=i+1) begin
      mem[i] = 0;
    end
  end


  always @(posedge clk) begin
    if(we) begin
      mem[waddr] <= din;
    end 
    if(re) begin
      dout <= mem[raddr];
    end 
  end


endmodule
