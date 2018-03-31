
module AsyncGrn
(
  input clk,
  input rst,
  input reql0,
  output reg ackl0,
  input [34-1:0] din0,
  output reg reqr0,
  input ackr0,
  output reg [34-1:0] dout0
);

  reg gnr_start;
  reg data_in_valid;
  reg [161-1:0] data_in;
  reg end_data_in;
  reg read_data_en;
  wire has_data_out;
  wire has_lst3_data_out;
  wire [58-1:0] data_out;
  reg [64-1:0] data_out_reg;
  wire [32-1:0] data_to_send [0:2-1];
  wire task_done;
  assign data_to_send[0] = { 3'd0, data_out_reg[28:0] };
  assign data_to_send[1] = { 3'd0, data_out_reg[58:29] };
  reg read_data;
  reg sent_data;
  reg [3-1:0] fsm_read;
  localparam FSM_READ_IDLE = 3'd0;
  localparam FSM_READ_WAIT = 3'd1;
  reg [4-1:0] fsm_process;
  localparam FSM_PROCESS_IDLE = 4'd0;
  localparam FSM_PROCESS_READ = 4'd1;
  localparam FSM_PROCESS_WAIT_PROCESS = 4'd2;
  localparam FSM_PROCESS_READ_DATA = 4'd3;
  localparam FSM_PROCESS_SEND_DATA = 4'd4;
  localparam FSM_PROCESS_WAIT_ACKR = 4'd5;
  localparam FSM_PROCESS_DONE = 4'd6;
  reg [34-1:0] data;
  reg [20-1:0] counter_words;
  reg [2-1:0] counter_send_data;

  always @(posedge clk) begin
    if(rst) begin
      read_data <= 1'b0;
      ackl0 <= 1'b0;
      fsm_read <= FSM_READ_IDLE;
    end else begin
      case(fsm_read)
        FSM_READ_IDLE: begin
          if(reql0 && (read_data == sent_data)) begin
            data <= din0;
            ackl0 <= 1'b1;
            read_data <= ~read_data;
            fsm_read <= FSM_READ_WAIT;
          end 
        end
        FSM_READ_WAIT: begin
          if(!reql0) begin
            ackl0 <= 1'b0;
            fsm_read <= FSM_READ_IDLE;
          end 
        end
      endcase
    end
  end

  reg flag_wait;

  always @(posedge clk) begin
    if(rst) begin
      sent_data <= 1'b0;
      counter_words <= 20'd0;
      gnr_start <= 1'b0;
      data_in_valid <= 1'b0;
      end_data_in <= 1'b0;
      read_data_en <= 1'b0;
      data_in <= 161'd0;
      fsm_process <= FSM_PROCESS_IDLE;
    end else begin
      gnr_start <= 1'b1;
      data_in_valid <= 1'b0;
      read_data_en <= 1'b0;
      case(fsm_process)
        FSM_PROCESS_IDLE: begin
          if(sent_data != read_data) begin
            fsm_process <= FSM_PROCESS_READ;
          end 
        end
        FSM_PROCESS_READ: begin
          case(data[33:32])
            2'b1: begin
              if(counter_words < 20'd5) begin
                data_in <= { data[31:0], data_in[160:32] };
                sent_data <= ~sent_data;
                counter_words <= counter_words + 20'd1;
                fsm_process <= FSM_PROCESS_IDLE;
              end else begin
                data_in <= data_in >> 1'b1;
                data_in_valid <= 1'b1;
                fsm_process <= FSM_PROCESS_WAIT_PROCESS;
              end
            end
          endcase
        end
        FSM_PROCESS_WAIT_PROCESS: begin
          end_data_in <= 1'b1;
          if(has_data_out) begin
            read_data_en <= 1'b1;
            flag_wait <= 1'b0;
            fsm_process <= FSM_PROCESS_READ_DATA;
          end else if(task_done) begin
            if(!ackr0) begin
              dout0 <= { 2'b10, 32'd0 };
              reqr0 <= 1'b1;
              fsm_process <= FSM_PROCESS_DONE;
            end 
          end 
        end
        FSM_PROCESS_READ_DATA: begin
          if(!flag_wait) begin
            flag_wait <= 1'b1;
          end else begin
            data_out_reg <= data_out;
            counter_send_data <= 2'd0;
            fsm_process <= FSM_PROCESS_SEND_DATA;
          end
        end
        FSM_PROCESS_SEND_DATA: begin
          if(!ackr0) begin
            dout0 <= { 2'b1, data_to_send[counter_send_data[0]] };
            counter_send_data <= counter_send_data + 2'd1;
            reqr0 <= 1'b1;
            fsm_process <= FSM_PROCESS_WAIT_ACKR;
          end 
        end
        FSM_PROCESS_WAIT_ACKR: begin
          if(ackr0) begin
            reqr0 <= 1'b0;
            if(counter_send_data[1]) begin
              fsm_process <= FSM_PROCESS_WAIT_PROCESS;
            end else begin
              fsm_process <= FSM_PROCESS_SEND_DATA;
            end
          end 
        end
        FSM_PROCESS_DONE: begin
          if(ackr0) begin
            reqr0 <= 1'b0;
          end 
        end
      endcase
    end
  end


  regulator_network
  #(
    .ID(0)
  )
  gnr_network
  (
    .clk(clk),
    .rst(rst),
    .start(gnr_start),
    .data_in_valid(data_in_valid),
    .data_in(data_in),
    .end_data_in(end_data_in),
    .read_data_en(read_data_en),
    .has_data_out(has_data_out),
    .has_lst3_data_out(has_lst3_data_out),
    .data_out(data_out),
    .task_done(task_done)
  );


endmodule
