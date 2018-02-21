
module afu_0 #
(
  parameter ADDR_WIDTH = 64,
  parameter QTD_WIDTH = 32,
  parameter DATA_WIDTH = 512,
  parameter CONF_AFU_ID_WIDTH = 64,
  parameter CONF_ID_QUEUE_WIDTH = 32,
  parameter INITIAL_INPUT_QUEUE_ID = 0,
  parameter INITIAL_OUTPUT_QUEUE_ID = 0,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1,
  parameter TAG_WIDTH = 16,
  parameter FIFO_DEPTH_BITS = 4,
  parameter FIFO_FULL = 2 ** FIFO_DEPTH_BITS,
  parameter NUM_CL_DSM_RD = $rtoi($ceil(NUM_INPUT_QUEUES/8)),
  parameter NUM_CL_DSM_WR = $rtoi($ceil(NUM_OUTPUT_QUEUES/8)),
  parameter NUM_CL_DSM_TOTAL = 1 + NUM_CL_DSM_RD + NUM_CL_DSM_WR,
  parameter NUM_CL_DSM_TOTAL_BITS = $rtoi($ceil($clog2(NUM_CL_DSM_TOTAL))),
  parameter DSM_ADDR_WIDTH = $rtoi($ceil($clog2(NUM_CL_DSM_TOTAL))),
  parameter DSM_DATA_WIDTH = 512,
  parameter DSM_NUM_CL = 1 + NUM_CL_DSM_RD + NUM_CL_DSM_WR
)
(
  input clk,
  input rst,
  input start,
  input [2-1:0] conf_valid,
  input [ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0] conf,
  input [NUM_INPUT_QUEUES-1:0] available_read,
  output [NUM_INPUT_QUEUES-1:0] request_read,
  output [(ADDR_WIDTH+TAG_WIDTH)*NUM_INPUT_QUEUES-1:0] request_data,
  input read_data_valid,
  input [TAG_WIDTH-1:0] read_queue_id,
  input [DATA_WIDTH-1:0] read_data,
  input [NUM_OUTPUT_QUEUES-1:0] available_write,
  output [NUM_OUTPUT_QUEUES-1:0] request_write,
  output [(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)*NUM_OUTPUT_QUEUES-1:0] write_data,
  input write_data_valid,
  input [TAG_WIDTH-1:0] write_queue_id
);

  wire [NUM_INPUT_QUEUES-1:0] afu_user_available_read;
  wire [NUM_INPUT_QUEUES-1:0] afu_user_request_read;
  wire [DATA_WIDTH*NUM_INPUT_QUEUES-1:0] afu_user_read_data;
  wire [NUM_INPUT_QUEUES-1:0] afu_user_read_data_valid;
  wire [NUM_OUTPUT_QUEUES-1:0] afu_user_available_write;
  wire [NUM_OUTPUT_QUEUES-1:0] afu_user_request_write;
  wire [DATA_WIDTH*NUM_OUTPUT_QUEUES-1:0] afu_user_write_data;
  wire afu_user_done;
  wire afu_user_done_dsm;
  wire [NUM_INPUT_QUEUES-1:0] has_peding_rd;
  wire [NUM_OUTPUT_QUEUES-1:0] has_peding_wr;
  wire has_peding;
  wire [NUM_INPUT_QUEUES-1:0] input_queue_done;
  wire [NUM_OUTPUT_QUEUES-1:0] output_queue_done;
  wire afu_dsm_update;
  wire afu_dsm_valid;
  wire [DSM_DATA_WIDTH-1:0] afu_dsm_data;
  wire afu_dsm_req_rd;
  wire [DSM_ADDR_WIDTH-1:0] afu_dsm_addr;
  genvar idx_in_queue;
  genvar idx_out_queue;

  generate for(idx_in_queue=0; idx_in_queue<NUM_INPUT_QUEUES; idx_in_queue=idx_in_queue+1) begin : gen_in_queue_controller

    input_queue_controller
    #(
      .ID_QUEUE(INITIAL_INPUT_QUEUE_ID + idx_in_queue),
      .ADDR_WIDTH(ADDR_WIDTH),
      .QTD_WIDTH(QTD_WIDTH),
      .DATA_WIDTH(DATA_WIDTH),
      .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
      .TAG_WIDTH(TAG_WIDTH),
      .FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
      .FIFO_FULL(FIFO_FULL)
    )
    input_queue_controller
    (
      .clk(clk),
      .rst(rst),
      .start(start),
      .conf_valid(conf_valid),
      .conf(conf),
      .available_read(available_read[idx_in_queue]),
      .has_peding(has_peding_rd[idx_in_queue]),
      .request_read(request_read[idx_in_queue]),
      .request_data(request_data[idx_in_queue*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH)-1:idx_in_queue*(ADDR_WIDTH+TAG_WIDTH)]),
      .read_data_valid(read_data_valid),
      .read_queue_id(read_queue_id),
      .read_data(read_data),
      .afu_user_available_read(afu_user_available_read[idx_in_queue]),
      .afu_user_request_read(afu_user_request_read[idx_in_queue]),
      .afu_user_read_data(afu_user_read_data[idx_in_queue*DATA_WIDTH+DATA_WIDTH-1:idx_in_queue*DATA_WIDTH]),
      .afu_user_read_data_valid(afu_user_read_data_valid[idx_in_queue]),
      .done(input_queue_done[idx_in_queue])
    );

  end
  endgenerate


  output_queue_dsm_controller
  #(
    .AFU_ID(0),
    .CONF_AFU_ID_WIDTH(CONF_AFU_ID_WIDTH),
    .ID_QUEUE(INITIAL_OUTPUT_QUEUE_ID),
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .TAG_WIDTH(TAG_WIDTH),
    .FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
    .FIFO_FULL(FIFO_FULL),
    .DSM_ADDR_WIDTH(DSM_ADDR_WIDTH),
    .DSM_DATA_WIDTH(DSM_DATA_WIDTH),
    .DSM_NUM_CL(DSM_NUM_CL)
  )
  output_queue_dsm_controller
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .conf_valid(conf_valid),
    .conf(conf),
    .available_write(available_write[0]),
    .has_peding(has_peding_wr[0]),
    .request_write(request_write[0]),
    .write_data(write_data[DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH-1:0]),
    .write_data_valid(write_data_valid),
    .write_queue_id(write_queue_id),
    .afu_user_available_write(afu_user_available_write[0]),
    .afu_user_request_write(afu_user_request_write[0]),
    .afu_user_write_data(afu_user_write_data[DATA_WIDTH-1:0]),
    .afu_dsm_update(afu_dsm_update),
    .afu_dsm_valid(afu_dsm_valid),
    .afu_dsm_data(afu_dsm_data),
    .afu_dsm_req_rd(afu_dsm_req_rd),
    .afu_dsm_addr(afu_dsm_addr),
    .done(output_queue_done[0])
  );


  generate for(idx_out_queue=1; idx_out_queue<NUM_OUTPUT_QUEUES; idx_out_queue=idx_out_queue+1) begin : gen_out_queue_controller

    output_queue_controller
    #(
      .ID_QUEUE(INITIAL_OUTPUT_QUEUE_ID + idx_out_queue),
      .ADDR_WIDTH(ADDR_WIDTH),
      .QTD_WIDTH(QTD_WIDTH),
      .DATA_WIDTH(DATA_WIDTH),
      .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
      .TAG_WIDTH(TAG_WIDTH),
      .FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
      .FIFO_FULL(FIFO_FULL)
    )
    output_queue_controller
    (
      .clk(clk),
      .rst(rst),
      .start(start),
      .conf_valid(conf_valid),
      .conf(conf),
      .available_write(available_write[idx_out_queue]),
      .has_peding(has_peding_wr[idx_out_queue]),
      .request_write(request_write[idx_out_queue]),
      .write_data(write_data[idx_out_queue*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)-1:idx_out_queue*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)]),
      .write_data_valid(write_data_valid),
      .write_queue_id(write_queue_id),
      .afu_user_available_write(afu_user_available_write[idx_out_queue]),
      .afu_user_request_write(afu_user_request_write[idx_out_queue]),
      .afu_user_write_data(afu_user_write_data[idx_out_queue*DATA_WIDTH+DATA_WIDTH-1:idx_out_queue*DATA_WIDTH]),
      .done(output_queue_done[idx_out_queue])
    );

  end
  endgenerate


  dsm_controller
  #(
    .QTD_WIDTH(QTD_WIDTH),
    .DSM_DATA_WIDTH(DSM_DATA_WIDTH),
    .NUM_INPUT_QUEUES(NUM_INPUT_QUEUES),
    .NUM_OUTPUT_QUEUES(NUM_OUTPUT_QUEUES),
    .NUM_CL_DSM_RD(NUM_CL_DSM_RD),
    .NUM_CL_DSM_WR(NUM_CL_DSM_WR),
    .NUM_CL_DSM_TOTAL(NUM_CL_DSM_TOTAL),
    .NUM_CL_DSM_TOTAL_BITS(NUM_CL_DSM_TOTAL_BITS)
  )
  dsm_controller
  (
    .clk(clk),
    .rst(rst),
    .done_rd(input_queue_done),
    .done_wr(output_queue_done),
    .done_afu(afu_user_done_dsm),
    .afu_req_rd_data_en(afu_user_request_read),
    .afu_req_wr_data_en(afu_user_request_write),
    .afu_dsm_req_rd(afu_dsm_req_rd),
    .afu_dsm_addr(afu_dsm_addr),
    .afu_dsm_update(afu_dsm_update),
    .afu_dsm_valid(afu_dsm_valid),
    .afu_dsm_data(afu_dsm_data)
  );


  afu_user_0
  #(
    .DATA_WIDTH(DATA_WIDTH),
    .NUM_INPUT_QUEUES(NUM_INPUT_QUEUES),
    .NUM_OUTPUT_QUEUES(NUM_OUTPUT_QUEUES)
  )
  afu_user_0
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .afu_user_done_rd_data(input_queue_done),
    .afu_user_done_wr_data(output_queue_done),
    .afu_user_available_read(afu_user_available_read),
    .afu_user_request_read(afu_user_request_read),
    .afu_user_read_data_valid(afu_user_read_data_valid),
    .afu_user_read_data(afu_user_read_data),
    .afu_user_available_write(afu_user_available_write),
    .afu_user_request_write(afu_user_request_write),
    .afu_user_write_data(afu_user_write_data),
    .afu_user_done(afu_user_done)
  );

  assign has_peding = |{has_peding_rd,has_peding_wr};
  assign afu_user_done_dsm = afu_user_done && ~has_peding;

endmodule
