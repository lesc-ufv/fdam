module cgra_omega32x32_4 #
(
  parameter WIDTH = 16,
  parameter PIPE_EXTRA = 0
)
(
  input clk,
  input [131-1:0] en,
  input [192-1:0] conf,
  input [WIDTH-1:0] in0,
  input [WIDTH-1:0] in1,
  input [WIDTH-1:0] in2,
  input [WIDTH-1:0] in3,
  input [WIDTH-1:0] in4,
  input [WIDTH-1:0] in5,
  input [WIDTH-1:0] in6,
  input [WIDTH-1:0] in7,
  input [WIDTH-1:0] in8,
  input [WIDTH-1:0] in9,
  input [WIDTH-1:0] in10,
  input [WIDTH-1:0] in11,
  input [WIDTH-1:0] in12,
  input [WIDTH-1:0] in13,
  input [WIDTH-1:0] in14,
  input [WIDTH-1:0] in15,
  input [WIDTH-1:0] in16,
  input [WIDTH-1:0] in17,
  input [WIDTH-1:0] in18,
  input [WIDTH-1:0] in19,
  input [WIDTH-1:0] in20,
  input [WIDTH-1:0] in21,
  input [WIDTH-1:0] in22,
  input [WIDTH-1:0] in23,
  input [WIDTH-1:0] in24,
  input [WIDTH-1:0] in25,
  input [WIDTH-1:0] in26,
  input [WIDTH-1:0] in27,
  input [WIDTH-1:0] in28,
  input [WIDTH-1:0] in29,
  input [WIDTH-1:0] in30,
  input [WIDTH-1:0] in31,
  output [WIDTH-1:0] out0,
  output [WIDTH-1:0] out1,
  output [WIDTH-1:0] out2,
  output [WIDTH-1:0] out3,
  output [WIDTH-1:0] out4,
  output [WIDTH-1:0] out5,
  output [WIDTH-1:0] out6,
  output [WIDTH-1:0] out7,
  output [WIDTH-1:0] out8,
  output [WIDTH-1:0] out9,
  output [WIDTH-1:0] out10,
  output [WIDTH-1:0] out11,
  output [WIDTH-1:0] out12,
  output [WIDTH-1:0] out13,
  output [WIDTH-1:0] out14,
  output [WIDTH-1:0] out15,
  output [WIDTH-1:0] out16,
  output [WIDTH-1:0] out17,
  output [WIDTH-1:0] out18,
  output [WIDTH-1:0] out19,
  output [WIDTH-1:0] out20,
  output [WIDTH-1:0] out21,
  output [WIDTH-1:0] out22,
  output [WIDTH-1:0] out23,
  output [WIDTH-1:0] out24,
  output [WIDTH-1:0] out25,
  output [WIDTH-1:0] out26,
  output [WIDTH-1:0] out27,
  output [WIDTH-1:0] out28,
  output [WIDTH-1:0] out29,
  output [WIDTH-1:0] out30,
  output [WIDTH-1:0] out31
);

  wire [64-1:0] conf_reg_0;
  wire [64-1:0] conf_reg_1;
  wire [64-1:0] conf_reg_2;
  wire [WIDTH-1:0] in_reg_wire_0 [0:32-1];
  wire [WIDTH-1:0] in_reg_wire_1 [0:32-1];
  wire [WIDTH-1:0] in_reg_wire_2 [0:32-1];
  wire [WIDTH-1:0] in_reg_wire_3 [0:32-1];
  wire [WIDTH-1:0] in_reg_wire_4 [0:32-1];
  wire [WIDTH-1:0] in_reg_wire_5 [0:32-1];

  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(64)
  )
  reg_conf_0
  (
    .clk(clk),
    .en(en[0]),
    .in(conf[63:0]),
    .out(conf_reg_0)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 2),
    .DATA_WIDTH(64)
  )
  reg_conf_1
  (
    .clk(clk),
    .en(en[1]),
    .in(conf[127:64]),
    .out(conf_reg_1)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 3),
    .DATA_WIDTH(64)
  )
  reg_conf_2
  (
    .clk(clk),
    .en(en[2]),
    .in(conf[191:128]),
    .out(conf_reg_2)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_0
  (
    .clk(clk),
    .en(en[3]),
    .in(in0),
    .out(in_reg_wire_0[0])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_1
  (
    .clk(clk),
    .en(en[4]),
    .in(in1),
    .out(in_reg_wire_0[1])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_2
  (
    .clk(clk),
    .en(en[5]),
    .in(in2),
    .out(in_reg_wire_0[2])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_3
  (
    .clk(clk),
    .en(en[6]),
    .in(in3),
    .out(in_reg_wire_0[3])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_4
  (
    .clk(clk),
    .en(en[7]),
    .in(in4),
    .out(in_reg_wire_0[4])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_5
  (
    .clk(clk),
    .en(en[8]),
    .in(in5),
    .out(in_reg_wire_0[5])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_6
  (
    .clk(clk),
    .en(en[9]),
    .in(in6),
    .out(in_reg_wire_0[6])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_7
  (
    .clk(clk),
    .en(en[10]),
    .in(in7),
    .out(in_reg_wire_0[7])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_8
  (
    .clk(clk),
    .en(en[11]),
    .in(in8),
    .out(in_reg_wire_0[8])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_9
  (
    .clk(clk),
    .en(en[12]),
    .in(in9),
    .out(in_reg_wire_0[9])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_10
  (
    .clk(clk),
    .en(en[13]),
    .in(in10),
    .out(in_reg_wire_0[10])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_11
  (
    .clk(clk),
    .en(en[14]),
    .in(in11),
    .out(in_reg_wire_0[11])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_12
  (
    .clk(clk),
    .en(en[15]),
    .in(in12),
    .out(in_reg_wire_0[12])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_13
  (
    .clk(clk),
    .en(en[16]),
    .in(in13),
    .out(in_reg_wire_0[13])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_14
  (
    .clk(clk),
    .en(en[17]),
    .in(in14),
    .out(in_reg_wire_0[14])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_15
  (
    .clk(clk),
    .en(en[18]),
    .in(in15),
    .out(in_reg_wire_0[15])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_16
  (
    .clk(clk),
    .en(en[19]),
    .in(in16),
    .out(in_reg_wire_0[16])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_17
  (
    .clk(clk),
    .en(en[20]),
    .in(in17),
    .out(in_reg_wire_0[17])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_18
  (
    .clk(clk),
    .en(en[21]),
    .in(in18),
    .out(in_reg_wire_0[18])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_19
  (
    .clk(clk),
    .en(en[22]),
    .in(in19),
    .out(in_reg_wire_0[19])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_20
  (
    .clk(clk),
    .en(en[23]),
    .in(in20),
    .out(in_reg_wire_0[20])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_21
  (
    .clk(clk),
    .en(en[24]),
    .in(in21),
    .out(in_reg_wire_0[21])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_22
  (
    .clk(clk),
    .en(en[25]),
    .in(in22),
    .out(in_reg_wire_0[22])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_23
  (
    .clk(clk),
    .en(en[26]),
    .in(in23),
    .out(in_reg_wire_0[23])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_24
  (
    .clk(clk),
    .en(en[27]),
    .in(in24),
    .out(in_reg_wire_0[24])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_25
  (
    .clk(clk),
    .en(en[28]),
    .in(in25),
    .out(in_reg_wire_0[25])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_26
  (
    .clk(clk),
    .en(en[29]),
    .in(in26),
    .out(in_reg_wire_0[26])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_27
  (
    .clk(clk),
    .en(en[30]),
    .in(in27),
    .out(in_reg_wire_0[27])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_28
  (
    .clk(clk),
    .en(en[31]),
    .in(in28),
    .out(in_reg_wire_0[28])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_29
  (
    .clk(clk),
    .en(en[32]),
    .in(in29),
    .out(in_reg_wire_0[29])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_30
  (
    .clk(clk),
    .en(en[33]),
    .in(in30),
    .out(in_reg_wire_0[30])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_0_31
  (
    .clk(clk),
    .en(en[34]),
    .in(in31),
    .out(in_reg_wire_0[31])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_0
  (
    .clk(clk),
    .en(en[35]),
    .in(in_reg_wire_1[0]),
    .out(in_reg_wire_2[0])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_1
  (
    .clk(clk),
    .en(en[36]),
    .in(in_reg_wire_1[1]),
    .out(in_reg_wire_2[1])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_2
  (
    .clk(clk),
    .en(en[37]),
    .in(in_reg_wire_1[2]),
    .out(in_reg_wire_2[2])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_3
  (
    .clk(clk),
    .en(en[38]),
    .in(in_reg_wire_1[3]),
    .out(in_reg_wire_2[3])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_4
  (
    .clk(clk),
    .en(en[39]),
    .in(in_reg_wire_1[4]),
    .out(in_reg_wire_2[4])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_5
  (
    .clk(clk),
    .en(en[40]),
    .in(in_reg_wire_1[5]),
    .out(in_reg_wire_2[5])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_6
  (
    .clk(clk),
    .en(en[41]),
    .in(in_reg_wire_1[6]),
    .out(in_reg_wire_2[6])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_7
  (
    .clk(clk),
    .en(en[42]),
    .in(in_reg_wire_1[7]),
    .out(in_reg_wire_2[7])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_8
  (
    .clk(clk),
    .en(en[43]),
    .in(in_reg_wire_1[8]),
    .out(in_reg_wire_2[8])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_9
  (
    .clk(clk),
    .en(en[44]),
    .in(in_reg_wire_1[9]),
    .out(in_reg_wire_2[9])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_10
  (
    .clk(clk),
    .en(en[45]),
    .in(in_reg_wire_1[10]),
    .out(in_reg_wire_2[10])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_11
  (
    .clk(clk),
    .en(en[46]),
    .in(in_reg_wire_1[11]),
    .out(in_reg_wire_2[11])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_12
  (
    .clk(clk),
    .en(en[47]),
    .in(in_reg_wire_1[12]),
    .out(in_reg_wire_2[12])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_13
  (
    .clk(clk),
    .en(en[48]),
    .in(in_reg_wire_1[13]),
    .out(in_reg_wire_2[13])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_14
  (
    .clk(clk),
    .en(en[49]),
    .in(in_reg_wire_1[14]),
    .out(in_reg_wire_2[14])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_15
  (
    .clk(clk),
    .en(en[50]),
    .in(in_reg_wire_1[15]),
    .out(in_reg_wire_2[15])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_16
  (
    .clk(clk),
    .en(en[51]),
    .in(in_reg_wire_1[16]),
    .out(in_reg_wire_2[16])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_17
  (
    .clk(clk),
    .en(en[52]),
    .in(in_reg_wire_1[17]),
    .out(in_reg_wire_2[17])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_18
  (
    .clk(clk),
    .en(en[53]),
    .in(in_reg_wire_1[18]),
    .out(in_reg_wire_2[18])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_19
  (
    .clk(clk),
    .en(en[54]),
    .in(in_reg_wire_1[19]),
    .out(in_reg_wire_2[19])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_20
  (
    .clk(clk),
    .en(en[55]),
    .in(in_reg_wire_1[20]),
    .out(in_reg_wire_2[20])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_21
  (
    .clk(clk),
    .en(en[56]),
    .in(in_reg_wire_1[21]),
    .out(in_reg_wire_2[21])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_22
  (
    .clk(clk),
    .en(en[57]),
    .in(in_reg_wire_1[22]),
    .out(in_reg_wire_2[22])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_23
  (
    .clk(clk),
    .en(en[58]),
    .in(in_reg_wire_1[23]),
    .out(in_reg_wire_2[23])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_24
  (
    .clk(clk),
    .en(en[59]),
    .in(in_reg_wire_1[24]),
    .out(in_reg_wire_2[24])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_25
  (
    .clk(clk),
    .en(en[60]),
    .in(in_reg_wire_1[25]),
    .out(in_reg_wire_2[25])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_26
  (
    .clk(clk),
    .en(en[61]),
    .in(in_reg_wire_1[26]),
    .out(in_reg_wire_2[26])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_27
  (
    .clk(clk),
    .en(en[62]),
    .in(in_reg_wire_1[27]),
    .out(in_reg_wire_2[27])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_28
  (
    .clk(clk),
    .en(en[63]),
    .in(in_reg_wire_1[28]),
    .out(in_reg_wire_2[28])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_29
  (
    .clk(clk),
    .en(en[64]),
    .in(in_reg_wire_1[29]),
    .out(in_reg_wire_2[29])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_30
  (
    .clk(clk),
    .en(en[65]),
    .in(in_reg_wire_1[30]),
    .out(in_reg_wire_2[30])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_1_31
  (
    .clk(clk),
    .en(en[66]),
    .in(in_reg_wire_1[31]),
    .out(in_reg_wire_2[31])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_0
  (
    .clk(clk),
    .en(en[67]),
    .in(in_reg_wire_3[0]),
    .out(in_reg_wire_4[0])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_1
  (
    .clk(clk),
    .en(en[68]),
    .in(in_reg_wire_3[1]),
    .out(in_reg_wire_4[1])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_2
  (
    .clk(clk),
    .en(en[69]),
    .in(in_reg_wire_3[2]),
    .out(in_reg_wire_4[2])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_3
  (
    .clk(clk),
    .en(en[70]),
    .in(in_reg_wire_3[3]),
    .out(in_reg_wire_4[3])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_4
  (
    .clk(clk),
    .en(en[71]),
    .in(in_reg_wire_3[4]),
    .out(in_reg_wire_4[4])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_5
  (
    .clk(clk),
    .en(en[72]),
    .in(in_reg_wire_3[5]),
    .out(in_reg_wire_4[5])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_6
  (
    .clk(clk),
    .en(en[73]),
    .in(in_reg_wire_3[6]),
    .out(in_reg_wire_4[6])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_7
  (
    .clk(clk),
    .en(en[74]),
    .in(in_reg_wire_3[7]),
    .out(in_reg_wire_4[7])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_8
  (
    .clk(clk),
    .en(en[75]),
    .in(in_reg_wire_3[8]),
    .out(in_reg_wire_4[8])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_9
  (
    .clk(clk),
    .en(en[76]),
    .in(in_reg_wire_3[9]),
    .out(in_reg_wire_4[9])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_10
  (
    .clk(clk),
    .en(en[77]),
    .in(in_reg_wire_3[10]),
    .out(in_reg_wire_4[10])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_11
  (
    .clk(clk),
    .en(en[78]),
    .in(in_reg_wire_3[11]),
    .out(in_reg_wire_4[11])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_12
  (
    .clk(clk),
    .en(en[79]),
    .in(in_reg_wire_3[12]),
    .out(in_reg_wire_4[12])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_13
  (
    .clk(clk),
    .en(en[80]),
    .in(in_reg_wire_3[13]),
    .out(in_reg_wire_4[13])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_14
  (
    .clk(clk),
    .en(en[81]),
    .in(in_reg_wire_3[14]),
    .out(in_reg_wire_4[14])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_15
  (
    .clk(clk),
    .en(en[82]),
    .in(in_reg_wire_3[15]),
    .out(in_reg_wire_4[15])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_16
  (
    .clk(clk),
    .en(en[83]),
    .in(in_reg_wire_3[16]),
    .out(in_reg_wire_4[16])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_17
  (
    .clk(clk),
    .en(en[84]),
    .in(in_reg_wire_3[17]),
    .out(in_reg_wire_4[17])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_18
  (
    .clk(clk),
    .en(en[85]),
    .in(in_reg_wire_3[18]),
    .out(in_reg_wire_4[18])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_19
  (
    .clk(clk),
    .en(en[86]),
    .in(in_reg_wire_3[19]),
    .out(in_reg_wire_4[19])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_20
  (
    .clk(clk),
    .en(en[87]),
    .in(in_reg_wire_3[20]),
    .out(in_reg_wire_4[20])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_21
  (
    .clk(clk),
    .en(en[88]),
    .in(in_reg_wire_3[21]),
    .out(in_reg_wire_4[21])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_22
  (
    .clk(clk),
    .en(en[89]),
    .in(in_reg_wire_3[22]),
    .out(in_reg_wire_4[22])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_23
  (
    .clk(clk),
    .en(en[90]),
    .in(in_reg_wire_3[23]),
    .out(in_reg_wire_4[23])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_24
  (
    .clk(clk),
    .en(en[91]),
    .in(in_reg_wire_3[24]),
    .out(in_reg_wire_4[24])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_25
  (
    .clk(clk),
    .en(en[92]),
    .in(in_reg_wire_3[25]),
    .out(in_reg_wire_4[25])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_26
  (
    .clk(clk),
    .en(en[93]),
    .in(in_reg_wire_3[26]),
    .out(in_reg_wire_4[26])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_27
  (
    .clk(clk),
    .en(en[94]),
    .in(in_reg_wire_3[27]),
    .out(in_reg_wire_4[27])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_28
  (
    .clk(clk),
    .en(en[95]),
    .in(in_reg_wire_3[28]),
    .out(in_reg_wire_4[28])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_29
  (
    .clk(clk),
    .en(en[96]),
    .in(in_reg_wire_3[29]),
    .out(in_reg_wire_4[29])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_30
  (
    .clk(clk),
    .en(en[97]),
    .in(in_reg_wire_3[30]),
    .out(in_reg_wire_4[30])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_2_31
  (
    .clk(clk),
    .en(en[98]),
    .in(in_reg_wire_3[31]),
    .out(in_reg_wire_4[31])
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_0
  (
    .clk(clk),
    .en(en[99]),
    .in(in_reg_wire_5[0]),
    .out(out0)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_1
  (
    .clk(clk),
    .en(en[100]),
    .in(in_reg_wire_5[1]),
    .out(out1)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_2
  (
    .clk(clk),
    .en(en[101]),
    .in(in_reg_wire_5[2]),
    .out(out2)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_3
  (
    .clk(clk),
    .en(en[102]),
    .in(in_reg_wire_5[3]),
    .out(out3)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_4
  (
    .clk(clk),
    .en(en[103]),
    .in(in_reg_wire_5[4]),
    .out(out4)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_5
  (
    .clk(clk),
    .en(en[104]),
    .in(in_reg_wire_5[5]),
    .out(out5)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_6
  (
    .clk(clk),
    .en(en[105]),
    .in(in_reg_wire_5[6]),
    .out(out6)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_7
  (
    .clk(clk),
    .en(en[106]),
    .in(in_reg_wire_5[7]),
    .out(out7)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_8
  (
    .clk(clk),
    .en(en[107]),
    .in(in_reg_wire_5[8]),
    .out(out8)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_9
  (
    .clk(clk),
    .en(en[108]),
    .in(in_reg_wire_5[9]),
    .out(out9)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_10
  (
    .clk(clk),
    .en(en[109]),
    .in(in_reg_wire_5[10]),
    .out(out10)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_11
  (
    .clk(clk),
    .en(en[110]),
    .in(in_reg_wire_5[11]),
    .out(out11)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_12
  (
    .clk(clk),
    .en(en[111]),
    .in(in_reg_wire_5[12]),
    .out(out12)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_13
  (
    .clk(clk),
    .en(en[112]),
    .in(in_reg_wire_5[13]),
    .out(out13)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_14
  (
    .clk(clk),
    .en(en[113]),
    .in(in_reg_wire_5[14]),
    .out(out14)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_15
  (
    .clk(clk),
    .en(en[114]),
    .in(in_reg_wire_5[15]),
    .out(out15)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_16
  (
    .clk(clk),
    .en(en[115]),
    .in(in_reg_wire_5[16]),
    .out(out16)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_17
  (
    .clk(clk),
    .en(en[116]),
    .in(in_reg_wire_5[17]),
    .out(out17)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_18
  (
    .clk(clk),
    .en(en[117]),
    .in(in_reg_wire_5[18]),
    .out(out18)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_19
  (
    .clk(clk),
    .en(en[118]),
    .in(in_reg_wire_5[19]),
    .out(out19)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_20
  (
    .clk(clk),
    .en(en[119]),
    .in(in_reg_wire_5[20]),
    .out(out20)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_21
  (
    .clk(clk),
    .en(en[120]),
    .in(in_reg_wire_5[21]),
    .out(out21)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_22
  (
    .clk(clk),
    .en(en[121]),
    .in(in_reg_wire_5[22]),
    .out(out22)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_23
  (
    .clk(clk),
    .en(en[122]),
    .in(in_reg_wire_5[23]),
    .out(out23)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_24
  (
    .clk(clk),
    .en(en[123]),
    .in(in_reg_wire_5[24]),
    .out(out24)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_25
  (
    .clk(clk),
    .en(en[124]),
    .in(in_reg_wire_5[25]),
    .out(out25)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_26
  (
    .clk(clk),
    .en(en[125]),
    .in(in_reg_wire_5[26]),
    .out(out26)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_27
  (
    .clk(clk),
    .en(en[126]),
    .in(in_reg_wire_5[27]),
    .out(out27)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_28
  (
    .clk(clk),
    .en(en[127]),
    .in(in_reg_wire_5[28]),
    .out(out28)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_29
  (
    .clk(clk),
    .en(en[128]),
    .in(in_reg_wire_5[29]),
    .out(out29)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_30
  (
    .clk(clk),
    .en(en[129]),
    .in(in_reg_wire_5[30]),
    .out(out30)
  );


  cgra_reg_pipe
  #(
    .NUM_STAGES(PIPE_EXTRA + 1),
    .DATA_WIDTH(WIDTH)
  )
  reg_in_3_31
  (
    .clk(clk),
    .en(en[130]),
    .in(in_reg_wire_5[31]),
    .out(out31)
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_0_0
  (
    .sel(conf_reg_0[7:0]),
    .in0(in_reg_wire_0[0]),
    .in1(in_reg_wire_0[8]),
    .in2(in_reg_wire_0[16]),
    .in3(in_reg_wire_0[24]),
    .out0(in_reg_wire_1[0]),
    .out1(in_reg_wire_1[1]),
    .out2(in_reg_wire_1[2]),
    .out3(in_reg_wire_1[3])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_0_1
  (
    .sel(conf_reg_0[15:8]),
    .in0(in_reg_wire_0[1]),
    .in1(in_reg_wire_0[9]),
    .in2(in_reg_wire_0[17]),
    .in3(in_reg_wire_0[25]),
    .out0(in_reg_wire_1[4]),
    .out1(in_reg_wire_1[5]),
    .out2(in_reg_wire_1[6]),
    .out3(in_reg_wire_1[7])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_0_2
  (
    .sel(conf_reg_0[23:16]),
    .in0(in_reg_wire_0[2]),
    .in1(in_reg_wire_0[10]),
    .in2(in_reg_wire_0[18]),
    .in3(in_reg_wire_0[26]),
    .out0(in_reg_wire_1[8]),
    .out1(in_reg_wire_1[9]),
    .out2(in_reg_wire_1[10]),
    .out3(in_reg_wire_1[11])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_0_3
  (
    .sel(conf_reg_0[31:24]),
    .in0(in_reg_wire_0[3]),
    .in1(in_reg_wire_0[11]),
    .in2(in_reg_wire_0[19]),
    .in3(in_reg_wire_0[27]),
    .out0(in_reg_wire_1[12]),
    .out1(in_reg_wire_1[13]),
    .out2(in_reg_wire_1[14]),
    .out3(in_reg_wire_1[15])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_0_4
  (
    .sel(conf_reg_0[39:32]),
    .in0(in_reg_wire_0[4]),
    .in1(in_reg_wire_0[12]),
    .in2(in_reg_wire_0[20]),
    .in3(in_reg_wire_0[28]),
    .out0(in_reg_wire_1[16]),
    .out1(in_reg_wire_1[17]),
    .out2(in_reg_wire_1[18]),
    .out3(in_reg_wire_1[19])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_0_5
  (
    .sel(conf_reg_0[47:40]),
    .in0(in_reg_wire_0[5]),
    .in1(in_reg_wire_0[13]),
    .in2(in_reg_wire_0[21]),
    .in3(in_reg_wire_0[29]),
    .out0(in_reg_wire_1[20]),
    .out1(in_reg_wire_1[21]),
    .out2(in_reg_wire_1[22]),
    .out3(in_reg_wire_1[23])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_0_6
  (
    .sel(conf_reg_0[55:48]),
    .in0(in_reg_wire_0[6]),
    .in1(in_reg_wire_0[14]),
    .in2(in_reg_wire_0[22]),
    .in3(in_reg_wire_0[30]),
    .out0(in_reg_wire_1[24]),
    .out1(in_reg_wire_1[25]),
    .out2(in_reg_wire_1[26]),
    .out3(in_reg_wire_1[27])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_0_7
  (
    .sel(conf_reg_0[63:56]),
    .in0(in_reg_wire_0[7]),
    .in1(in_reg_wire_0[15]),
    .in2(in_reg_wire_0[23]),
    .in3(in_reg_wire_0[31]),
    .out0(in_reg_wire_1[28]),
    .out1(in_reg_wire_1[29]),
    .out2(in_reg_wire_1[30]),
    .out3(in_reg_wire_1[31])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_1_0
  (
    .sel(conf_reg_1[7:0]),
    .in0(in_reg_wire_2[0]),
    .in1(in_reg_wire_2[8]),
    .in2(in_reg_wire_2[16]),
    .in3(in_reg_wire_2[24]),
    .out0(in_reg_wire_3[0]),
    .out1(in_reg_wire_3[1]),
    .out2(in_reg_wire_3[2]),
    .out3(in_reg_wire_3[3])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_1_1
  (
    .sel(conf_reg_1[15:8]),
    .in0(in_reg_wire_2[1]),
    .in1(in_reg_wire_2[9]),
    .in2(in_reg_wire_2[17]),
    .in3(in_reg_wire_2[25]),
    .out0(in_reg_wire_3[4]),
    .out1(in_reg_wire_3[5]),
    .out2(in_reg_wire_3[6]),
    .out3(in_reg_wire_3[7])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_1_2
  (
    .sel(conf_reg_1[23:16]),
    .in0(in_reg_wire_2[2]),
    .in1(in_reg_wire_2[10]),
    .in2(in_reg_wire_2[18]),
    .in3(in_reg_wire_2[26]),
    .out0(in_reg_wire_3[8]),
    .out1(in_reg_wire_3[9]),
    .out2(in_reg_wire_3[10]),
    .out3(in_reg_wire_3[11])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_1_3
  (
    .sel(conf_reg_1[31:24]),
    .in0(in_reg_wire_2[3]),
    .in1(in_reg_wire_2[11]),
    .in2(in_reg_wire_2[19]),
    .in3(in_reg_wire_2[27]),
    .out0(in_reg_wire_3[12]),
    .out1(in_reg_wire_3[13]),
    .out2(in_reg_wire_3[14]),
    .out3(in_reg_wire_3[15])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_1_4
  (
    .sel(conf_reg_1[39:32]),
    .in0(in_reg_wire_2[4]),
    .in1(in_reg_wire_2[12]),
    .in2(in_reg_wire_2[20]),
    .in3(in_reg_wire_2[28]),
    .out0(in_reg_wire_3[16]),
    .out1(in_reg_wire_3[17]),
    .out2(in_reg_wire_3[18]),
    .out3(in_reg_wire_3[19])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_1_5
  (
    .sel(conf_reg_1[47:40]),
    .in0(in_reg_wire_2[5]),
    .in1(in_reg_wire_2[13]),
    .in2(in_reg_wire_2[21]),
    .in3(in_reg_wire_2[29]),
    .out0(in_reg_wire_3[20]),
    .out1(in_reg_wire_3[21]),
    .out2(in_reg_wire_3[22]),
    .out3(in_reg_wire_3[23])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_1_6
  (
    .sel(conf_reg_1[55:48]),
    .in0(in_reg_wire_2[6]),
    .in1(in_reg_wire_2[14]),
    .in2(in_reg_wire_2[22]),
    .in3(in_reg_wire_2[30]),
    .out0(in_reg_wire_3[24]),
    .out1(in_reg_wire_3[25]),
    .out2(in_reg_wire_3[26]),
    .out3(in_reg_wire_3[27])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_1_7
  (
    .sel(conf_reg_1[63:56]),
    .in0(in_reg_wire_2[7]),
    .in1(in_reg_wire_2[15]),
    .in2(in_reg_wire_2[23]),
    .in3(in_reg_wire_2[31]),
    .out0(in_reg_wire_3[28]),
    .out1(in_reg_wire_3[29]),
    .out2(in_reg_wire_3[30]),
    .out3(in_reg_wire_3[31])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_2_0
  (
    .sel(conf_reg_2[7:0]),
    .in0(in_reg_wire_4[0]),
    .in1(in_reg_wire_4[8]),
    .in2(in_reg_wire_4[16]),
    .in3(in_reg_wire_4[24]),
    .out0(in_reg_wire_5[0]),
    .out1(in_reg_wire_5[1]),
    .out2(in_reg_wire_5[2]),
    .out3(in_reg_wire_5[3])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_2_1
  (
    .sel(conf_reg_2[15:8]),
    .in0(in_reg_wire_4[1]),
    .in1(in_reg_wire_4[9]),
    .in2(in_reg_wire_4[17]),
    .in3(in_reg_wire_4[25]),
    .out0(in_reg_wire_5[4]),
    .out1(in_reg_wire_5[5]),
    .out2(in_reg_wire_5[6]),
    .out3(in_reg_wire_5[7])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_2_2
  (
    .sel(conf_reg_2[23:16]),
    .in0(in_reg_wire_4[2]),
    .in1(in_reg_wire_4[10]),
    .in2(in_reg_wire_4[18]),
    .in3(in_reg_wire_4[26]),
    .out0(in_reg_wire_5[8]),
    .out1(in_reg_wire_5[9]),
    .out2(in_reg_wire_5[10]),
    .out3(in_reg_wire_5[11])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_2_3
  (
    .sel(conf_reg_2[31:24]),
    .in0(in_reg_wire_4[3]),
    .in1(in_reg_wire_4[11]),
    .in2(in_reg_wire_4[19]),
    .in3(in_reg_wire_4[27]),
    .out0(in_reg_wire_5[12]),
    .out1(in_reg_wire_5[13]),
    .out2(in_reg_wire_5[14]),
    .out3(in_reg_wire_5[15])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_2_4
  (
    .sel(conf_reg_2[39:32]),
    .in0(in_reg_wire_4[4]),
    .in1(in_reg_wire_4[12]),
    .in2(in_reg_wire_4[20]),
    .in3(in_reg_wire_4[28]),
    .out0(in_reg_wire_5[16]),
    .out1(in_reg_wire_5[17]),
    .out2(in_reg_wire_5[18]),
    .out3(in_reg_wire_5[19])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_2_5
  (
    .sel(conf_reg_2[47:40]),
    .in0(in_reg_wire_4[5]),
    .in1(in_reg_wire_4[13]),
    .in2(in_reg_wire_4[21]),
    .in3(in_reg_wire_4[29]),
    .out0(in_reg_wire_5[20]),
    .out1(in_reg_wire_5[21]),
    .out2(in_reg_wire_5[22]),
    .out3(in_reg_wire_5[23])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_2_6
  (
    .sel(conf_reg_2[55:48]),
    .in0(in_reg_wire_4[6]),
    .in1(in_reg_wire_4[14]),
    .in2(in_reg_wire_4[22]),
    .in3(in_reg_wire_4[30]),
    .out0(in_reg_wire_5[24]),
    .out1(in_reg_wire_5[25]),
    .out2(in_reg_wire_5[26]),
    .out3(in_reg_wire_5[27])
  );


  cgra_switch_box4x4
  #(
    .WIDTH(WIDTH)
  )
  sw_2_7
  (
    .sel(conf_reg_2[63:56]),
    .in0(in_reg_wire_4[7]),
    .in1(in_reg_wire_4[15]),
    .in2(in_reg_wire_4[23]),
    .in3(in_reg_wire_4[31]),
    .out0(in_reg_wire_5[28]),
    .out1(in_reg_wire_5[29]),
    .out2(in_reg_wire_5[30]),
    .out3(in_reg_wire_5[31])
  );

endmodule