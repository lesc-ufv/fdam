module cgra_pe_conf_control
(
  input clk,
  input rst,
  input start,
  input [32-1:0] qtd_conf,
  input available_read,
  output req_rd_data,
  input [512-1:0] rd_data,
  input rd_data_valid,
  output reg [21-1:0] conf_pe_out_bus,
  output reg done
);

  localparam FSM_CONF_PE_CTRL_IDLE = 0;
  localparam FSM_SEND_CONF_PE = 1;
  localparam FSM_CONF_PE_CTRL_WAIT_DATA = 2;
  localparam FSM_CONF_PE_CTRL_REQ_DATA = 3;
  localparam FSM_CONF_PE_DONE = 4;

  reg [3-1:0] fsm_conf_ctrl;
  reg [3-1:0] fsm_conf_ctrl_next;
  reg conf_req_data;
  reg [512-1:0] conf_cl;
  reg [5-1:0] conf_id;
  reg [16-1:0] conf_pe;
  reg send_conf;
  reg [32-1:0] conf_counter;
  reg [5-1:0] conf_counter_cl;

  assign req_rd_data = conf_req_data;

  always @(posedge clk) begin
    if(rst) begin
      fsm_conf_ctrl <= FSM_CONF_PE_CTRL_IDLE;
      fsm_conf_ctrl_next <= FSM_CONF_PE_CTRL_IDLE;
      conf_req_data <= 0;
      send_conf <= 0;
      conf_counter <= 0;
      conf_counter_cl <= 5'd16;
      done <= 0;
    end else begin
      conf_req_data <= 0;
      send_conf <= 0;
      case(fsm_conf_ctrl)
        FSM_CONF_PE_CTRL_IDLE: begin
          if(start) begin
            fsm_conf_ctrl <= FSM_SEND_CONF_PE;
          end 
        end
        FSM_SEND_CONF_PE: begin
          if(conf_counter >= qtd_conf) begin
            fsm_conf_ctrl <= FSM_CONF_PE_DONE;
          end else if(conf_counter_cl < 5'd16) begin
            conf_id <= conf_cl[4:0];
            conf_pe <= conf_cl[31:16];
            conf_cl <= conf_cl[511:32];
            send_conf <= 1;
            conf_counter <= conf_counter + 1;
            conf_counter_cl <= conf_counter_cl + 1;
          end else begin
            conf_counter_cl <= 5'd0;
            fsm_conf_ctrl <= FSM_CONF_PE_CTRL_REQ_DATA;
            fsm_conf_ctrl_next <= FSM_SEND_CONF_PE;
          end
        end
        FSM_CONF_PE_CTRL_REQ_DATA: begin
          if(available_read) begin
            conf_req_data <= 1;
            fsm_conf_ctrl <= FSM_CONF_PE_CTRL_WAIT_DATA;
          end 
        end
        FSM_CONF_PE_CTRL_WAIT_DATA: begin
          if(rd_data_valid) begin
            conf_cl <= rd_data;
            fsm_conf_ctrl <= fsm_conf_ctrl_next;
          end 
        end
        FSM_CONF_PE_DONE: begin
          done <= 1;
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      conf_pe_out_bus <= 0;
    end else begin
      if(send_conf) begin
        conf_pe_out_bus <= { conf_pe, conf_id };
      end else begin
        conf_pe_out_bus <= 0;
      end
    end
  end

endmodule