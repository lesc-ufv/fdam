module thread_counter_8
(
  input clk,
  input rst,
  input en,
  output reg [3-1:0] out
);


  initial begin
    out = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      out <= 0;
    end else begin
      if(en) begin
        out <= out + 1;
        if(out >= 7) begin
          out <= 0;
        end 
      end 
    end
  end

endmodule