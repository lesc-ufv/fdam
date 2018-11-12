
module cgra_net_conf_control
(
  input clk,
  input rst,
  input start,
  input [32-1:0] qtd_net_conf,
  input available_read,
  output req_rd_data,
  input [512-1:0] rd_data,
  input rd_data_valid,
  output reg conf_net_we,
  output reg [8-1:0] conf_net_addr,
  output reg [192-1:0] conf_net_data_out,
  output reg done
);

  localparam FSM_CONF_NET_CTRL_IDLE = 0;
  localparam FSM_SEND_CONF_NET = 1;
  localparam FSM_CONF_NET_CTRL_WAIT_DATA = 2;
  localparam FSM_CONF_NET_CTRL_REQ_DATA = 3;
  localparam FSM_CONF_NET_DONE = 4;

  reg [3-1:0] fsm_conf_net_ctrl;
  reg [3-1:0] fsm_conf_net_ctrl_next;
  reg conf_net_req_data;
  reg [512-1:0] conf_net_cl;
  reg [192-1:0] conf_net;
  reg send_conf_net;
  reg [32-1:0] conf_net_counter;
  reg [32-1:0] conf_net_counter_cl;
  reg [8-1:0] conf_net_addr_next;

  assign req_rd_data = conf_net_req_data;

  always @(posedge clk) begin
    if(rst) begin
      fsm_conf_net_ctrl <= FSM_CONF_NET_CTRL_IDLE;
      fsm_conf_net_ctrl_next <= FSM_CONF_NET_CTRL_IDLE;
      conf_net_req_data <= 0;
      send_conf_net <= 0;
      conf_net_counter <= 0;
      conf_net_counter_cl <= 9'd2;
      done <= 0;
    end else begin
      conf_net_req_data <= 0;
      send_conf_net <= 0;
      case(fsm_conf_net_ctrl)
        FSM_CONF_NET_CTRL_IDLE: begin
          if(start) begin
            fsm_conf_net_ctrl <= FSM_SEND_CONF_NET;
          end 
        end
        FSM_SEND_CONF_NET: begin
          if(conf_net_counter >= qtd_net_conf) begin
            fsm_conf_net_ctrl <= FSM_CONF_NET_DONE;
          end else if(conf_net_counter_cl < 9'd2) begin
            conf_net <= conf_net_cl[191:0];
            conf_net_cl <= conf_net_cl[511:192];
            send_conf_net <= 1;
            conf_net_counter <= conf_net_counter + 1;
            conf_net_counter_cl <= conf_net_counter_cl + 1;
          end else begin
            conf_net_counter_cl <= 0;
            fsm_conf_net_ctrl <= FSM_CONF_NET_CTRL_REQ_DATA;
            fsm_conf_net_ctrl_next <= FSM_SEND_CONF_NET;
          end
        end
        FSM_CONF_NET_CTRL_REQ_DATA: begin
          if(available_read) begin
            conf_net_req_data <= 1;
            fsm_conf_net_ctrl <= FSM_CONF_NET_CTRL_WAIT_DATA;
          end 
        end
        FSM_CONF_NET_CTRL_WAIT_DATA: begin
          if(rd_data_valid) begin
            conf_net_cl <= rd_data;
            fsm_conf_net_ctrl <= fsm_conf_net_ctrl_next;
          end 
        end
        FSM_CONF_NET_DONE: begin
          done <= 1;
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      conf_net_addr <= 0;
      conf_net_addr_next <= 0;
      conf_net_we <= 0;
    end else begin
      conf_net_we <= 0;
      if(send_conf_net) begin
        conf_net_addr <= conf_net_addr_next;
        conf_net_addr_next <= conf_net_addr_next + 1;
        conf_net_we <= 1;
      end 
    end
    conf_net_data_out <= conf_net;
  end


endmodule
