
module SyncAdd
(
  input clk,
  input rst,
  input en,
  input [34-1:0] din0,
  input [34-1:0] din1,
  output reg [34-1:0] dout0
);


  always @(posedge clk) begin
    if(rst) begin
      dout0 <= 34'd0;
    end else begin
      if(en) begin
        //Stop = {00,00}, Done = {10,10}, Valid = {01,01}
        case({ din0[33:32], din1[33:32] })
          4'b0: begin
            dout0 <= 34'd0;
          end
          { 2'b10, 2'b10 }: begin
            dout0 <= { 2'd2, 32'd0 };
          end
          { 2'b1, 2'b1 }: begin
            dout0 <= { 2'd1, din0[31:0] + din1[31:0] };
          end
          default: begin
            dout0 <= 34'd0;
          end
        endcase
      end 
    end
  end


endmodule
