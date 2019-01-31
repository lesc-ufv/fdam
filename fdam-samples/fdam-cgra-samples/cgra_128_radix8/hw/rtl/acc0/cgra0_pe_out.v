module cgra0_pe_out #
(
  parameter PE_ID = 0
)
(
  input clk,
  input rst,
  input en,
  input branch_in,
  output branch_out,
  input [64-1:0] conf_bus_in,
  input [16-1:0] ina,
  input [16-1:0] inb,
  output [16-1:0] outa,
  output [16-1:0] outb,
  output [16-1:0] fifo_data,
  output fifo_we
);

  genvar genv;
  wire en_reg;
  wire [4-1:0] alu_op_inst_dec;
  wire [4-1:0] alu_op;
  wire [16-1:0] mux_b_in;
  wire mux_a_sel_inst_dec;
  wire mux_b_sel_inst_dec;
  wire mux_b_sel;
  wire rf_we_inst_dec;
  wire rf_we;
  wire rf_we_p;
  wire [4-1:0] rf_waddr_inst_dec;
  wire [4-1:0] rf_waddr;
  wire [16-1:0] rf_data_out;
  wire rf_re_inst_dec;
  wire [4-1:0] rf_raddr_inst_dec;
  wire [16-1:0] rf_data_in;
  wire fifo_we_inst_dec;
  wire fifo_re_inst_dec;
  wire out_a_en_inst_dec;
  wire out_a_en;
  wire out_b_en_inst_dec;
  wire out_b_en;
  wire [8-1:0] pc_max_mem;
  wire pc_max_we;
  wire [8-1:0] pc_loop_mem;
  wire pc_loop_we;
  wire [8-1:0] ignore_mem;
  wire ignore_we;
  wire qtd_we_low;
  wire [32-1:0] qtd_mem_low;
  wire qtd_we_high;
  wire [32-1:0] qtd_mem_high;
  wire [3-1:0] thread_id;
  wire [8-1:0] thread_id_dec;
  wire [3-1:0] thread_idx;
  wire [3-1:0] thread_idx_p;
  wire [3-1:0] thread_idx_pp;
  wire [3-1:0] thread_idx_pppp;
  wire [8-1:0] thread_idx_dec;
  wire [8-1:0] conf_wr_addr;
  wire [16-1:0] conf_wr_data;
  wire conf_wr_en;
  wire init_const_we;
  wire [4-1:0] init_conf_const_waddr;
  wire [16-1:0] init_conf_const;
  wire [16-1:0] inst_mem_out;
  wire branch_in_alu;
  wire branch_out_alu;
  wire [8-1:0] pc_max [0:8-1];
  wire [8-1:0] pc_loop [0:8-1];
  wire [8-1:0] ignore [0:8-1];
  wire [8-1:0] pc_out [0:8-1];
  wire [8-1:0] mux_pc_out;
  wire [16-1:0] reg_alu_in_a_out;
  wire [16-1:0] reg_alu_in_b_out;
  wire [16-1:0] alu_out;
  wire [16-1:0] mux_rf_const_out;
  wire [7-1:0] mux_rf_const_waddr_out;
  wire fifo_we_ignore_en;
  wire ignore_end_counter;
  wire [8-1:0] ignore_end_counters;
  wire we_trash;
  wire [8-1:0] thread_idx_dec_pppp;
  wire [32-1:0] qtd_low [0:8-1];
  wire [32-1:0] qtd_high [0:8-1];
  wire [8-1:0] qtd_end_counters;

  assign fifo_we = (fifo_we_ignore_en && ignore_end_counter && en) | we_trash;
  assign we_trash = &qtd_end_counters & en;

  reg_pipe
  #(
    .NUM_STAGES(4),
    .DATA_WIDTH(8)
  )
  reg_thread_idx_dec_ppp
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(thread_idx_dec),
    .out(thread_idx_dec_pppp)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  reg_fifo_data
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(alu_out),
    .out(fifo_data)
  );


  reg_pipe
  #(
    .NUM_STAGES(3),
    .DATA_WIDTH(1)
  )
  reg_fifo_we_ignore_en
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(fifo_we_inst_dec),
    .out(fifo_we_ignore_en)
  );


  generate for(genv=0; genv<8; genv=genv+1) begin : ignore_reg_inst

    reg_pipe
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(8)
    )
    ignore_reg
    (
      .clk(clk),
      .rst(rst),
      .en(thread_id_dec[genv] & ignore_we),
      .in(ignore_mem),
      .out(ignore[genv])
    );

  end
  endgenerate


  generate for(genv=0; genv<8; genv=genv+1) begin : ignore_counter_inst

    ignore_counter
    #(
      .WIDTH(8)
    )
    ignore_counter
    (
      .clk(clk),
      .rst(rst),
      .en(thread_idx_dec_pppp[genv] & fifo_we_ignore_en & en),
      .limit(ignore[genv]),
      .end_counter(ignore_end_counters[genv])
    );

  end
  endgenerate


  generate for(genv=0; genv<8; genv=genv+1) begin : qtd_low_reg_inst

    reg_pipe
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(32)
    )
    qtd_low_reg
    (
      .clk(clk),
      .rst(rst),
      .en(thread_id_dec[genv] & qtd_we_low),
      .in(qtd_mem_low),
      .out(qtd_low[genv])
    );

  end
  endgenerate


  generate for(genv=0; genv<8; genv=genv+1) begin : qtd_high_reg_inst

    reg_pipe
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(32)
    )
    qtd_high_reg
    (
      .clk(clk),
      .rst(rst),
      .en(thread_id_dec[genv] & qtd_we_high),
      .in(qtd_mem_high),
      .out(qtd_high[genv])
    );

  end
  endgenerate


  generate for(genv=0; genv<8; genv=genv+1) begin : qtd_counter_inst

    ignore_counter
    #(
      .WIDTH(64)
    )
    qtd_counter
    (
      .clk(clk),
      .rst(rst),
      .en(thread_idx_dec_pppp[genv] & fifo_we),
      .limit({ qtd_high[genv], qtd_low[genv] }),
      .end_counter(qtd_end_counters[genv])
    );

  end
  endgenerate


  mux8x1
  #(
    .WIDTH(1)
  )
  mux_ignore_end_counter
  (
    .sel(thread_idx_pppp),
    .out(ignore_end_counter),
    .in0(ignore_end_counters[0] & ~qtd_end_counters[0]),
    .in1(ignore_end_counters[1] & ~qtd_end_counters[1]),
    .in2(ignore_end_counters[2] & ~qtd_end_counters[2]),
    .in3(ignore_end_counters[3] & ~qtd_end_counters[3]),
    .in4(ignore_end_counters[4] & ~qtd_end_counters[4]),
    .in5(ignore_end_counters[5] & ~qtd_end_counters[5]),
    .in6(ignore_end_counters[6] & ~qtd_end_counters[6]),
    .in7(ignore_end_counters[7] & ~qtd_end_counters[7])
  );

  assign thread_idx_dec = 1 << thread_idx;
  assign thread_id_dec = 1 << thread_id;

  generate for(genv=0; genv<8; genv=genv+1) begin : pc_max_reg_inst

    reg_pipe
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(8)
    )
    pc_max_reg
    (
      .clk(clk),
      .rst(rst),
      .en(thread_id_dec[genv] & pc_max_we),
      .in(pc_max_mem),
      .out(pc_max[genv])
    );

  end
  endgenerate


  generate for(genv=0; genv<8; genv=genv+1) begin : pc_loop_reg_inst

    reg_pipe
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(8)
    )
    pc_loop_reg
    (
      .clk(clk),
      .rst(rst),
      .en(thread_id_dec[genv] & pc_loop_we),
      .in(pc_loop_mem),
      .out(pc_loop[genv])
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
    .out(mux_pc_out),
    .in0(pc_out[0]),
    .in1(pc_out[1]),
    .in2(pc_out[2]),
    .in3(pc_out[3]),
    .in4(pc_out[4]),
    .in5(pc_out[5]),
    .in6(pc_out[6]),
    .in7(pc_out[7])
  );


  generate for(genv=0; genv<8; genv=genv+1) begin : pc_inst

    program_counter_8
    pc
    (
      .clk(clk),
      .rst(rst),
      .en(thread_idx_dec[genv] & en),
      .max(pc_max[genv]),
      .loop(pc_loop[genv]),
      .pc(pc_out[genv])
    );

  end
  endgenerate


  memory
  #(
    .DATA_WIDTH(16),
    .ADDR_WIDTH(11)
  )
  inst_mem
  (
    .clk(clk),
    .we(conf_wr_en),
    .re(en),
    .raddr({ thread_idx, mux_pc_out }),
    .waddr({ thread_id, conf_wr_addr }),
    .din(conf_wr_data),
    .dout(inst_mem_out)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(1)
  )
  reg_rf_we_p
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(rf_we),
    .out(rf_we_p)
  );


  reg_pipe
  #(
    .NUM_STAGES(3),
    .DATA_WIDTH(4)
  )
  reg_rf_waddr
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(rf_waddr_inst_dec),
    .out(rf_waddr)
  );


  mux2x1
  #(
    .WIDTH(7)
  )
  mux_rf_const_waddr
  (
    .sel(init_const_we),
    .in0({ thread_idx_pppp, rf_waddr }),
    .in1({ thread_id, init_conf_const_waddr }),
    .out(mux_rf_const_waddr_out)
  );


  memory
  #(
    .DATA_WIDTH(16),
    .ADDR_WIDTH(7)
  )
  rf
  (
    .clk(clk),
    .we(rf_we_p & en | init_const_we),
    .re(rf_re_inst_dec),
    .raddr({ thread_idx_p, rf_raddr_inst_dec }),
    .waddr(mux_rf_const_waddr_out),
    .din(mux_rf_const_out),
    .dout(rf_data_out)
  );


  mux2x1
  #(
    .WIDTH(16)
  )
  mux_rf_const
  (
    .sel(init_const_we),
    .in0(rf_data_in),
    .in1(init_conf_const),
    .out(mux_rf_const_out)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(1)
  )
  reg_mux_b_sel
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(mux_b_sel_inst_dec),
    .out(mux_b_sel)
  );


  mux2x1
  #(
    .WIDTH(16)
  )
  mux_b
  (
    .sel(mux_b_sel),
    .in0(mux_b_in),
    .in1(rf_data_out),
    .out(reg_alu_in_b_out)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  reg_alu_in_a
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(ina),
    .out(reg_alu_in_a_out)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  reg_alu_in_b
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(inb),
    .out(mux_b_in)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(4)
  )
  reg_alu_op
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(alu_op_inst_dec),
    .out(alu_op)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(1)
  )
  reg_branch_in
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(branch_in),
    .out(branch_in_alu)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(1)
  )
  reg_branch_out
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(branch_out_alu),
    .out(branch_out)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(3)
  )
  reg_thread_idx_pp
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(thread_idx_p),
    .out(thread_idx_pp)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(3)
  )
  reg_thread_idx_p
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(thread_idx),
    .out(thread_idx_p)
  );


  reg_pipe
  #(
    .NUM_STAGES(2),
    .DATA_WIDTH(3)
  )
  reg_thread_idx_pppp
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(thread_idx_pp),
    .out(thread_idx_pppp)
  );


  cgra0_alu
  #(
    .DATA_WIDTH(16)
  )
  alu
  (
    .clk(clk),
    .op(alu_op),
    .branch_in(branch_in_alu),
    .branch_out(branch_out_alu),
    .ina(reg_alu_in_a_out),
    .inb(reg_alu_in_b_out),
    .out(alu_out)
  );


  reg_pipe
  #(
    .NUM_STAGES(2),
    .DATA_WIDTH(1)
  )
  reg_out_a_en
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(out_a_en_inst_dec),
    .out(out_a_en)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  reg_out_a
  (
    .clk(clk),
    .rst(rst),
    .en(out_a_en & en),
    .in(alu_out),
    .out(outa)
  );


  reg_pipe
  #(
    .NUM_STAGES(2),
    .DATA_WIDTH(1)
  )
  reg_out_b_en
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(out_b_en_inst_dec),
    .out(out_b_en)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  reg_out_b
  (
    .clk(clk),
    .rst(rst),
    .en(out_b_en & en),
    .in(alu_out),
    .out(outb)
  );


  reg_pipe
  #(
    .NUM_STAGES(2),
    .DATA_WIDTH(1)
  )
  reg_rf_we_en
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(rf_we_inst_dec),
    .out(rf_we)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  reg_rf_data_in
  (
    .clk(clk),
    .rst(rst),
    .en(rf_we),
    .in(alu_out),
    .out(rf_data_in)
  );


  cgra0_conf_reader_pe
  #(
    .PE_ID(PE_ID)
  )
  conf_reader_pe
  (
    .clk(clk),
    .rst(rst),
    .conf_bus_in(conf_bus_in),
    .instruction_we(conf_wr_en),
    .instruction_addr(conf_wr_addr),
    .instruction_data(conf_wr_data),
    .const_we(init_const_we),
    .const_waddr(init_conf_const_waddr),
    .const_data(init_conf_const),
    .pc_max(pc_max_mem),
    .pc_max_we(pc_max_we),
    .pc_loop(pc_loop_mem),
    .pc_loop_we(pc_loop_we),
    .ignore_data(ignore_mem),
    .ignore_we(ignore_we),
    .qtd_low(qtd_mem_low),
    .qtd_we_low(qtd_we_low),
    .qtd_high(qtd_mem_high),
    .qtd_we_high(qtd_we_high),
    .thread_id(thread_id)
  );


  thread_counter_8
  thread_counter
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .out(thread_idx)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(1)
  )
  reg_en
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(en),
    .out(en_reg)
  );


  cgra0_inst_decode
  inst_decode
  (
    .instruction((en_reg)? inst_mem_out : 16'd0),
    .alu_op(alu_op_inst_dec),
    .mux_a(mux_a_sel_inst_dec),
    .mux_b(mux_b_sel_inst_dec),
    .rf_re(rf_re_inst_dec),
    .rf_we(rf_we_inst_dec),
    .fifo_re(fifo_re_inst_dec),
    .fifo_we(fifo_we_inst_dec),
    .out_a_en(out_a_en_inst_dec),
    .out_b_en(out_b_en_inst_dec),
    .rf_raddr(rf_raddr_inst_dec),
    .rf_waddr(rf_waddr_inst_dec)
  );

endmodule