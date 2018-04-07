
module hebe_muli #
(
  parameter N = 16,
  parameter ID = 0
)
(
  input clk,
  input rst,
  input en,
  input [32-1:0] dconf,
  input rin,
  input [N-1:0] din,
  output reg [1-1:0] rout,
  output reg [N-1:0] dout
);

  reg [24-1:0] immediate;

  always @(posedge clk or posedge rst) begin
    if(rst) begin
      rout <= 1'd0;
      dout <= 0;
      immediate <= ID;
    end else begin
      if(clk) begin
        if(dconf[7:0] == ID) begin
          immediate <= { 8'd0, dconf[N+8-1:8] };
        end 
        if(en) begin
          if(rin == 1'd1) begin
            dout <= din * immediate;
            rout <= 1'd1;
          end else begin
            rout <= 1'd0;
          end
        end 
      end 
    end
  end


endmodule
