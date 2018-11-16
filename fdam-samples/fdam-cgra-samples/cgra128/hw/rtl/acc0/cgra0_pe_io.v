module cgra0_pe_io #
(
  parameter PE_ID = 0
)
(
  input clk,
  input rst,
  input en,
  input [64-1:0] conf_bus_in,
  input [16-1:0] ina,
  input [16-1:0] inb,
  output [16-1:0] outa,
  output [16-1:0] outb,
  input [16-1:0] fifo_in_data,
  output fifo_in_re,
  output [16-1:0] fifo_out_data,
  output fifo_out_we
);

  wire [16-1:0] alu_out;
  wire [2-1:0] alu_out_addr;
  wire [2-1:0] alu_out_addr_reg;
  wire [16-1:0] alu_ina;
  wire [16-1:0] alu_ina_reg;
  wire [2-1:0] alu_ina_addr;
  wire [2-1:0] alu_ina_addr_reg;
  wire [16-1:0] alu_inb;
  wire [16-1:0] alu_inb_reg;
  wire [2-1:0] alu_inb_addr;
  wire [2-1:0] alu_inb_addr_reg;
  wire [4-1:0] alu_op;
  wire [4-1:0] alu_op_reg;
  wire [3-1:0] rf_raddr;
  wire [3-1:0] rf_raddr_reg;
  wire [3-1:0] rf_waddr;
  wire [3-1:0] rf_waddr_reg;
  wire [16-1:0] rf_data_in;
  wire [16-1:0] rf_data_out;
  wire [16-1:0] rf_data_out_reg;
  wire [16-1:0] mux_rf_const_out;
  wire [3-1:0] mux_rf_const_waddr_out;

  wire [16-1:0] conf_wr_addr;
  wire [16-1:0] conf_wr_data;
  wire conf_wr_en;

  wire init_const_we;
  wire [3-1:0] init_conf_const_waddr;
  wire [16-1:0] init_conf_const;
  wire [16-1:0] pc_max;
  wire [16-1:0] pc_loop;
  wire [16-1:0] pc_out;
  wire [16-1:0] ignore_until;

  wire [16-1:0] ina_reg;
  wire [16-1:0] inb_reg;
  wire [16-1:0] inst_mem_out;
  wire [4-1:0] alu_out_decode_out;
  wire rf_we;
  wire rf_rd_en;
  wire [16-1:0] fifo_in_data_reg;
  wire ignore_until_en;
  wire ignore_counter_en;
  wire [16-1:0] ignore_counter_out;

  assign fifo_in_re = ((alu_ina_addr_reg == 2'd3) || (alu_inb_addr_reg == 2'd3)) && en;
  assign fifo_out_we = (alu_out_addr_reg == 2'd3) && en && ignore_until_en;
  assign ignore_counter_en = (alu_out_addr_reg == 2'd3) && en;
  assign rf_rd_en = ((alu_ina_addr_reg == 2'd2) || (alu_inb_addr_reg == 2'd2)) && en;

  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  fifo_in_data_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(fifo_in_re),
    .in(fifo_in_data),
    .out(fifo_in_data_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  fifo_out_data_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en & alu_out_decode_out[3]),
    .in(alu_out),
    .out(fifo_out_data)
  );


  cgra_counter
  #(
    .WIDTH(16)
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


  mux4x1
  #(
    .WIDTH(16)
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


  mux4x1
  #(
    .WIDTH(16)
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


  cgra0_alu_out_decode_io
  alu_out_decode
  (
    .sel(alu_out_addr_reg),
    .out(alu_out_decode_out)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  ina_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en),
    .in(ina),
    .out(ina_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  inb_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en),
    .in(inb),
    .out(inb_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  outa_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en & alu_out_decode_out[0]),
    .in(alu_out),
    .out(outa)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  outb_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en & alu_out_decode_out[1]),
    .in(alu_out),
    .out(outb)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  alu_ina_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en),
    .in(alu_ina),
    .out(alu_ina_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(2)
  )
  alu_ina_addr_reg_inst
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(alu_ina_addr),
    .out(alu_ina_addr_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  alu_inb_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en),
    .in(alu_inb),
    .out(alu_inb_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(2)
  )
  alu_inb_addr_reg_inst
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(alu_inb_addr),
    .out(alu_inb_addr_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(8),
    .DATA_WIDTH(2)
  )
  alu_out_addr_reg_inst
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(alu_out_addr),
    .out(alu_out_addr_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  rf_data_in_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en & alu_out_decode_out[2]),
    .in(alu_out),
    .out(rf_data_in)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(1)
  )
  rf_we_reg_inst
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(alu_out_decode_out[2]),
    .out(rf_we)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(16)
  )
  rf_data_out_reg_inst
  (
    .clk(clk),
    .rst(1'b0),
    .en(en),
    .in(rf_data_out),
    .out(rf_data_out_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(6),
    .DATA_WIDTH(4)
  )
  alu_op_reg_inst
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(alu_op),
    .out(alu_op_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(3)
  )
  rf_raddr_reg_inst
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(rf_raddr),
    .out(rf_raddr_reg)
  );


  reg_pipe
  #(
    .NUM_STAGES(8),
    .DATA_WIDTH(3)
  )
  rf_waddr_reg_inst
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .in(rf_waddr),
    .out(rf_waddr_reg)
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


  mux2x1
  #(
    .WIDTH(3)
  )
  mux_rf_const_waddr
  (
    .sel(init_const_we),
    .in0(rf_waddr_reg),
    .in1(init_conf_const_waddr),
    .out(mux_rf_const_waddr_out)
  );


  memory
  #(
    .DATA_WIDTH(16),
    .ADDR_WIDTH(3)
  )
  rf
  (
    .clk(clk),
    .we(rf_we | init_const_we),
    .re(rf_rd_en),
    .raddr(rf_raddr_reg),
    .waddr(mux_rf_const_waddr_out),
    .din(mux_rf_const_out),
    .dout(rf_data_out)
  );


  cgra0_alu
  #(
    .DATA_WIDTH(16)
  )
  alu
  (
    .clk(clk),
    .op(alu_op_reg),
    .ina(alu_ina_reg),
    .inb(alu_inb_reg),
    .out(alu_out)
  );


  memory
  #(
    .DATA_WIDTH(16),
    .ADDR_WIDTH(16)
  )
  inst_mem
  (
    .clk(clk),
    .we(conf_wr_en),
    .re(1'b1),
    .raddr(pc_out),
    .waddr(conf_wr_addr),
    .din(conf_wr_data),
    .dout(inst_mem_out)
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
    .pc_max(pc_max),
    .pc_loop(pc_loop),
    .store_ignore(ignore_until)
  );


  program_counter_16
  pc
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .max(pc_max),
    .loop(pc_loop),
    .pc(pc_out)
  );


  cgra0_inst_decode
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