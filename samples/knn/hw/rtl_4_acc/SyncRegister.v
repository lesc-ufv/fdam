
module SyncRegister
(
  input clk,
  input rst,
  input en,
  input [18-1:0] din0,
  output reg [18-1:0] dout0
);


  always @(posedge clk) begin
    if(rst) begin
      dout0 <= 18'd0;
    end else begin
      if(en) begin
        //Stop = 00, Done = 10, Valid = 01
        case(din0[17:16])
          2'b0: begin
            dout0 <= 18'd0;
          end
          2'b10: begin
            dout0 <= { 2'd2, 16'd0 };
          end
          2'b1: begin
            dout0 <= { 2'd1, din0[15:0] };
          end
          default: begin
            dout0 <= 18'd0;
          end
        endcase
      end 
    end
  end


endmodule
