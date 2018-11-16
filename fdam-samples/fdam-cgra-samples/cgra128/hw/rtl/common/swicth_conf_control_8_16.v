module swicth_conf_control_8_16 #
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

  wire [16-1:0] net_pc_max;
  wire [16-1:0] net_pc_loop;
  wire [16-1:0] net_conf_mem_raddr;
  wire net_mem_we;
  wire [16-1:0] net_mem_waddr;
  wire [8-1:0] net_mem_din;
  wire [8-1:0] net_mem_dout;

  swicth_conf_reader_8_16
  #(
    .SWICTH_NUMBER(SWICTH_NUMBER)
  )
  swicth_conf_reader
  (
    .clk(clk),
    .rst(rst),
    .conf_bus_in(conf_bus_in),
    .pc_max(net_pc_max),
    .pc_loop(net_pc_loop),
    .net_mem_we(net_mem_we),
    .net_mem_waddr(net_mem_waddr),
    .net_mem_data(net_mem_din)
  );


  program_counter_16
  pc
  (
    .clk(clk),
    .rst(rst),
    .en(en_pc_net),
    .max(net_pc_max),
    .loop(net_pc_loop),
    .pc(net_conf_mem_raddr)
  );


  memory
  #(
    .DATA_WIDTH(8),
    .ADDR_WIDTH(16)
  )
  mem_conf
  (
    .clk(clk),
    .we(net_mem_we),
    .re(1'b1),
    .raddr(net_conf_mem_raddr),
    .waddr(net_conf_mem_raddr),
    .din(net_mem_din),
    .dout(net_mem_dout)
  );


  reg_pipe
  #(
    .NUM_STAGES(STAGE),
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