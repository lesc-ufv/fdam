
module SyncOut
(
  input clk,
  input rst,
  input start,
  input en,
  input wr_available,
  output reg wr_en,
  output reg [512-1:0] wr_data,
  output reg component_ready,
  input [34-1:0] din0,
  output reg done
);

  localparam DATA_WIDTH = 32'd32;
  reg [512-1:0] data;
  reg [6-1:0] counter;
  reg wr_flag;

  always @(posedge clk) begin
    if(rst) begin
      wr_en <= 1'b0;
      component_ready <= 1'b0;
      counter <= 6'd0;
      data <= 512'd0;
      wr_flag <= 1'b0;
      done <= 1'b0;
    end else begin
      if(start && !done) begin
        //Stop = 00, Done = 10, Valid = 01
        component_ready <= 1'b1;
        wr_en <= 1'b0;
        if(!wr_available) begin
          component_ready <= 1'b0;
        end else if(en) begin
          case(din0[33:32])
            2'b10: begin
              component_ready <= 1'b1;
              if(wr_flag) begin
                wr_flag <= 1'b0;
                wr_data <= data;
                wr_en <= 1'b1;
              end else begin
                done <= 1'b1;
              end
            end
            2'b1: begin
              if(counter == 15) begin
                counter <= 6'd0;
                wr_flag <= 1'b0;
                wr_data <= (din0[31:0] << DATA_WIDTH * counter) | data;
                data <= 512'd0;
                wr_en <= 1'b1;
              end else begin
                counter <= counter + 6'd1;
                data <= (din0[31:0] << DATA_WIDTH * counter) | data;
                wr_flag <= 1'b1;
              end
            end
          endcase
        end 
      end 
    end
  end


endmodule
