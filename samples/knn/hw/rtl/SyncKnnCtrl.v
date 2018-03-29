
module SyncKnnCtrl
(
  input clk,
  input rst,
  input en,
  input [18-1:0] din0,
  output reg [18-1:0] dout0,
  output reg [2-1:0] dout1
);

  reg [16-1:0] knn_distance;
  reg flag;

  always @(posedge clk) begin
    if(rst) begin
      dout0 <= 18'd0;
      dout1 <= 2'd0;
      knn_distance <= 16'd0;
      flag <= 1'b0;
    end else begin
      if(en) begin
        //Stop = 00, Done = 10, Valid = 01
        case(din0[17:16])
          2'b0: begin
            dout0 <= 18'd0;
            dout1 <= 2'd0;
          end
          2'b10: begin
            dout0 <= { 2'd2, 16'd0 };
            dout1 <= 2'd2;
          end
          2'b1: begin
            if(!flag) begin
              knn_distance <= din0[15:0];
              flag <= 1'b1;
              dout0 <= { 2'd1, 16'd32767 };
              dout1 <= 2'd1;
            end else if(knn_distance > din0[15:0]) begin
              knn_distance <= din0[15:0];
              dout0 <= { 2'd1, knn_distance };
              dout1 <= 2'd1;
            end else begin
              dout0 <= { 2'd1, din0[15:0] };
              dout1 <= 2'd0;
            end
          end
          default: begin
            dout0 <= 18'd0;
          end
        endcase
      end 
    end
  end


endmodule
