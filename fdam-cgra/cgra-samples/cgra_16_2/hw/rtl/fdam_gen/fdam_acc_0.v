module fdam_acc_0 #
(
  parameter ADDR_WIDTH = 64,
  parameter QTD_WIDTH = 32,
  parameter DATA_WIDTH = 512,
  parameter CONF_ID_QUEUE_WIDTH = 32,
  parameter INITIAL_INPUT_QUEUE_ID = 0,
  parameter INITIAL_OUTPUT_QUEUE_ID = 0,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1,
  parameter TAG_WIDTH = 16
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
  input [NUM_OUTPUT_QUEUES+1-1:0] available_write,
  output [NUM_OUTPUT_QUEUES+1-1:0] request_write,
  output [(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)*(NUM_OUTPUT_QUEUES+1)-1:0] write_data,
  input write_data_valid,
  input [TAG_WIDTH-1:0] write_queue_id
);

  reg rst_reg;
  reg start_reg;
  reg [2-1:0] conf_valid_reg;
  reg [ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0] conf_reg;
  reg read_data_valid_reg;
  reg [TAG_WIDTH-1:0] read_queue_id_reg;
  reg [DATA_WIDTH-1:0] read_data_reg;
  reg write_data_valid_reg;
  reg [TAG_WIDTH-1:0] write_queue_id_reg;
  wire [NUM_INPUT_QUEUES-1:0] acc_user_available_read;
  wire [NUM_INPUT_QUEUES-1:0] acc_user_request_read;
  wire [DATA_WIDTH*NUM_INPUT_QUEUES-1:0] acc_user_read_data;
  wire [NUM_INPUT_QUEUES-1:0] acc_user_read_data_valid;
  wire [NUM_OUTPUT_QUEUES-1:0] acc_user_available_write;
  wire [NUM_OUTPUT_QUEUES-1:0] acc_user_request_write;
  wire [DATA_WIDTH*(NUM_OUTPUT_QUEUES)-1:0] acc_user_write_data;
  wire acc_user_done;
  wire acc_user_done_dsm;
  wire [NUM_INPUT_QUEUES-1:0] has_peding_rd;
  wire [NUM_OUTPUT_QUEUES-1:0] has_peding_wr;
  wire has_peding;
  wire [NUM_INPUT_QUEUES-1:0] input_queue_done;
  wire [NUM_OUTPUT_QUEUES-1:0] output_queue_done;
  genvar idx_in_queue;
  genvar idx_out_queue;

  generate for(idx_in_queue=0; idx_in_queue<NUM_INPUT_QUEUES; idx_in_queue=idx_in_queue+1) begin : gen_in_queue_controller

    fdam_input_queue_controller
    #(
      .ID_QUEUE(INITIAL_INPUT_QUEUE_ID + idx_in_queue),
      .ADDR_WIDTH(ADDR_WIDTH),
      .QTD_WIDTH(QTD_WIDTH),
      .DATA_WIDTH(DATA_WIDTH),
      .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
      .TAG_WIDTH(TAG_WIDTH)
    )
    input_queue_controller
    (
      .clk(clk),
      .rst(rst_reg),
      .start(start_reg),
      .conf_valid(conf_valid_reg),
      .conf(conf_reg),
      .available_read(available_read[idx_in_queue]),
      .has_rd_peding(has_peding_rd[idx_in_queue]),
      .request_read(request_read[idx_in_queue]),
      .request_data(request_data[idx_in_queue*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH)-1:idx_in_queue*(ADDR_WIDTH+TAG_WIDTH)]),
      .read_data_valid(read_data_valid_reg),
      .read_queue_id(read_queue_id_reg),
      .read_data(read_data_reg),
      .acc_user_available_read(acc_user_available_read[idx_in_queue]),
      .acc_user_request_read(acc_user_request_read[idx_in_queue]),
      .acc_user_read_data(acc_user_read_data[idx_in_queue*DATA_WIDTH+DATA_WIDTH-1:idx_in_queue*DATA_WIDTH]),
      .acc_user_read_data_valid(acc_user_read_data_valid[idx_in_queue]),
      .done(input_queue_done[idx_in_queue])
    );

  end
  endgenerate


  generate for(idx_out_queue=0; idx_out_queue<NUM_OUTPUT_QUEUES; idx_out_queue=idx_out_queue+1) begin : gen_out_queue_controller

    fdam_output_queue_controller
    #(
      .ID_QUEUE(INITIAL_OUTPUT_QUEUE_ID + idx_out_queue),
      .ADDR_WIDTH(ADDR_WIDTH),
      .QTD_WIDTH(QTD_WIDTH),
      .DATA_WIDTH(DATA_WIDTH),
      .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
      .TAG_WIDTH(TAG_WIDTH)
    )
    output_queue_controller
    (
      .clk(clk),
      .rst(rst_reg),
      .start(start_reg),
      .conf_valid(conf_valid_reg),
      .conf(conf_reg),
      .available_write(available_write[idx_out_queue]),
      .has_wr_peding(has_peding_wr[idx_out_queue]),
      .request_write(request_write[idx_out_queue]),
      .write_data(write_data[idx_out_queue*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)-1:idx_out_queue*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)]),
      .write_data_valid(write_data_valid_reg),
      .write_queue_id(write_queue_id_reg),
      .acc_user_available_write(acc_user_available_write[idx_out_queue]),
      .acc_user_request_write(acc_user_request_write[idx_out_queue]),
      .acc_user_write_data(acc_user_write_data[idx_out_queue*DATA_WIDTH+DATA_WIDTH-1:idx_out_queue*DATA_WIDTH]),
      .acc_user_done(acc_user_done),
      .done(output_queue_done[idx_out_queue])
    );

  end
  endgenerate


  fdam_dsm_controller
  #(
    .ACC_ID(0),
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .TAG_WIDTH(TAG_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .NUM_INPUT_QUEUES(NUM_INPUT_QUEUES),
    .NUM_OUTPUT_QUEUES(NUM_OUTPUT_QUEUES)
  )
  dsm_controller
  (
    .clk(clk),
    .rst(rst_reg),
    .start(start_reg),
    .done_rd(input_queue_done),
    .done_wr(output_queue_done),
    .done_acc(acc_user_done_dsm),
    .acc_req_rd_data_en(acc_user_request_read),
    .acc_req_wr_data_en(acc_user_request_write),
    .conf_valid(conf_valid_reg),
    .conf(conf_reg),
    .available_write(available_write[NUM_OUTPUT_QUEUES]),
    .request_write(request_write[NUM_OUTPUT_QUEUES]),
    .write_data(write_data[NUM_OUTPUT_QUEUES*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)-1:NUM_OUTPUT_QUEUES*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)]),
    .write_data_valid(write_data_valid_reg),
    .write_queue_id(write_queue_id_reg)
  );


  fdam_acc_user_0
  #(
    .DATA_WIDTH(DATA_WIDTH),
    .NUM_INPUT_QUEUES(NUM_INPUT_QUEUES),
    .NUM_OUTPUT_QUEUES(NUM_OUTPUT_QUEUES)
  )
  acc_user_0
  (
    .clk(clk),
    .rst(rst_reg),
    .start(start_reg),
    .acc_user_done_rd_data(input_queue_done),
    .acc_user_done_wr_data(output_queue_done),
    .acc_user_available_read(acc_user_available_read),
    .acc_user_request_read(acc_user_request_read),
    .acc_user_read_data_valid(acc_user_read_data_valid),
    .acc_user_read_data(acc_user_read_data),
    .acc_user_available_write(acc_user_available_write),
    .acc_user_request_write(acc_user_request_write),
    .acc_user_write_data(acc_user_write_data),
    .acc_user_done(acc_user_done)
  );

  assign has_peding = |{ has_peding_rd, has_peding_wr };
  assign acc_user_done_dsm = acc_user_done && ~has_peding;

  always @(posedge clk) begin
    rst_reg <= rst;
    start_reg <= start;
    conf_valid_reg <= conf_valid;
    conf_reg <= conf;
    read_data_valid_reg <= read_data_valid;
    read_queue_id_reg <= read_queue_id;
    read_data_reg <= read_data;
    write_data_valid_reg <= write_data_valid;
    write_queue_id_reg <= write_queue_id;
  end

endmodule