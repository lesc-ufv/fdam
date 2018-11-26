module cgra0_control_conf
(
  input clk,
  input rst,
  input start,
  input available_read,
  output req_rd_data,
  input [512-1:0] rd_data,
  input rd_data_valid,
  output reg [64-1:0] conf_out_bus,
  output reg [32-1:0] read_fifo_mask,
  output reg [32-1:0] write_fifo_mask,
  output reg done
);

  localparam FSM_INIT_CTRL_IDLE = 0;
  localparam FSM_INIT_CTRL_INIT = 1;
  localparam FSM_SEND_INIT_CONF_PE = 2;
  localparam FSM_INIT_CTRL_WAIT_DATA = 3;
  localparam FSM_INIT_CTRL_REQ_DATA = 4;
  localparam FSM_INIT_CONF_DONE = 5;
  localparam FSM_WAIT_ALL_CONF_FINISH = 6;

  reg [3-1:0] fsm_conf_ctrl;
  reg [3-1:0] fsm_conf_ctrl_next;
  reg conf_req_data;
  reg [512-1:0] conf_cl;
  reg [32-1:0] qtd_conf;
  reg [64-1:0] conf_data;
  reg send_conf;
  reg [32-1:0] conf_counter;
  reg [4-1:0] conf_counter_cl;
  reg [10-1:0] wait_counter;

  assign req_rd_data = conf_req_data;

  always @(posedge clk) begin
    if(rst) begin
      fsm_conf_ctrl <= FSM_INIT_CTRL_IDLE;
      fsm_conf_ctrl_next <= FSM_INIT_CTRL_IDLE;
      conf_req_data <= 0;
      send_conf <= 0;
      conf_counter <= 0;
      conf_counter_cl <= 4'd8;
      done <= 0;
      read_fifo_mask <= 0;
      write_fifo_mask <= 0;
      wait_counter <= 0;
    end else begin
      conf_req_data <= 0;
      send_conf <= 0;
      case(fsm_conf_ctrl)
        FSM_INIT_CTRL_IDLE: begin
          if(start) begin
            fsm_conf_ctrl <= FSM_INIT_CTRL_REQ_DATA;
            fsm_conf_ctrl_next <= FSM_INIT_CTRL_INIT;
          end 
        end
        FSM_INIT_CTRL_INIT: begin
          qtd_conf <= conf_cl[31:0];
          read_fifo_mask <= conf_cl[63:32];
          write_fifo_mask <= conf_cl[95:64];
          fsm_conf_ctrl <= FSM_SEND_INIT_CONF_PE;
        end
        FSM_SEND_INIT_CONF_PE: begin
          if(conf_counter >= qtd_conf) begin
            fsm_conf_ctrl <= FSM_WAIT_ALL_CONF_FINISH;
          end else if(conf_counter_cl < 4'd8) begin
            conf_data <= conf_cl[63:0];
            conf_cl <= conf_cl[511:64];
            send_conf <= 1;
            conf_counter <= conf_counter + 1;
            conf_counter_cl <= conf_counter_cl + 1;
          end else begin
            conf_counter_cl <= 4'd0;
            fsm_conf_ctrl <= FSM_INIT_CTRL_REQ_DATA;
            fsm_conf_ctrl_next <= FSM_SEND_INIT_CONF_PE;
          end
        end
        FSM_INIT_CTRL_REQ_DATA: begin
          if(available_read) begin
            conf_req_data <= 1;
            fsm_conf_ctrl <= FSM_INIT_CTRL_WAIT_DATA;
          end 
        end
        FSM_INIT_CTRL_WAIT_DATA: begin
          if(rd_data_valid) begin
            conf_cl <= rd_data;
            fsm_conf_ctrl <= fsm_conf_ctrl_next;
          end 
        end
        FSM_WAIT_ALL_CONF_FINISH: begin
          wait_counter <= wait_counter + 1;
          if(wait_counter > 384) begin
            fsm_conf_ctrl <= FSM_INIT_CONF_DONE;
          end 
        end
        FSM_INIT_CONF_DONE: begin
          done <= 1;
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      conf_out_bus <= 0;
    end else begin
      if(send_conf) begin
        conf_out_bus <= conf_data;
      end else begin
        conf_out_bus <= 0;
      end
    end
  end

endmodule