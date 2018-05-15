
module acc_counter #
(
  parameter DEPTH_BITS = 32
)
(
  input clk,
  input rst,
  input en,
  output reg [DEPTH_BITS-1:0] dout
);


  always @(posedge clk) begin
    if(rst) begin
      dout <= 0;
    end else begin
      if(en) begin
        dout <= dout + 1;
      end 
    end
  end


endmodule
