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

  (* ramstyle = "AUTO, no_rw_check" *) reg  [DATA_WIDTH-1:0] mem[0:2**ADDR_WIDTH-1];
  /*
  reg [DATA_WIDTH-1:0] mem [0:2**ADDR_WIDTH-1];
  */

  always @(posedge clk) begin
    if(we) begin
      mem[waddr] <= din;
    end 
    if(re) begin
      dout <= mem[raddr];
    end 
  end

  //synthesis translate_off
  integer i;

  initial begin
    dout = 0;
    for(i=0; i<2**ADDR_WIDTH; i=i+1) begin
      mem[i] = 0;
    end
  end

  //synthesis translate_on

endmodule