
module hebe_register #
(
  parameter N = 16
)
(
  input clk,
  input rst,
  input en,
  input rin,
  input [N-1:0] din,
  output reg [1-1:0] rout,
  output reg [N-1:0] dout
);


  always @(posedge clk or posedge rst) begin
    if(rst) begin
      rout <= 1'd0;
      dout <= 0;
    end else begin
      if(clk & en) begin
        if(rin == 1'd1) begin
          dout <= din;
          rout <= 1'd1;
        end else begin
          rout <= 1'd0;
        end
      end 
    end
  end


endmodule
