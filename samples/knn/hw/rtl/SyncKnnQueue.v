
module SyncKnnQueue
(
  input clk,
  input rst,
  input en,
  input [2-1:0] din0,
  input [18-1:0] din1,
  output reg [18-1:0] dout0
);

  reg [16-1:0] knn_value;
  reg last_scan;

  always @(posedge clk) begin
    if(rst) begin
      dout0 <= 18'd0;
      knn_value <= 16'd0;
      last_scan <= 1'b0;
    end else begin
      if(en) begin
        //Stop = 00, Done = 10, Valid = 01
        case(din1[17:16])
          4'b0: begin
            dout0 <= 18'd0;
          end
          2'b10: begin
            if(!last_scan) begin
              dout0 <= { 2'd1, knn_value };
              last_scan <= 1'b1;
            end else begin
              dout0 <= { 2'd2, 16'd0 };
            end
          end
          2'b1: begin
            if(din0 == 2'd0) begin
              dout0 <= { 2'd3, din1[15:0] };
            end else if(din0 == 2'd1) begin
              knn_value <= din1[15:0];
              dout0 <= { 2'd3, knn_value };
            end else begin
              knn_value <= din1[15:0];
              dout0 <= { 2'd1, knn_value };
            end
          end
          2'b11: begin
            if(din0 == 2'd0) begin
              dout0 <= { 2'd3, din1[15:0] };
            end else if(din0 == 2'd1) begin
              knn_value <= din1[15:0];
              dout0 <= { 2'd3, knn_value };
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
