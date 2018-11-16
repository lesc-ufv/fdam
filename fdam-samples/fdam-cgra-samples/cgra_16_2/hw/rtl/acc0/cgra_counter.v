
module cgra_counter #
(
  parameter WIDTH = 32
)
(
  input clk,
  input rst,
  input en,
  output [WIDTH-1:0] out,
  input [WIDTH-1:0] limit,
  output reg end_counter
);

  reg [WIDTH-1:0] count;
  assign out = count;

  always @(posedge clk) begin
    if(rst) begin
      count <= 0;
      end_counter <= 0;
    end else begin
      if(en) begin
        count <= count + 1;
        if(count >= limit) begin
          end_counter <= 1'b1;
        end 
      end 
    end
  end


endmodule
