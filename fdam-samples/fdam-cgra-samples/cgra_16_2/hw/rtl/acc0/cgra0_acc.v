module cgra0_acc
(
  input clk,
  input rst,
  input start,
  input [2-1:0] acc_user_done_rd_data,
  input [2-1:0] acc_user_done_wr_data,
  input [2-1:0] acc_user_available_read,
  output [2-1:0] acc_user_request_read,
  input [2-1:0] acc_user_read_data_valid,
  input [1024-1:0] acc_user_read_data,
  input [2-1:0] acc_user_available_write,
  output [2-1:0] acc_user_request_write,
  output [1024-1:0] acc_user_write_data,
  output acc_user_done
);

  wire [2-1:0] request_read;
  wire conf_control_req_rd_data;
  wire [2-1:0] en_fecth_data;
  wire [2-1:0] en_dispath_data;
  wire [16-1:0] en_pe;
  wire [128-1:0] en_net;
  wire [24-1:0] en_pc_net;
  wire [2-1:0] fifo_in_re;
  wire [32-1:0] fifo_in_data;
  wire [2-1:0] available_pop;
  wire [2-1:0] fifo_out_we;
  wire [32-1:0] fifo_out_data;
  wire [2-1:0] available_push;
  wire [64-1:0] conf_out_bus;
  wire [2-1:0] read_fifo_mask;
  wire [2-1:0] write_fifo_mask;
  wire conf_done;
  genvar genv;
  assign acc_user_request_read[1:1] = request_read[1:1];
  assign acc_user_request_read[0] = request_read[0] | conf_control_req_rd_data;

  generate for(genv=0; genv<2; genv=genv+1) begin : inst_fecth_data

    fecth_data
    #(
      .INPUT_DATA_WIDTH(512),
      .OUTPUT_DATA_WIDTH(16)
    )
    fecth_data
    (
      .clk(clk),
      .rst(rst),
      .en(en_fecth_data[genv]),
      .available_read(acc_user_available_read[genv]),
      .request_read(request_read[genv]),
      .data_valid(acc_user_read_data_valid[genv]),
      .read_data(acc_user_read_data[(genv+1)*512-1:genv*512]),
      .pop_data(fifo_in_re[genv]),
      .available_pop(available_pop[genv]),
      .data_out(fifo_in_data[(genv+1)*16-1:genv*16])
    );

  end
  endgenerate


  generate for(genv=0; genv<2; genv=genv+1) begin : inst_dispath_data

    dispath_data
    #(
      .INPUT_DATA_WIDTH(16),
      .OUTPUT_DATA_WIDTH(512)
    )
    dispath_data
    (
      .clk(clk),
      .rst(rst),
      .en(en_dispath_data[genv]),
      .available_write(acc_user_available_write[genv]),
      .request_write(acc_user_request_write[genv]),
      .write_data(acc_user_write_data[(genv+1)*512-1:genv*512]),
      .push_data(fifo_out_we[genv]),
      .available_push(available_push[genv]),
      .data_in(fifo_out_data[(genv+1)*16-1:genv*16])
    );

  end
  endgenerate


  cgra0_control_conf
  control_conf
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .available_read(acc_user_available_read[0]),
    .req_rd_data(conf_control_req_rd_data),
    .rd_data(acc_user_read_data[511:0]),
    .rd_data_valid(acc_user_read_data_valid[0]),
    .conf_out_bus(conf_out_bus),
    .read_fifo_mask(read_fifo_mask),
    .write_fifo_mask(write_fifo_mask),
    .done(conf_done)
  );


  cgra0_control_exec
  control_exec
  (
    .clk(clk),
    .rst(rst),
    .start(conf_done),
    .read_fifo_mask(read_fifo_mask),
    .write_fifo_mask(write_fifo_mask),
    .available_read(acc_user_available_read),
    .available_write(acc_user_available_write),
    .available_pop(available_pop),
    .available_push(available_push),
    .read_fifo_done(acc_user_done_rd_data),
    .write_fifo_done(acc_user_done_wr_data),
    .en_pe(en_pe),
    .en_net(en_net),
    .en_pc_net(en_pc_net),
    .en_fecth_data(en_fecth_data),
    .en_dispath_data(en_dispath_data),
    .done(acc_user_done)
  );


  cgra0_top
  cgra
  (
    .clk(clk),
    .rst(rst),
    .pes_en(en_pe),
    .net_en(en_net),
    .en_pc_net(en_pc_net),
    .conf_bus_in(conf_out_bus),
    .fifo_in_re(fifo_in_re),
    .fifo_in_data(fifo_in_data),
    .fifo_out_we(fifo_out_we),
    .fifo_out_data(fifo_out_data)
  );

endmodule