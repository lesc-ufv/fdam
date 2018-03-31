
module AsyncOut
(
  input clk,
  input rst,
  input start,
  input wr_available,
  output reg wr_en,
  output reg [512-1:0] wr_data,
  input reql0,
  output reg ackl0,
  input [34-1:0] din0,
  output reg done
);

  localparam DATA_WIDTH = 32'd32;
  reg [3-1:0] fsm_main;
  localparam FSM_IDLE = 3'd0;
  localparam FSM_WAIT = 3'd1;
  localparam FSM_DONE = 3'd2;
  reg [512-1:0] data;
  reg [6-1:0] counter;

  always @(posedge clk) begin
    if(rst) begin
      wr_en <= 1'b0;
      ackl0 <= 1'b0;
      counter <= 6'd0;
      data <= 512'd0;
      done <= 1'b0;
      fsm_main <= FSM_IDLE;
    end else begin
      if(start && wr_available) begin
        wr_en <= 1'b0;
        case(fsm_main)
          FSM_IDLE: begin
            if(reql0) begin
              ackl0 <= 1'b1;
              case(din0[33:32])
                2'b10: begin
                  if(counter == 6'd15) begin
                    counter <= 6'd0;
                    wr_data <= { din0[31:0], data[511:32] };
                    wr_en <= 1'b1;
                    fsm_main <= FSM_DONE;
                  end else begin
                    counter <= counter + 6'd1;
                    data <= { 32'd0, data[511:32] };
                    ackl0 <= 1'b0;
                  end
                end
                2'b1: begin
                  fsm_main <= FSM_WAIT;
                  if(counter == 6'd15) begin
                    counter <= 6'd0;
                    wr_data <= { din0[31:0], data[511:32] };
                    data <= 512'd0;
                    wr_en <= 1'b1;
                  end else begin
                    counter <= counter + 6'd1;
                    data <= { din0[31:0], data[511:32] };
                  end
                end
              endcase
            end 
          end
          FSM_WAIT: begin
            if(!reql0) begin
              ackl0 <= 1'b0;
              fsm_main <= FSM_IDLE;
            end 
          end
          FSM_DONE: begin
            if(!reql0) begin
              ackl0 <= 1'b0;
            end 
            done <= 1'b1;
            fsm_main <= FSM_DONE;
          end
        endcase
      end 
    end
  end


endmodule
