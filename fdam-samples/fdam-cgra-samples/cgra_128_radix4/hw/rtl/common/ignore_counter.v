module ignore_counter #
(
  parameter WIDTH = 8
)
(
  input clk,
  input rst,
  input en,
  input [WIDTH-1:0] limit,
  output reg end_counter
);

  reg [WIDTH+1-1:0] count;

  initial begin
    count = 0;
    end_counter = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      count <= 0;
      end_counter <= 1;
    end else begin
      end_counter <= 0;
      if(en & ~count[WIDTH + 1 - 1]) begin
        count <= count + 1;
      end 
      if(count >= limit) begin
        end_counter <= 1;
      end 
    end
  end

endmodule