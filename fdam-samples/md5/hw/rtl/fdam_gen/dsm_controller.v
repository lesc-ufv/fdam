module dsm_controller #
(
  parameter ACC_ID = 1,
  parameter ADDR_WIDTH = 48,
  parameter QTD_WIDTH = 32,
  parameter TAG_WIDTH = 16,
  parameter CONF_ID_QUEUE_WIDTH = 32,
  parameter DATA_WIDTH = 512,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1
)
(
  input clk,
  input rst,
  input start,
  input [2-1:0] conf_valid,
  input [ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0] conf,
  input [NUM_INPUT_QUEUES-1:0] done_rd,
  input [NUM_OUTPUT_QUEUES-1:0] done_wr,
  input done_acc,
  input [NUM_INPUT_QUEUES-1:0] acc_req_rd_data_en,
  input [NUM_OUTPUT_QUEUES-1:0] acc_req_wr_data_en,
  input write_data_valid,
  input [TAG_WIDTH-1:0] write_queue_id,
  input available_write,
  output reg request_write,
  output reg [DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH-1:0] write_data
);

  localparam NUM_CL_DSM_RD = $rtoi($ceil(NUM_INPUT_QUEUES/16.0));
  localparam NUM_CL_DSM_WR = $rtoi($ceil(NUM_OUTPUT_QUEUES/16.0));
  localparam NUM_CL_DSM_TOTAL = NUM_CL_DSM_RD + NUM_CL_DSM_WR;
  localparam NUM_CL_DSM_TOTAL_ALIGN = $rtoi($ceil(NUM_CL_DSM_TOTAL/4.0)*4.0);
  localparam NUM_CL_DSM_TOTAL_BITS = $rtoi($ceil($clog2(NUM_CL_DSM_TOTAL_ALIGN)));
  localparam CONF_TYPE_OUT_DSM = 3;
  localparam FSM_DSM_UPDATE_IDLE = 0;
  localparam FSM_DSM_UPDATE_DELAY = 1;
  localparam FSM_DSM_UPDATE_WR1 = 2;
  localparam FSM_DSM_UPDATE_WR2 = 3;
  localparam FSM_DSM_UPDATE_WAIT_RESP = 4;
  reg [7-1:0] count_dsm_delay;
  reg [DATA_WIDTH-1:0] dsm_data [0:NUM_CL_DSM_TOTAL-1];
  reg [DATA_WIDTH-1:0] done;
  reg [DATA_WIDTH-1:0] done_last;
  reg [3-1:0] fsm_update_dsm;
  reg [NUM_CL_DSM_TOTAL_BITS+1-1:0] acc_req_wr_count;
  reg update_dsm;
  reg [NUM_CL_DSM_TOTAL_BITS+1-1:0] dsm_count_cl;
  reg [ADDR_WIDTH-1:0] dsm_addr_base;
  reg [QTD_WIDTH-1:0] dsm_qtd_data_cl;
  reg dsm_conf_ready;
  reg [ADDR_WIDTH-1:0] dsm_addr_write_next;
  reg flag_send_done;
  reg reset_dsm_count_cl;
  wire dsm_write_data_valid_queue;
  wire dsm_conf_rd_valid;
  wire dsm_rst_internal;
  wire [((ADDR_WIDTH+QTD_WIDTH)+CONF_ID_QUEUE_WIDTH)-1:0] conf_rd;
  wire [QTD_WIDTH-1:0] acc_in_rd_count [0:(NUM_CL_DSM_RD*16)-1];
  wire [QTD_WIDTH-1:0] acc_in_wr_count [0:(NUM_CL_DSM_WR*16)-1];
  wire [DATA_WIDTH-1:0] dsm_data_wire [0:NUM_CL_DSM_TOTAL-1];
  integer idx;
  genvar i;

  generate for(i=0; i<NUM_INPUT_QUEUES; i=i+1) begin : gen_counter_rd

    acc_counter
    #(
      .DEPTH_BITS(QTD_WIDTH)
    )
    counter_rd
    (
      .clk(clk),
      .rst(dsm_rst_internal),
      .en(acc_req_rd_data_en[i]),
      .dout(acc_in_rd_count[i])
    );

  end
  endgenerate


  generate for(i=0; i<NUM_OUTPUT_QUEUES; i=i+1) begin : gen_counter_wr

    acc_counter
    #(
      .DEPTH_BITS(QTD_WIDTH)
    )
    counter_wr
    (
      .clk(clk),
      .rst(dsm_rst_internal),
      .en(acc_req_wr_data_en[i]),
      .dout(acc_in_wr_count[i])
    );

  end
  endgenerate


  conf_receiver
  #(
    .CONF_TYPE(CONF_TYPE_OUT_DSM),
    .CONF_ID(ACC_ID),
    .CONF_ID_WIDTH(CONF_ID_QUEUE_WIDTH),
    .CONF_WIDTH(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH)
  )
  dsm_conf_receiver
  (
    .clk(clk),
    .rst(rst),
    .conf_in_valid(conf_valid),
    .conf_in_data(conf),
    .conf_out_valid(dsm_conf_rd_valid),
    .conf_out_data(conf_rd),
    .conf_reset_out(dsm_rst_internal)
  );

  assign dsm_write_data_valid_queue = write_data_valid && (write_queue_id == { 1'b1, ACC_ID[TAG_WIDTH-1-1:0] });

  generate for(i=(NUM_CL_DSM_RD*16)-1; i>=NUM_INPUT_QUEUES; i=i-1) begin : gen_zero_rd_count
    assign acc_in_rd_count[i] = { QTD_WIDTH{ 1'b0 } };
  end
  endgenerate


  generate for(i=(NUM_CL_DSM_WR*16)-1; i>=NUM_OUTPUT_QUEUES; i=i-1) begin : gen_zero_wr_count
    assign acc_in_wr_count[i] = { QTD_WIDTH{ 1'b0 } };
  end
  endgenerate


  generate for(i=0; i<NUM_CL_DSM_RD; i=i+1) begin : gen_dsm_data_rd
    assign dsm_data_wire[i] = { acc_in_rd_count[i * 16 + 15], acc_in_rd_count[i * 16 + 14], acc_in_rd_count[i * 16 + 13], acc_in_rd_count[i * 16 + 12], acc_in_rd_count[i * 16 + 11], acc_in_rd_count[i * 16 + 10], acc_in_rd_count[i * 16 + 9], acc_in_rd_count[i * 16 + 8], acc_in_rd_count[i * 16 + 7], acc_in_rd_count[i * 16 + 6], acc_in_rd_count[i * 16 + 5], acc_in_rd_count[i * 16 + 4], acc_in_rd_count[i * 16 + 3], acc_in_rd_count[i * 16 + 2], acc_in_rd_count[i * 16 + 1], acc_in_rd_count[i * 16 + 0] };
  end
  endgenerate


  generate for(i=0; i<NUM_CL_DSM_WR; i=i+1) begin : gen_dsm_data_wr
    assign dsm_data_wire[i + NUM_CL_DSM_WR] = { acc_in_wr_count[i * 16 + 15], acc_in_wr_count[i * 16 + 14], acc_in_wr_count[i * 16 + 13], acc_in_wr_count[i * 16 + 12], acc_in_wr_count[i * 16 + 11], acc_in_wr_count[i * 16 + 10], acc_in_wr_count[i * 16 + 9], acc_in_wr_count[i * 16 + 8], acc_in_wr_count[i * 16 + 7], acc_in_wr_count[i * 16 + 6], acc_in_wr_count[i * 16 + 5], acc_in_wr_count[i * 16 + 4], acc_in_wr_count[i * 16 + 3], acc_in_wr_count[i * 16 + 2], acc_in_wr_count[i * 16 + 1], acc_in_wr_count[i * 16 + 0] };
  end
  endgenerate


  always @(posedge clk) begin
    if(dsm_rst_internal) begin
      dsm_addr_base <= 0;
      dsm_qtd_data_cl <= 0;
      dsm_conf_ready <= 1'b0;
    end else begin
      if(dsm_conf_rd_valid) begin
        dsm_qtd_data_cl <= conf_rd[CONF_ID_QUEUE_WIDTH+QTD_WIDTH-1:CONF_ID_QUEUE_WIDTH];
        dsm_addr_base <= conf_rd[CONF_ID_QUEUE_WIDTH+QTD_WIDTH+ADDR_WIDTH-1:CONF_ID_QUEUE_WIDTH+QTD_WIDTH];
        dsm_conf_ready <= 1'b1;
      end 
    end
  end


  always @(posedge clk) begin
    if(dsm_rst_internal) begin
      dsm_count_cl <= 0;
    end else begin
      if(reset_dsm_count_cl) begin
        dsm_count_cl <= 0;
      end else begin
        if(dsm_write_data_valid_queue) begin
          dsm_count_cl <= dsm_count_cl + 4;
        end 
      end
    end
  end


  always @(posedge clk) begin
    if(dsm_rst_internal) begin
      done <= 512'd0;
      update_dsm <= 1'b0;
    end else begin
      done <= { { DATA_WIDTH - (NUM_INPUT_QUEUES + NUM_OUTPUT_QUEUES + 1){ 1'b0 } }, done_wr, done_rd, done_acc };
      update_dsm <= |(done ^ done_last & done);
    end
  end


  always @(posedge clk) begin
    for(idx=0; idx<NUM_CL_DSM_TOTAL; idx=idx+1) begin
      dsm_data[idx] <= dsm_data_wire[idx];
    end
  end


  always @(posedge clk) begin
    if(dsm_rst_internal) begin
      request_write <= 1'b0;
      fsm_update_dsm <= FSM_DSM_UPDATE_IDLE;
      done_last <= 0;
      acc_req_wr_count <= 0;
      write_data <= 0;
      dsm_addr_write_next <= 0;
      flag_send_done <= 1'b0;
      reset_dsm_count_cl <= 1'b0;
      count_dsm_delay <= 7'd0;
    end else begin
      if(start) begin
        request_write <= 1'b0;
        reset_dsm_count_cl <= 1'b0;
        case(fsm_update_dsm)
          FSM_DSM_UPDATE_IDLE: begin
            if(update_dsm & dsm_conf_ready) begin
              acc_req_wr_count <= 0;
              dsm_addr_write_next <= dsm_addr_base;
              flag_send_done <= 1'b0;
              fsm_update_dsm <= FSM_DSM_UPDATE_DELAY;
            end 
          end
          FSM_DSM_UPDATE_DELAY: begin
            if(count_dsm_delay[6]) begin
              done_last <= done;
              count_dsm_delay[6] <= 1'b0;
              reset_dsm_count_cl <= 1'b1;
              fsm_update_dsm <= FSM_DSM_UPDATE_WR1;
            end else begin
              count_dsm_delay <= count_dsm_delay + 7'd1;
            end
          end
          FSM_DSM_UPDATE_WR1: begin
            if(available_write) begin
              request_write <= 1'b1;
              write_data <= { dsm_data[acc_req_wr_count], dsm_addr_write_next, { 1'b1, ACC_ID[TAG_WIDTH-1-1:0] } };
              acc_req_wr_count <= acc_req_wr_count + 1;
              dsm_addr_write_next <= dsm_addr_write_next + 1;
            end 
            if(acc_req_wr_count == NUM_CL_DSM_TOTAL - 1) begin
              fsm_update_dsm <= FSM_DSM_UPDATE_WR2;
            end 
          end
          FSM_DSM_UPDATE_WR2: begin
            if(flag_send_done) begin
              write_data <= { done_last, dsm_addr_write_next, { 1'b1, ACC_ID[TAG_WIDTH-1-1:0] } };
            end else begin
              write_data <= { 512'd0, dsm_addr_write_next, { 1'b1, ACC_ID[TAG_WIDTH-1-1:0] } };
            end
            if(available_write) begin
              request_write <= 1'b1;
              acc_req_wr_count <= acc_req_wr_count + 1;
              dsm_addr_write_next <= dsm_addr_write_next + 1;
            end 
            if(acc_req_wr_count == NUM_CL_DSM_TOTAL_ALIGN - 1) begin
              fsm_update_dsm <= FSM_DSM_UPDATE_WAIT_RESP;
            end 
          end
          FSM_DSM_UPDATE_WAIT_RESP: begin
            if(dsm_count_cl == NUM_CL_DSM_TOTAL_ALIGN) begin
              if(flag_send_done) begin
                fsm_update_dsm <= FSM_DSM_UPDATE_IDLE;
              end else begin
                dsm_addr_write_next <= dsm_addr_base;
                acc_req_wr_count <= 0;
                flag_send_done <= 1'b1;
                reset_dsm_count_cl <= 1'b1;
                fsm_update_dsm <= FSM_DSM_UPDATE_WR1;
              end
            end 
          end
        endcase
      end 
    end
  end

endmodule
