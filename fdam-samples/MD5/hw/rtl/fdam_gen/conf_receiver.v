module conf_receiver #
(
  parameter CONF_TYPE = 1,
  parameter CONF_ID = 0,
  parameter CONF_ID_WIDTH = 16,
  parameter CONF_WIDTH = 128
)
(
  input clk,
  input rst,
  input [2-1:0] conf_in_valid,
  input [CONF_WIDTH-1:0] conf_in_data,
  output reg conf_out_valid,
  output reg [CONF_WIDTH-1:0] conf_out_data,
  output reg conf_reset_out
);

  reg [2-1:0] fsm_conf_rec;

  always @(posedge clk) begin
    if(rst) begin
      conf_out_valid <= 1'b0;
      conf_out_data <= 0;
      conf_reset_out <= 1'b1;
      fsm_conf_rec <= 2'd0;
    end else begin
      conf_out_valid <= 1'b0;
      case(fsm_conf_rec)
        2'd0: begin
          if((conf_in_valid == CONF_TYPE) && (conf_in_data[CONF_ID_WIDTH-1:0] == CONF_ID)) begin
            conf_out_data <= conf_in_data;
            conf_reset_out <= 1'b1;
            fsm_conf_rec <= 2'd1;
          end 
        end
        2'd1: begin
          conf_reset_out <= 1'b0;
          fsm_conf_rec <= 2'd2;
        end
        2'd2: begin
          conf_out_valid <= 1'b1;
          fsm_conf_rec <= 2'd0;
        end
      endcase
    end
  end

endmodule