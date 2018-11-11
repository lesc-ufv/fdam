module cgra_initial_conf_control
(
  input clk,
  input rst,
  input start,
  input available_read,
  output req_rd_data,
  input [512-1:0] rd_data,
  input rd_data_valid,
  output reg [45-1:0] conf_pe_init_out_bus,
  output reg [32-1:0] qtd_conf,
  output reg [32-1:0] qtd_net_conf,
  output reg [2-1:0] read_fifo_mask,
  output reg [2-1:0] write_fifo_mask,
  output reg done
);

  localparam FSM_INIT_CTRL_IDLE = 0;
  localparam FSM_INIT_CTRL_INIT = 1;
  localparam FSM_SEND_INIT_CONF_PE = 2;
  localparam FSM_INIT_CTRL_WAIT_DATA = 3;
  localparam FSM_INIT_CTRL_REQ_DATA = 4;
  localparam FSM_INIT_CONF_DONE = 5;

  reg [3-1:0] fsm_init_conf_ctrl;
  reg [3-1:0] fsm_init_conf_ctrl_next;
  reg init_conf_req_data;
  reg [512-1:0] init_conf_cl;
  reg [32-1:0] qtd_init_conf;
  reg [5-1:0] init_conf_id;
  reg [16-1:0] init_conf_const;
  reg [8-1:0] init_conf_pc_max;
  reg [8-1:0] init_conf_loop;
  reg [8-1:0] init_conf_ignore;
  reg send_init_conf;
  reg [32-1:0] conf_init_counter;
  reg [3-1:0] conf_init_counter_cl;

  assign req_rd_data = init_conf_req_data;

  always @(posedge clk) begin
    if(rst) begin
      fsm_init_conf_ctrl <= FSM_INIT_CTRL_IDLE;
      fsm_init_conf_ctrl_next <= FSM_INIT_CTRL_IDLE;
      init_conf_req_data <= 0;
      init_conf_id <= 0;
      init_conf_pc_max <= 0;
      init_conf_loop <= 0;
      init_conf_ignore <= 0;
      send_init_conf <= 0;
      conf_init_counter <= 0;
      conf_init_counter_cl <= 3'd4;
      done <= 0;
      read_fifo_mask <= 0;
      write_fifo_mask <= 0;
    end else begin
      init_conf_req_data <= 0;
      send_init_conf <= 0;
      case(fsm_init_conf_ctrl)
        FSM_INIT_CTRL_IDLE: begin
          if(start) begin
            fsm_init_conf_ctrl <= FSM_INIT_CTRL_REQ_DATA;
            fsm_init_conf_ctrl_next <= FSM_INIT_CTRL_INIT;
          end 
        end
        FSM_INIT_CTRL_INIT: begin
          qtd_init_conf <= init_conf_cl[31:0];
          qtd_conf <= init_conf_cl[63:32];
          qtd_net_conf <= init_conf_cl[95:64];
          read_fifo_mask <= init_conf_cl[97:96];
          write_fifo_mask <= init_conf_cl[129:128];
          fsm_init_conf_ctrl <= FSM_SEND_INIT_CONF_PE;
        end
        FSM_SEND_INIT_CONF_PE: begin
          if(conf_init_counter >= qtd_init_conf) begin
            fsm_init_conf_ctrl <= FSM_INIT_CONF_DONE;
          end else if(conf_init_counter_cl < 3'd4) begin
            init_conf_id <= init_conf_cl[4:0];
            init_conf_const <= init_conf_cl[31:16];
            init_conf_pc_max <= init_conf_cl[39:32];
            init_conf_loop <= init_conf_cl[71:64];
            init_conf_ignore <= init_conf_cl[103:96];
            init_conf_cl <= init_conf_cl[511:128];
            send_init_conf <= 1;
            conf_init_counter <= conf_init_counter + 1;
            conf_init_counter_cl <= conf_init_counter_cl + 1;
          end else begin
            conf_init_counter_cl <= 3'd0;
            fsm_init_conf_ctrl <= FSM_INIT_CTRL_REQ_DATA;
            fsm_init_conf_ctrl_next <= FSM_SEND_INIT_CONF_PE;
          end
        end
        FSM_INIT_CTRL_REQ_DATA: begin
          if(available_read) begin
            init_conf_req_data <= 1;
            fsm_init_conf_ctrl <= FSM_INIT_CTRL_WAIT_DATA;
          end 
        end
        FSM_INIT_CTRL_WAIT_DATA: begin
          if(rd_data_valid) begin
            init_conf_cl <= rd_data;
            fsm_init_conf_ctrl <= fsm_init_conf_ctrl_next;
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
      conf_pe_init_out_bus <= 0;
    end else begin
      if(send_init_conf) begin
        conf_pe_init_out_bus <= { init_conf_ignore, init_conf_loop, init_conf_pc_max, init_conf_const, init_conf_id };
      end else begin
        conf_pe_init_out_bus <= 0;
      end
    end
  end

endmodule