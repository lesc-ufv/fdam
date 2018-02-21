
module select_8 #
(
  parameter DATA_WIDTH = 32
)
(
  input clk,
  input rst,
  input [8-1:0] data_in_valid,
  input [DATA_WIDTH-1:0] data_in_0,
  input [DATA_WIDTH-1:0] data_in_1,
  input [DATA_WIDTH-1:0] data_in_2,
  input [DATA_WIDTH-1:0] data_in_3,
  input [DATA_WIDTH-1:0] data_in_4,
  input [DATA_WIDTH-1:0] data_in_5,
  input [DATA_WIDTH-1:0] data_in_6,
  input [DATA_WIDTH-1:0] data_in_7,
  output reg [DATA_WIDTH-1:0] data_out,
  output reg data_out_valid
);


  always @(posedge clk) begin
    if(rst) begin
      data_out <= 0;
    end else begin
      case(data_in_valid)
           1: data_out <= data_in_0;
           2: data_out <= data_in_1;
           4: data_out <= data_in_2;
           8: data_out <= data_in_3;
           16: data_out <= data_in_4;
           32: data_out <= data_in_5;
           64: data_out <= data_in_6;
           128: data_out <= data_in_7;
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
