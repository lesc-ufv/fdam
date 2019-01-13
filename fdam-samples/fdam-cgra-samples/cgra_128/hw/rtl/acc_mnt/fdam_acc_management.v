module fdam_acc_management
(
  input clk,
  input rst,
  input [64-1:0] rst_accs,
  input [64-1:0] start_accs,
  input [2-1:0] conf_valid,
  input [128-1:0] conf,
  output reg req_rd_en,
  output reg [48-1:0] req_rd_addr,
  output reg [16-1:0] req_rd_mdata,
  input req_rd_available,
  input resp_rd_valid,
  input [512-1:0] resp_rd_data,
  input [16-1:0] resp_rd_mdata,
  input req_wr_available,
  output reg req_wr_en,
  output reg [48-1:0] req_wr_addr,
  output reg [16-1:0] req_wr_mdata,
  output reg [512-1:0] req_wr_data,
  input resp_wr_valid,
  input [16-1:0] resp_wr_mdata,
  output reg [512-1:0] info
);


  wire [9-1:0] req_wr_en_in;
  wire [5184-1:0] req_wr_data_in;
  wire [9-1:0] req_wr_available_in;

  wire req_wr_available_out;
  wire req_wr_en_out;
  wire [576-1:0] req_wr_data_out;

  wire [8-1:0] req_rd_en_in;
  wire [512-1:0] req_rd_data_in;
  wire [8-1:0] req_rd_available_in;

  wire req_rd_available_out;
  wire req_rd_en_out;
  wire [64-1:0] req_rd_data_out;

  wire [6-1:0] rst_reg;

  reg_tree_4_6_2
  #(
    .DATA_WIDTH(1)
  )
  rst_reg_tree_4_6_2
  (
    .clk(clk),
    .in(rst),
    .out_0(rst_reg[0]),
    .out_1(rst_reg[1]),
    .out_2(rst_reg[2]),
    .out_3(rst_reg[3]),
    .out_4(rst_reg[4]),
    .out_5(rst_reg[5])
  );


  fdam_accelerator_0
  acc_0_8x8
  (
    .clk(clk),
    .rst(rst_reg[0] | rst_accs[0]),
    .start(start_accs[0]),
    .conf_valid(conf_valid),
    .conf(conf[111:0]),
    .available_read(req_rd_available_in[7:0]),
    .request_read(req_rd_en_in[7:0]),
    .request_data(req_rd_data_in[511:0]),
    .read_data_valid(resp_rd_valid),
    .read_queue_id(resp_rd_mdata),
    .read_data(resp_rd_data),
    .available_write(req_wr_available_in[8:0]),
    .request_write(req_wr_en_in[8:0]),
    .write_data(req_wr_data_in[5183:0]),
    .write_data_valid(resp_wr_valid),
    .write_queue_id(resp_wr_mdata)
  );


  fdam_arbiter_controller_rd_req_tree_8
  #(
    .DATA_WIDTH(64),
    .INPUT_FIFO_DEPTH_BITS(5),
    .OUTPUT_FIFO_DEPTH_BITS(5)
  )
  arbiter_controller_rd_req_tree
  (
    .clk(clk),
    .rst(rst_reg[1]),
    .req_wr_en_in(req_rd_en_in),
    .req_wr_data_in(req_rd_data_in),
    .req_wr_available_in(req_rd_available_in),
    .req_wr_available_out(req_rd_available_out),
    .req_wr_en_out(req_rd_en_out),
    .req_wr_data_out(req_rd_data_out)
  );


  fdam_arbiter_controller_wr_req_tree_9
  #(
    .DATA_WIDTH(576),
    .INPUT_FIFO_DEPTH_BITS(5),
    .OUTPUT_FIFO_DEPTH_BITS(5)
  )
  arbiter_controller_wr_req_tree
  (
    .clk(clk),
    .rst(rst_reg[2]),
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
    if(rst_reg[3]) begin
      req_rd_en <= 1'b0;
    end else begin
      req_rd_en <= req_rd_en_out;
      req_rd_addr <= req_rd_data_out[63:16];
      req_rd_mdata <= req_rd_data_out[15:0];
    end
  end


  always @(posedge clk) begin
    if(rst_reg[4]) begin
      req_wr_en <= 1'b0;
    end else begin
      req_wr_en <= req_wr_en_out;
      req_wr_data <= req_wr_data_out[575:64];
      req_wr_addr <= req_wr_data_out[63:16];
      req_wr_mdata <= req_wr_data_out[15:0];
    end
  end


  always @(posedge clk) begin
    if(rst_reg[5]) begin
      info <= 512'd0;
    end else begin
      info[15:0] <= {8'd8, 8'd8};

    end
  end

endmodule