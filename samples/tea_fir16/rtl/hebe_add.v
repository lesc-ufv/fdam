
module hebe_add #
(
  parameter N = 16
)
(
  input clk,
  input rst,
  input en,
  input [N-1:0] din1,
  input [N-1:0] din2,
  input rin1,
  input rin2,
  output reg [1-1:0] rout,
  output reg [N-1:0] dout
);


  always @(posedge clk or posedge rst) begin
    if(rst) begin
      rout <= 1'd0;
      dout <= 0;
    end else begin
      if(clk & en) begin
        if((rin1 == 1'd1) & (rin2 == 1'd1)) begin
          dout <= din1 + din2;
          rout <= 1'd1;
        end else begin
          rout <= 1'd0;
        end
      end 
    end
  end


endmodule
