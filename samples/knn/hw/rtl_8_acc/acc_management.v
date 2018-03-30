
module acc_management #
(
  parameter ADDR_WIDTH = 48,
  parameter QTD_WIDTH = 32,
  parameter DATA_WIDTH = 512,
  parameter CONF_ID_QUEUE_WIDTH = 32,
  parameter TAG_WIDTH = 16
)
(
  input clk,
  input rst,
  input [64-1:0] rst_accs,
  input [64-1:0] start_accs,
  input [2-1:0] conf_valid,
  input [128-1:0] conf,
  output reg req_rd_en,
  output reg [ADDR_WIDTH-1:0] req_rd_addr,
  output reg [TAG_WIDTH-1:0] req_rd_mdata,
  input req_rd_available,
  input resp_rd_valid,
  input [DATA_WIDTH-1:0] resp_rd_data,
  input [TAG_WIDTH-1:0] resp_rd_mdata,
  input req_wr_available,
  output reg req_wr_en,
  output reg [ADDR_WIDTH-1:0] req_wr_addr,
  output reg [TAG_WIDTH-1:0] req_wr_mdata,
  output reg [DATA_WIDTH-1:0] req_wr_data,
  input resp_wr_valid,
  input [TAG_WIDTH-1:0] resp_wr_mdata,
  output reg [512-1:0] info
);


  wire [40-1:0] req_wr_en_in;
  wire [(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)*40-1:0] req_wr_data_in;
  wire [40-1:0] req_wr_available_in;

  wire req_wr_available_out;
  wire req_wr_en_out;
  wire [DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH-1:0] req_wr_data_out;

  wire [32-1:0] req_rd_en_in;
  wire [(ADDR_WIDTH+TAG_WIDTH)*32-1:0] req_rd_data_in;
  wire [32-1:0] req_rd_available_in;

  wire req_rd_available_out;
  wire req_rd_en_out;
  wire [ADDR_WIDTH+TAG_WIDTH-1:0] req_rd_data_out;


  acc_0
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(0),
    .INITIAL_OUTPUT_QUEUE_ID(0),
    .NUM_INPUT_QUEUES(4),
    .NUM_OUTPUT_QUEUES(4),
    .TAG_WIDTH(TAG_WIDTH)
  )
  acc_0_4x4
  (
    .clk(clk),
    .rst(rst | rst_accs[0]),
    .start(start_accs[0]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[3:0]),
    .request_read(req_rd_en_in[3:0]),
    .request_data(req_rd_data_in[4*(0*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:4*(0*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[4:0]),
    .request_write(req_wr_en_in[4:0]),
    .write_data(req_wr_data_in[5*(0*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:5*(0*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  acc_1
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(4),
    .INITIAL_OUTPUT_QUEUE_ID(4),
    .NUM_INPUT_QUEUES(4),
    .NUM_OUTPUT_QUEUES(4),
    .TAG_WIDTH(TAG_WIDTH)
  )
  acc_1_4x4
  (
    .clk(clk),
    .rst(rst | rst_accs[1]),
    .start(start_accs[1]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[7:4]),
    .request_read(req_rd_en_in[7:4]),
    .request_data(req_rd_data_in[4*(1*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:4*(1*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[9:5]),
    .request_write(req_wr_en_in[9:5]),
    .write_data(req_wr_data_in[5*(1*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:5*(1*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  acc_2
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(8),
    .INITIAL_OUTPUT_QUEUE_ID(8),
    .NUM_INPUT_QUEUES(4),
    .NUM_OUTPUT_QUEUES(4),
    .TAG_WIDTH(TAG_WIDTH)
  )
  acc_2_4x4
  (
    .clk(clk),
    .rst(rst | rst_accs[2]),
    .start(start_accs[2]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[11:8]),
    .request_read(req_rd_en_in[11:8]),
    .request_data(req_rd_data_in[4*(2*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:4*(2*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[14:10]),
    .request_write(req_wr_en_in[14:10]),
    .write_data(req_wr_data_in[5*(2*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:5*(2*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  acc_3
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(12),
    .INITIAL_OUTPUT_QUEUE_ID(12),
    .NUM_INPUT_QUEUES(4),
    .NUM_OUTPUT_QUEUES(4),
    .TAG_WIDTH(TAG_WIDTH)
  )
  acc_3_4x4
  (
    .clk(clk),
    .rst(rst | rst_accs[3]),
    .start(start_accs[3]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[15:12]),
    .request_read(req_rd_en_in[15:12]),
    .request_data(req_rd_data_in[4*(3*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:4*(3*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[19:15]),
    .request_write(req_wr_en_in[19:15]),
    .write_data(req_wr_data_in[5*(3*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:5*(3*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  acc_4
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(16),
    .INITIAL_OUTPUT_QUEUE_ID(16),
    .NUM_INPUT_QUEUES(4),
    .NUM_OUTPUT_QUEUES(4),
    .TAG_WIDTH(TAG_WIDTH)
  )
  acc_4_4x4
  (
    .clk(clk),
    .rst(rst | rst_accs[4]),
    .start(start_accs[4]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[19:16]),
    .request_read(req_rd_en_in[19:16]),
    .request_data(req_rd_data_in[4*(4*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:4*(4*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[24:20]),
    .request_write(req_wr_en_in[24:20]),
    .write_data(req_wr_data_in[5*(4*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:5*(4*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  acc_5
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(20),
    .INITIAL_OUTPUT_QUEUE_ID(20),
    .NUM_INPUT_QUEUES(4),
    .NUM_OUTPUT_QUEUES(4),
    .TAG_WIDTH(TAG_WIDTH)
  )
  acc_5_4x4
  (
    .clk(clk),
    .rst(rst | rst_accs[5]),
    .start(start_accs[5]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[23:20]),
    .request_read(req_rd_en_in[23:20]),
    .request_data(req_rd_data_in[4*(5*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:4*(5*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[29:25]),
    .request_write(req_wr_en_in[29:25]),
    .write_data(req_wr_data_in[5*(5*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:5*(5*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  acc_6
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(24),
    .INITIAL_OUTPUT_QUEUE_ID(24),
    .NUM_INPUT_QUEUES(4),
    .NUM_OUTPUT_QUEUES(4),
    .TAG_WIDTH(TAG_WIDTH)
  )
  acc_6_4x4
  (
    .clk(clk),
    .rst(rst | rst_accs[6]),
    .start(start_accs[6]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[27:24]),
    .request_read(req_rd_en_in[27:24]),
    .request_data(req_rd_data_in[4*(6*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:4*(6*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[34:30]),
    .request_write(req_wr_en_in[34:30]),
    .write_data(req_wr_data_in[5*(6*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:5*(6*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  acc_7
  #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .QTD_WIDTH(QTD_WIDTH),
    .DATA_WIDTH(DATA_WIDTH),
    .CONF_ID_QUEUE_WIDTH(CONF_ID_QUEUE_WIDTH),
    .INITIAL_INPUT_QUEUE_ID(28),
    .INITIAL_OUTPUT_QUEUE_ID(28),
    .NUM_INPUT_QUEUES(4),
    .NUM_OUTPUT_QUEUES(4),
    .TAG_WIDTH(TAG_WIDTH)
  )
  acc_7_4x4
  (
    .clk(clk),
    .rst(rst | rst_accs[7]),
    .start(start_accs[7]),
    .conf_valid(conf_valid),
    .conf(conf[ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0]),
    .available_read(req_rd_available_in[31:28]),
    .request_read(req_rd_en_in[31:28]),
    .request_data(req_rd_data_in[4*(7*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:4*(7*(ADDR_WIDTH+TAG_WIDTH))]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[39:35]),
    .request_write(req_wr_en_in[39:35]),
    .write_data(req_wr_data_in[5*(7*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:5*(7*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  arbiter_controller_rd_req_tree_32
  #(
    .DATA_WIDTH(ADDR_WIDTH + TAG_WIDTH),
    .INPUT_FIFO_DEPTH_BITS(6),
    .OUTPUT_FIFO_DEPTH_BITS(6)
  )
  arbiter_controller_rd_req_tree
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_rd_en_in),
    .req_wr_data_in(req_rd_data_in),
    .req_wr_available_in(req_rd_available_in),
    .req_wr_available_out(req_rd_available_out),
    .req_wr_en_out(req_rd_en_out),
    .req_wr_data_out(req_rd_data_out)
  );


  arbiter_controller_wr_req_tree_40
  #(
    .DATA_WIDTH(DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH)
  )
  arbiter_controller_wr_req_tree
  (
    .clk(clk),
    .rst(rst),
    .req_wr_en_in(req_wr_en_in),
    .req_wr_data_in(req_wr_data_in),
    .req_wr_available_in(req_wr_available_in),
    .req_wr_available_out(req_wr_available_out),
    .req_wr_en_out(req_wr_en_out),
    .req_wr_data_out(req_wr_data_out)
  );

  assign req_rd_available_out = req_rd_available;
  assign req_wr_available_out = req_wr_available;

  always @(posedge clk) begin
    if(rst) begin
      req_rd_en <= 1'b0;
      req_rd_addr <= 0;
      req_rd_mdata <= 0;
    end else begin
      req_rd_en <= req_rd_en_out;
      req_rd_addr <= req_rd_data_out[TAG_WIDTH+ADDR_WIDTH-1:TAG_WIDTH];
      req_rd_mdata <= req_rd_data_out[TAG_WIDTH-1:0];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      req_wr_en <= 1'b0;
      req_wr_addr <= 0;
      req_wr_mdata <= 0;
      req_wr_data <= 0;
    end else begin
      req_wr_en <= req_wr_en_out;
      req_wr_data <= req_wr_data_out[DATA_WIDTH+TAG_WIDTH+ADDR_WIDTH-1:TAG_WIDTH+ADDR_WIDTH];
      req_wr_addr <= req_wr_data_out[TAG_WIDTH+ADDR_WIDTH-1:TAG_WIDTH];
      req_wr_mdata <= req_wr_data_out[TAG_WIDTH-1:0];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      info <= 512'd0;
    end else begin
      info[15:0] <= {8'd4, 8'd4};
      info[31:16] <= {8'd4, 8'd4};
      info[47:32] <= {8'd4, 8'd4};
      info[63:48] <= {8'd4, 8'd4};
      info[79:64] <= {8'd4, 8'd4};
      info[95:80] <= {8'd4, 8'd4};
      info[111:96] <= {8'd4, 8'd4};
      info[127:112] <= {8'd4, 8'd4};

    end
  end


endmodule
