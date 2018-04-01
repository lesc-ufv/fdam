
module AsyncIn
(
  input clk,
  input rst,
  input start,
  input rd_done,
  input rd_available,
  input [512-1:0] rd_data,
  input rd_valid,
  output reg rd_en,
  output reg reqr0,
  input ackr0,
  output reg [34-1:0] dout0
);

  reg [3-1:0] fsm_main;
  localparam FSM_IDLE = 3'd0;
  localparam FSM_READ = 3'd1;
  localparam FSM_DATA_OUT = 3'd2;
  localparam FSM_WAIT_ACK = 3'd3;
  localparam FSM_DONE = 3'd4;
  reg [512-1:0] data;
  reg [6-1:0] counter;

  always @(posedge clk) begin
    if(rst) begin
      reqr0 <= 1'b0;
      rd_en <= 1'b0;
      counter <= 6'd0;
      fsm_main <= FSM_IDLE;
    end else begin
      if(start) begin
        rd_en <= 1'b0;
        case(fsm_main)
          FSM_IDLE: begin
            if(rd_available) begin
              rd_en <= 1'b1;
              fsm_main <= FSM_READ;
            end else if(rd_done) begin
              dout0 <= { 2'b10, 32'd0 };
              reqr0 <= 1'b1;
              fsm_main <= FSM_DONE;
            end 
          end
          FSM_READ: begin
            if(rd_valid) begin
              data <= rd_data;
              counter <= 6'd0;
              fsm_main <= FSM_DATA_OUT;
            end 
          end
          FSM_DATA_OUT: begin
            if(!ackr0) begin
              dout0 <= { 2'd1, data[31:0] };
              reqr0 <= 1'b1;
              data <= data >> 6'd32;
              counter <= counter + 6'b1;
              fsm_main <= FSM_WAIT_ACK;
            end 
          end
          FSM_WAIT_ACK: begin
            if(ackr0) begin
              reqr0 <= 1'b0;
              if(counter >= 6'd16) begin
                fsm_main <= FSM_IDLE;
              end else begin
                fsm_main <= FSM_DATA_OUT;
              end
            end 
          end
          FSM_DONE: begin
            if(ackr0) begin
              reqr0 <= 1'b0;
            end 
            fsm_main <= FSM_DONE;
          end
        endcase
      end 
    end
  end


endmodule
