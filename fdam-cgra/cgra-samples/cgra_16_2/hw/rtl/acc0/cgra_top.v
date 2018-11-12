
module cgra_top
(
  input clk,
  input rst,
  input [16-1:0] pes_en,
  input [60-1:0] conf_bus_in,
  input [131-1:0] net_en,
  input en_pc_net,
  input net_conf_mem_we,
  input [8-1:0] net_conf_mem_waddr,
  input [192-1:0] net_conf_mem_data_in,
  output [2-1:0] fifo_in_re,
  input [32-1:0] fifo_in_data,
  output [2-1:0] fifo_out_we,
  output [32-1:0] fifo_out_data
);


  wire net_conf_re;
  wire [8-1:0] net_conf_mem_raddr;
  wire [192-1:0] net_conf_mem_data_out;

  wire [60-1:0] conf_bus [0:17-1];
  wire [60-1:0] conf_bus_in_reg;
  wire [8-1:0] net_pc_max;
  wire [8-1:0] net_pc_loop;

  wire [16-1:0] pe2neta [0:16-1];
  wire [16-1:0] pe2netb [0:16-1];
  wire [16-1:0] net2pea [0:16-1];
  wire [16-1:0] net2peb [0:16-1];

  genvar genv;


  cgra_reg_pipe_
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(60)
  )
  reg_conf_bus_in
  (
    .clk(clk),
    .en(1'b1),
    .in(conf_bus_in),
    .out(conf_bus_in_reg)
  );


  cgra_reg_pipe_
  #(
    .NUM_STAGES(1),
    .DATA_WIDTH(60)
  )
  reg_conf_bus_in2
  (
    .clk(clk),
    .en(1'b1),
    .in(conf_bus_in),
    .out(conf_bus[0])
  );


  generate for(genv=0; genv<2; genv=genv+1) begin : inst_pe_io

    cgra_pe_io
    #(
      .PE_ID(genv + 1)
    )
    pe_io
    (
      .clk(clk),
      .rst(rst),
      .en(pes_en[genv]),
      .conf_bus_in(conf_bus[genv]),
      .fifo_in_re(fifo_in_re[genv]),
      .fifo_in_data(fifo_in_data[(genv+1)*16-1:genv*16]),
      .fifo_out_we(fifo_out_we[genv]),
      .fifo_out_data(fifo_out_data[(genv+1)*16-1:genv*16]),
      .ina(net2pea[genv]),
      .inb(net2peb[genv]),
      .outa(pe2neta[genv]),
      .outb(pe2netb[genv])
    );


    cgra_reg_pipe_
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(60)
    )
    conf_bus_reg_pe_io
    (
      .clk(clk),
      .en(1'b1),
      .in(conf_bus[genv]),
      .out(conf_bus[genv + 1])
    );

  end
  endgenerate


  generate for(genv=2; genv<16; genv=genv+1) begin : inst_pe

    cgra_pe
    #(
      .PE_ID(genv + 1)
    )
    pe
    (
      .clk(clk),
      .rst(rst),
      .en(pes_en[genv]),
      .conf_bus_in(conf_bus[genv]),
      .ina(net2pea[genv]),
      .inb(net2peb[genv]),
      .outa(pe2neta[genv]),
      .outb(pe2netb[genv])
    );


    cgra_reg_pipe_
    #(
      .NUM_STAGES(1),
      .DATA_WIDTH(60)
    )
    conf_bus_reg_pe
    (
      .clk(clk),
      .en(1'b1),
      .in(conf_bus[genv]),
      .out(conf_bus[genv + 1])
    );

  end
  endgenerate


  cgra_conf_reader_pc_net
  #(
    .PE_ID(17)
  )
  cgra_conf_reader_pc_net
  (
    .clk(clk),
    .rst(rst),
    .conf_bus_in(conf_bus_in_reg),
    .pc_max(net_pc_max),
    .pc_loop(net_pc_loop)
  );


  cgra_program_counter
  pc
  (
    .clk(clk),
    .rst(rst),
    .en(en_pc_net),
    .max(net_pc_max),
    .loop(net_pc_loop),
    .pc(net_conf_mem_raddr),
    .pc_en(net_conf_re)
  );


  generate for(genv=0; genv<6; genv=genv+1) begin : inst_mem_conf

    cgra_memory
    #(
      .DATA_WIDTH(32),
      .ADDR_WIDTH(8)
    )
    net_conf_mem
    (
      .clk(clk),
      .we(net_conf_mem_we),
      .re(net_conf_re),
      .raddr(net_conf_mem_raddr),
      .waddr(net_conf_mem_waddr),
      .din(net_conf_mem_data_in[32*(genv+1)-1:32*genv]),
      .dout(net_conf_mem_data_out[32*(genv+1)-1:32*genv])
    );

  end
  endgenerate


  cgra_omega32x32_4
  #(
    .WIDTH(16),
    .PIPE_EXTRA(0)
  )
  net
  (
    .clk(clk),
    .en(net_en),
    .conf(net_conf_mem_data_out),
    .in0(pe2neta[0]),
    .in1(pe2netb[0]),
    .in2(pe2neta[1]),
    .in3(pe2netb[1]),
    .in4(pe2neta[2]),
    .in5(pe2netb[2]),
    .in6(pe2neta[3]),
    .in7(pe2netb[3]),
    .in8(pe2neta[4]),
    .in9(pe2netb[4]),
    .in10(pe2neta[5]),
    .in11(pe2netb[5]),
    .in12(pe2neta[6]),
    .in13(pe2netb[6]),
    .in14(pe2neta[7]),
    .in15(pe2netb[7]),
    .in16(pe2neta[8]),
    .in17(pe2netb[8]),
    .in18(pe2neta[9]),
    .in19(pe2netb[9]),
    .in20(pe2neta[10]),
    .in21(pe2netb[10]),
    .in22(pe2neta[11]),
    .in23(pe2netb[11]),
    .in24(pe2neta[12]),
    .in25(pe2netb[12]),
    .in26(pe2neta[13]),
    .in27(pe2netb[13]),
    .in28(pe2neta[14]),
    .in29(pe2netb[14]),
    .in30(pe2neta[15]),
    .in31(pe2netb[15]),
    .out0(net2pea[0]),
    .out1(net2peb[0]),
    .out2(net2pea[1]),
    .out3(net2peb[1]),
    .out4(net2pea[2]),
    .out5(net2peb[2]),
    .out6(net2pea[3]),
    .out7(net2peb[3]),
    .out8(net2pea[4]),
    .out9(net2peb[4]),
    .out10(net2pea[5]),
    .out11(net2peb[5]),
    .out12(net2pea[6]),
    .out13(net2peb[6]),
    .out14(net2pea[7]),
    .out15(net2peb[7]),
    .out16(net2pea[8]),
    .out17(net2peb[8]),
    .out18(net2pea[9]),
    .out19(net2peb[9]),
    .out20(net2pea[10]),
    .out21(net2peb[10]),
    .out22(net2pea[11]),
    .out23(net2peb[11]),
    .out24(net2pea[12]),
    .out25(net2peb[12]),
    .out26(net2pea[13]),
    .out27(net2peb[13]),
    .out28(net2pea[14]),
    .out29(net2peb[14]),
    .out30(net2pea[15]),
    .out31(net2peb[15])
  );


endmodule
