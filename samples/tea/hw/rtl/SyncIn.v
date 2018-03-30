
module SyncIn
(
  input clk,
  input rst,
  input en,
  input start,
  input rd_done,
  input rd_available,
  input [512-1:0] rd_data,
  input rd_valid,
  output reg rd_en,
  output reg component_ready,
  output reg [34-1:0] dout0
);

  localparam FSM_IDLE = 1;
  localparam FSM_READ = 2;
  localparam FSM_DATA_OUT = 3;
  localparam FSM_DONE = 4;
  reg [512-1:0] data;
  reg [6-1:0] counter;
  reg [3-1:0] fsm_main;

  always @(posedge clk) begin
    if(rst) begin
      component_ready <= 1'b0;
      rd_en <= 1'b0;
      counter <= 6'd0;
      fsm_main <= FSM_IDLE;
    end else begin
      if(start) begin
        rd_en <= 1'b0;
        component_ready <= 1'b0;
        case(fsm_main)
          FSM_IDLE: begin
            if(rd_available) begin
              rd_en <= 1'b1;
              fsm_main <= FSM_READ;
            end else if(rd_done && !rd_available) begin
              fsm_main <= FSM_DONE;
            end 
          end
          FSM_READ: begin
            if(rd_valid) begin
              data <= rd_data;
              counter <= 6'd0;
              dout0 <= 34'b0;
              fsm_main <= FSM_DATA_OUT;
            end 
          end
          FSM_DATA_OUT: begin
            component_ready <= 1'b1;
            if(en) begin
              dout0 <= { 2'd1, data[31:0] };
              data <= data >> 6'd32;
              counter <= counter + 6'b1;
              if(counter == 15) begin
                fsm_main <= FSM_IDLE;
              end else begin
                fsm_main <= FSM_DATA_OUT;
              end
            end 
          end
          FSM_DONE: begin
            dout0 <= { 2'b10, 32'd0 };
            component_ready <= 1'b1;
            fsm_main <= FSM_DONE;
          end
        endcase
      end 
    end
  end


endmodule
