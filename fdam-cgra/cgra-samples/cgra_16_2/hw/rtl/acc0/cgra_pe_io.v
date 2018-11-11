module cgra_pe_io #
(
  parameter DATA_WIDTH = 16,
  parameter PE_ID = 0
)
(
  input clk,
  input rst,
  input en,
  input [21-1:0] conf_bus_in,
  output [21-1:0] conf_bus_out,
  input [45-1:0] init_conf_bus_in,
  output [45-1:0] init_conf_bus_out,
  input [DATA_WIDTH-1:0] ina,
  input [DATA_WIDTH-1:0] inb,
  output [DATA_WIDTH-1:0] outa,
  output [DATA_WIDTH-1:0] outb,
  input [DATA_WIDTH-1:0] fifo_in_data,
  output fifo_in_re,
  output [DATA_WIDTH-1:0] fifo_out_data,
  output fifo_out_we
);

  wire [DATA_WIDTH-1:0] alu_out;
  wire [2-1:0] alu_out_addr;
  wire [2-1:0] alu_out_addr_reg;
  wire [DATA_WIDTH-1:0] alu_ina;
  wire [DATA_WIDTH-1:0] alu_ina_reg;
  wire [2-1:0] alu_ina_addr;
  wire [2-1:0] alu_ina_addr_reg;
  wire [DATA_WIDTH-1:0] alu_inb;
  wire [DATA_WIDTH-1:0] alu_inb_reg;
  wire [2-1:0] alu_inb_addr;
  wire [2-1:0] alu_inb_addr_reg;
  wire [4-1:0] alu_op;
  wire [4-1:0] alu_op_reg;
  wire [3-1:0] rf_raddr;
  wire [3-1:0] rf_raddr_reg;
  wire [3-1:0] rf_waddr;
  wire [3-1:0] rf_waddr_reg;
  wire [DATA_WIDTH-1:0] rf_data_in;
  wire [DATA_WIDTH-1:0] rf_data_out;
  wire [DATA_WIDTH-1:0] rf_data_out_reg;
  wire [DATA_WIDTH-1:0] mux_rf_const_out;
  wire [3-1:0] mux_rf_const_waddr_out;
  wire rf_const_end_counter;

  wire [8-1:0] conf_wr_addr;
  wire [16-1:0] conf_wr_data;
  wire conf_wr_en;

  wire init_conf_we;
  wire [3-1:0] init_conf_const_waddr;
  wire [16-1:0] init_conf_const;
  wire [8-1:0] pc_max;
  wire [8-1:0] pc_loop;
  wire [8-1:0] pc_out;
  wire mem_re;
  wire [8-1:0] ignore_until;

  wire [DATA_WIDTH-1:0] ina_reg;
  wire [DATA_WIDTH-1:0] inb_reg;
  wire [16-1:0] inst_mem_out;
  wire [4-1:0] alu_out_decode_out;
  wire rf_we;
  wire rf_rd_en;
  wire [DATA_WIDTH-1:0] fifo_in_data_reg;
  wire ignore_until_en;
  wire ignore_counter_en;
  wire [8-1:0] ignore_counter_out;

  assign fifo_in_re = ((alu_ina_addr_reg == 2'd3) || (alu_inb_addr_reg == 2'd3)) && en;
  assign fifo_out_we = (alu_out_addr_reg == 2'd3) && en && ignore_until_en;
  assign ignore_counter_en = (alu_out_addr_reg == 2'd3) && en;
  assign rf_rd_en = ((alu_ina_addr_reg == 2'd2) || (alu_inb_addr_reg == 2'd2)) && en;

  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  fifo_in_data_reg_inst
  (
    .clk(clk),
    .en(fifo_in_re),
    .in(fifo_in_data),
    .out(fifo_in_data_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  fifo_out_data_reg_inst
  (
    .clk(clk),
    .en(en & alu_out_decode_out[3]),
    .in(alu_out),
    .out(fifo_out_data)
  );


  cgra_counter
  #(
    .WIDTH(8)
  )
  ignore_counter
  (
    .clk(clk),
    .rst(rst),
    .en(ignore_counter_en),
    .limit(ignore_until),
    .end_counter(ignore_until_en),
    .out(ignore_counter_out)
  );


  cgra_mux4x1
  #(
    .WIDTH(DATA_WIDTH)
  )
  mux_alu_ina
  (
    .sel(alu_ina_addr_reg),
    .in0(ina_reg),
    .in1(inb_reg),
    .in2(rf_data_out_reg),
    .in3(fifo_in_data_reg),
    .out(alu_ina)
  );


  cgra_mux4x1
  #(
    .WIDTH(DATA_WIDTH)
  )
  mux_alu_inb
  (
    .sel(alu_inb_addr_reg),
    .in0(ina_reg),
    .in1(inb_reg),
    .in2(rf_data_out_reg),
    .in3(fifo_in_data_reg),
    .out(alu_inb)
  );


  cgra_alu_out_decode_io
  alu_out_decode
  (
    .sel(alu_out_addr_reg),
    .out(alu_out_decode_out)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  ina_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(ina),
    .out(ina_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  inb_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(inb),
    .out(inb_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  outa_reg_inst
  (
    .clk(clk),
    .en(en & alu_out_decode_out[0]),
    .in(alu_out),
    .out(outa)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  outb_reg_inst
  (
    .clk(clk),
    .en(en & alu_out_decode_out[1]),
    .in(alu_out),
    .out(outb)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  alu_ina_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(alu_ina),
    .out(alu_ina_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(2)
  )
  alu_ina_addr_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(alu_ina_addr),
    .out(alu_ina_addr_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  alu_inb_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(alu_inb),
    .out(alu_inb_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(2)
  )
  alu_inb_addr_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(alu_inb_addr),
    .out(alu_inb_addr_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(4),
    .DATA_WIDTH(2)
  )
  alu_out_addr_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(alu_out_addr),
    .out(alu_out_addr_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  rf_data_in_reg_inst
  (
    .clk(clk),
    .en(en & alu_out_decode_out[2]),
    .in(alu_out),
    .out(rf_data_in)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(1)
  )
  rf_we_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(alu_out_decode_out[2]),
    .out(rf_we)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(DATA_WIDTH)
  )
  rf_data_out_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(rf_data_out),
    .out(rf_data_out_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(2),
    .DATA_WIDTH(4)
  )
  alu_op_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(alu_op),
    .out(alu_op_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(3)
  )
  rf_raddr_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(rf_raddr),
    .out(rf_raddr_reg)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(5),
    .DATA_WIDTH(3)
  )
  rf_waddr_reg_inst
  (
    .clk(clk),
    .en(en),
    .in(rf_waddr),
    .out(rf_waddr_reg)
  );


  cgra_mux2x1
  #(
    .WIDTH(DATA_WIDTH)
  )
  mux_rf_const
  (
    .sel(init_conf_we),
    .in0(rf_data_in),
    .in1(init_conf_const),
    .out(mux_rf_const_out)
  );


  cgra_mux2x1
  #(
    .WIDTH(3)
  )
  mux_rf_const_waddr
  (
    .sel(init_conf_we),
    .in0(rf_waddr_reg),
    .in1(init_conf_const_waddr),
    .out(mux_rf_const_waddr_out)
  );


  cgra_counter
  #(
    .WIDTH(3)
  )
  rf_const_waddr_count
  (
    .clk(clk),
    .rst(rst),
    .en(init_conf_we),
    .limit(3'd7),
    .end_counter(rf_const_end_counter),
    .out(init_conf_const_waddr)
  );


  cgra_memory
  #(
    .DATA_WIDTH(DATA_WIDTH),
    .ADDR_WIDTH(3)
  )
  rf
  (
    .clk(clk),
    .we(rf_we | init_conf_we),
    .re(rf_rd_en),
    .raddr(rf_raddr_reg),
    .waddr(mux_rf_const_waddr_out),
    .din(mux_rf_const_out),
    .dout(rf_data_out)
  );


  cgra_alu
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  alu
  (
    .clk(clk),
    .op(alu_op_reg),
    .ina(alu_ina_reg),
    .inb(alu_inb_reg),
    .out(alu_out)
  );


  cgra_memory
  #(
    .DATA_WIDTH(16),
    .ADDR_WIDTH(8)
  )
  inst_mem
  (
    .clk(clk),
    .we(conf_wr_en),
    .re(mem_re),
    .raddr(pc_out),
    .waddr(conf_wr_addr),
    .din(conf_wr_data),
    .dout(inst_mem_out)
  );


  cgra_conf_reader
  #(
    .PE_ID(PE_ID)
  )
  conf_reader
  (
    .clk(clk),
    .rst(rst),
    .conf_in(conf_bus_in),
    .conf_out(conf_bus_out),
    .conf_wr_addr(conf_wr_addr),
    .conf_wr_data(conf_wr_data),
    .conf_wr_en(conf_wr_en)
  );


  cgra_program_counter
  pc
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .max(pc_max),
    .loop(pc_loop),
    .pc(pc_out),
    .pc_en(mem_re)
  );


  cgra_initial_conf
  #(
    .PE_ID(PE_ID)
  )
  init_conf
  (
    .clk(clk),
    .rst(rst),
    .initial_conf_in(init_conf_bus_in),
    .initial_conf_out(init_conf_bus_out),
    .init_conf_we(init_conf_we),
    .init_const(init_conf_const),
    .pc_max(pc_max),
    .pc_loop(pc_loop),
    .ignore_until(ignore_until)
  );


  cgra_inst_decode
  inst_decode
  (
    .instruction(inst_mem_out),
    .alu_op(alu_op),
    .alu_a(alu_ina_addr),
    .alu_b(alu_inb_addr),
    .alu_out(alu_out_addr),
    .rf_raddr(rf_raddr),
    .rf_waddr(rf_waddr)
  );

endmodule