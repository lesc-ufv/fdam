module cgra0_control_exec
(
  input clk,
  input rst,
  input start,
  input [32-1:0] read_fifo_mask,
  input [32-1:0] write_fifo_mask,
  input [32-1:0] available_read,
  input [32-1:0] available_write,
  input [32-1:0] available_pop,
  input [32-1:0] available_push,
  input [32-1:0] read_fifo_done,
  input [32-1:0] write_fifo_done,
  output reg [128-1:0] en_pe,
  output reg [1280-1:0] en_net,
  output reg [256-1:0] en_pc_net,
  output reg [32-1:0] en_fecth_data,
  output reg done
);

  localparam FSM_IDLE = 0;
  localparam FSM_PROCESS = 1;
  localparam FSM_DONE = 3;
  localparam FSM_WAIT_DATA = 2;

  reg [2-1:0] fsm_state;
  reg [32-1:0] en_read;
  reg [32-1:0] en_write;
  reg [32-1:0] en_read1;
  reg [32-1:0] en_write1;
  reg [1664-1:0] en_process;
  reg [1664-1:0] en_process1;
  reg [32-1:0] available_pop_masked;
  reg [32-1:0] available_push_masked;
  reg available_queues;

  integer i;

  always @(posedge clk) begin
    if(rst) begin
      available_pop_masked <= 0;
      available_push_masked <= 0;
      available_queues <= 0;
    end else begin
      available_pop_masked <= available_pop | ~read_fifo_mask;
      available_push_masked <= available_push | ~write_fifo_mask;
      available_queues <= &(available_pop_masked & available_push_masked);
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      en_process <= 0;
      en_process1 <= 0;
      en_read <= 0;
      en_write <= 0;
      en_read1 <= 0;
      en_write1 <= 0;
      en_pe <= 0;
      en_net <= 0;
      en_pc_net <= 0;
    end else begin
      en_read <= available_read | read_fifo_done;
      en_write <= available_write | write_fifo_done;
      en_read1 <= en_read & read_fifo_mask;
      en_write1 <= en_write & write_fifo_mask;
      for(i=0; i<1664; i=i+1) begin
        en_process[i] <= |en_read1 & |en_write1;
        en_process1[i] <= en_process[i] & fsm_state[0];
      end
      for(i=0; i<128; i=i+1) begin
        en_pe[i] <= en_process1[i];
      end
      for(i=0; i<1280; i=i+1) begin
        en_net[i] <= en_process1[i + 128];
      end
      for(i=0; i<256; i=i+1) begin
        en_pc_net[i] <= en_process1[i + 128 + 1280];
      end
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      fsm_state <= FSM_IDLE;
      en_fecth_data <= 32'h0;
      done <= 0;
    end else begin
      case(fsm_state)
        FSM_IDLE: begin
          if(start) begin
            fsm_state <= FSM_WAIT_DATA;
            en_fecth_data <= 32'hffffffff;
          end 
        end
        FSM_WAIT_DATA: begin
          if(available_queues) begin
            fsm_state <= FSM_PROCESS;
          end 
        end
        FSM_PROCESS: begin
          if(&(write_fifo_done | ~write_fifo_mask)) begin
            fsm_state <= FSM_DONE;
          end 
        end
        FSM_DONE: begin
          done <= 1;
        end
      endcase
    end
  end

endmodule