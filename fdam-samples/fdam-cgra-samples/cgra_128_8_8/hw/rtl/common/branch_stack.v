module branch_stack
(
  input clk,
  input rst,
  input en,
  input push,
  input din,
  input pop,
  output dout
);

  reg [16-1:0] stack;
  assign dout = stack[0];

  initial begin
    stack = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      stack <= 16'd0;
    end else begin
      if(en) begin
        case({ pop, push })
          1: begin
            stack <= { stack[14:0], din };
          end
          2: begin
            stack <= stack[15:1];
          end
          3: begin
            stack[0] <= din;
          end
          default: begin
          end
        endcase
      end 
    end
  end

endmodule