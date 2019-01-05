module swicth_conf_control_8_8 #
(
  parameter SWICTH_NUMBER = 0,
  parameter STAGE = 1
)
(
  input clk,
  input rst,
  input en_pc_net,
  input [64-1:0] conf_bus_in,
  output [8-1:0] swicth_conf_out
);

  wire [8-1:0] net_pc_max [0:8-1];
  wire [8-1:0] net_pc_max_mem;
  wire net_pc_max_we;
  wire [8-1:0] net_pc_loop [0:8-1];
  wire [8-1:0] net_pc_loop_mem;
  wire net_pc_loop_we;
  wire [8-1:0] net_pc_out [0:8-1];
  wire [8-1:0] net_conf_mem_raddr;
  wire net_mem_we;
  wire [8-1:0] net_mem_waddr;
  wire [8-1:0] net_mem_din;
  wire [8-1:0] net_mem_dout;
  wire [3-1:0] thread_id;
  wire [8-1:0] thread_id_dec;
  wire [3-1:0] thread_idx;
  wire [8-1:0] thread_idx_dec;
  genvar genv;
  assign thread_idx_dec = 1 << thread_idx;
  assign thread_id_dec = 1 << thread_id;

  swicth_conf_reader_8_8
  #(
    .SWICTH_NUMBER(SWICTH_NUMBER)
  )
  swicth_conf_reader
  (
    .clk(clk),
    .rst(rst),
    .conf_bus_in(conf_bus_in),
    .pc_max(net_pc_max_mem),
    .pc_max_we(net_pc_max_we),
    .pc_loop(net_pc_loop_mem),
    .pc_loop_we(net_pc_loop_we),
    .net_mem_we(net_mem_we),
    .net_mem_waddr(net_mem_waddr),
    .net_mem_data(net_mem_din),
    .thread_id(thread_id)
  );


  generate for(genv=0; genv<8; genv=genv+1) begin : pc_inst

    program_counter_8
    pc
    (
      .clk(clk),
      .rst(rst),
      .en(thread_idx_dec[genv] & en_pc_net),
      .max(net_pc_max[genv]),
      .loop(net_pc_loop[genv]),
      .pc(net_pc_out[genv])
    );

  end
  endgenerate


  mux8x1
  #(
    .WIDTH(8)
  )
  mux_pc
  (
    .sel(thread_idx),
    .out(net_conf_mem_raddr),
    .in0(net_pc_out[0]),
    .in1(net_pc_out[1]),
    .in2(net_pc_out[2]),
    .in3(net_pc_out[3]),
    .in4(net_pc_out[4]),
    .in5(net_pc_out[5]),
    .in6(net_pc_out[6]),
    .in7(net_pc_out[7])
  );


  memory
  #(
    .DATA_WIDTH(8),
    .ADDR_WIDTH(11)
  )
  mem_conf
  (
    .clk(clk),
    .we(net_mem_we),
    .re(en_pc_net),
    .raddr({ thread_idx, net_conf_mem_raddr }),
    .waddr({ thread_id, net_mem_waddr }),
    .din(net_mem_din),
    .dout(net_mem_dout)
  );


  generate for(genv=0; genv<8; genv=genv+1) begin : pc_max_inst

    reg_pipe
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(8)
    )
    pc_max
    (
      .clk(clk),
      .rst(rst),
      .en(thread_id_dec[genv] & net_pc_max_we),
      .in(net_pc_max_mem),
      .out(net_pc_max[genv])
    );

  end
  endgenerate


  generate for(genv=0; genv<8; genv=genv+1) begin : pc_loop_inst

    reg_pipe
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(8)
    )
    pc_loop
    (
      .clk(clk),
      .rst(rst),
      .en(thread_id_dec[genv] & net_pc_loop_we),
      .in(net_pc_loop_mem),
      .out(net_pc_loop[genv])
    );

  end
  endgenerate


  thread_counter_8
  thread_counter
  (
    .clk(clk),
    .rst(rst),
    .en(en_pc_net),
    .out(thread_idx)
  );


  reg_pipe
  #(
    .NUM_STAGES(STAGE + 3),
    .DATA_WIDTH(8)
  )
  reg_net_conf_out
  (
    .clk(clk),
    .rst(1'b0),
    .en(en_pc_net),
    .in(net_mem_dout),
    .out(swicth_conf_out)
  );

endmodule