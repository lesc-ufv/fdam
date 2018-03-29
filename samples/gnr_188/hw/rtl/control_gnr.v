
module control_gnr #
(
  parameter ID = 0
)
(
  input clk,
  input rst,
  input start,
  input [188-1:0] s0,
  input [188-1:0] s1,
  input fifo_out_full,
  input fifo_in_empty,
  input [376-1:0] fifo_data_in,
  input end_data_in,
  output reg fifo_in_re,
  output reg start_s0,
  output reg start_s1,
  output reg reset_nos,
  output reg [188-1:0] init_state,
  output reg fifo_out_we,
  input fifo_out_empty,
  output reg [246-1:0] data_out,
  output reg done
);

  reg [188-1:0] state_net;
  reg [29-1:0] period;
  reg [29-1:0] transient;
  reg [188-1:0] end_state_reg;
  reg [29-1:0] period_count;
  reg [29-1:0] transient_count;
  reg start_count_period;
  reg start_count_transient;
  reg reset_counts;
  reg pass_cycle;
  reg [3-1:0] fsm_state;
  reg flag_rd;
  reg flag_wr;
  reg pass_cycle_attractor;
  localparam IDLE = 0;
  localparam GET_STATE = 1;
  localparam RESET_NOS = 2;
  localparam START_NOS = 3;
  localparam FIND_ATTRACTOR = 4;
  localparam CALC_PERIOD_ATTRACTOR = 5;
  localparam FIND_NEXT_ATTRACTOR = 6;
  localparam DONE = 7;

  always @(posedge clk) begin
    if(rst) begin
      init_state <= 188'd0;
      start_count_period <= 1'b0;
      start_count_transient <= 1'b0;
      done <= 1'b0;
      start_s0 <= 1'b0;
      start_s1 <= 1'b0;
      fifo_out_we <= 1'b0;
      reset_counts <= 1'b0;
      state_net <= 188'd0;
      period <= 29'd0;
      transient <= 29'd0;
      end_state_reg <= 188'd0;
      fifo_in_re <= 1'b0;
      flag_rd <= 1'b0;
      flag_wr <= 1'b0;
      reset_nos <= 1'b0;
      data_out <= 246'd0;
      fsm_state <= IDLE;
      pass_cycle_attractor <= 1'b1;
    end else begin
      if(start) begin
        fifo_out_we <= 1'b0;
        reset_nos <= 1'b0;
        reset_counts <= 1'b0;
        fifo_in_re <= 1'b0;
        case(fsm_state)
          IDLE: begin
            if(~fifo_in_empty) begin
              fifo_in_re <= 1'b1;
              flag_rd <= 1'b0;
              fsm_state <= GET_STATE;
            end else if(end_data_in) begin
              fsm_state <= DONE;
            end 
          end
          GET_STATE: begin
            if(flag_rd) begin
              init_state <= fifo_data_in[187:0];
              end_state_reg <= fifo_data_in[375:188];
              flag_rd <= 1'b0;
              fsm_state <= RESET_NOS;
            end else begin
              flag_rd <= 1'b1;
            end
          end
          RESET_NOS: begin
            reset_nos <= 1'b1;
            reset_counts <= 1'b1;
            fsm_state <= START_NOS;
          end
          START_NOS: begin
            start_s0 <= 1'b1;
            start_s1 <= 1'b1;
            pass_cycle_attractor <= 1'b1;
            fsm_state <= FIND_ATTRACTOR;
          end
          FIND_ATTRACTOR: begin
            if(pass_cycle_attractor) begin
              pass_cycle_attractor <= 1'b0;
              if((s0 == s1) && start_count_transient) begin
                state_net <= s0;
                if(s0 == init_state) begin
                  transient <= 29'd0;
                end else begin
                  transient <= transient_count;
                end
                start_count_transient <= 1'b0;
                start_s0 <= 1'b0;
                start_s1 <= 1'b0;
                fsm_state <= CALC_PERIOD_ATTRACTOR;
              end else begin
                start_count_transient <= 1'b1;
                fsm_state <= FIND_ATTRACTOR;
              end
            end else begin
              pass_cycle_attractor <= 1'b1;
            end
          end
          CALC_PERIOD_ATTRACTOR: begin
            if((state_net == s1) && start_count_period) begin
              start_count_period <= 1'b0;
              period <= period_count;
              start_s1 <= 1'b0;
              fsm_state <= FIND_NEXT_ATTRACTOR;
            end else begin
              fsm_state <= CALC_PERIOD_ATTRACTOR;
              start_count_period <= 1'b1;
              start_s1 <= 1'b1;
            end
          end
          FIND_NEXT_ATTRACTOR: begin
            if(~fifo_out_full) begin
              fifo_out_we <= 1'b1;
              data_out <= { state_net, transient, period };
              if(init_state < end_state_reg) begin
                init_state <= init_state + 188'd1;
                fsm_state <= RESET_NOS;
              end else begin
                fsm_state <= DONE;
                $display("%d: ID: %d DONE %d", 0, ID, (init_state + 1));
              end
            end 
          end
          DONE: begin
            if(fifo_in_empty) begin
              if(end_data_in) begin
                if(flag_wr) begin
                  if(fifo_out_empty) begin
                    done <= 1'b1;
                  end 
                  flag_wr <= 1'b0;
                end else begin
                  flag_wr <= 1'b1;
                end
              end 
              fsm_state <= DONE;
            end else begin
              fifo_in_re <= 1'b1;
              flag_rd <= 1'b0;
              init_state <= 188'd0;
              fsm_state <= GET_STATE;
            end
          end
        endcase
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      period_count <= 29'd0;
      transient_count <= 29'd0;
      pass_cycle <= 1'b0;
    end else begin
      if(start) begin
        if(reset_counts) begin
          period_count <= 29'd0;
          transient_count <= 29'd0;
          pass_cycle <= 1'b1;
        end else begin
          if(pass_cycle) begin
            if(start_count_period) begin
              period_count <= period_count + 29'd1;
            end 
            if(start_count_transient) begin
              transient_count <= transient_count + 29'd1;
              pass_cycle <= 1'b0;
            end 
          end else begin
            pass_cycle <= 1'b1;
          end
        end
      end 
    end
  end


endmodule
