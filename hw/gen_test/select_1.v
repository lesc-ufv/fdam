
module select_1 #
(
  parameter DATA_WIDTH = 32
)
(
  input clk,
  input rst,
  input [1-1:0] data_in_valid,
  input [DATA_WIDTH-1:0] data_in_0,
  output reg [DATA_WIDTH-1:0] data_out,
  output reg data_out_valid
);


  always @(posedge clk) begin
    if(rst) begin
      data_out <= 0;
    end else begin
      case(data_in_valid)
           1: data_out <= data_in_0;
           default:data_out <= 0;
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      data_out_valid <= 0;
    end else begin
      data_out_valid <= |data_in_valid;
    end
  end


endmodule
