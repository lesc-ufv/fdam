
module SyncShlI #
(
  parameter ID = 1,
  parameter IMMEDIATE = 1
)
(
  input clk,
  input rst,
  input en,
  input [34-1:0] din0,
  output reg [34-1:0] dout0,
  input [32-1:0] dconf
);

  reg [40-1:0] immediate;

  always @(posedge clk) begin
    if(rst) begin
      dout0 <= 34'd0;
    end else begin
      if(en) begin
        //Stop = 00, Done = 10, Valid = 01
        case(din0[33:32])
          4'b0: begin
            dout0 <= 34'd0;
          end
          2'b10: begin
            dout0 <= { 2'd2, 32'd0 };
          end
          2'b1: begin
            dout0 <= { 2'd1, din0[31:0] << IMMEDIATE[31:0] };
          end
          default: begin
            dout0 <= 34'd0;
          end
        endcase
      end 
    end
  end


endmodule
