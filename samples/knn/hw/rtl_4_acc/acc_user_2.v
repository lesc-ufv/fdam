
module acc_user_2 #
(
  parameter DATA_WIDTH = 32,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1
)
(
  input clk,
  input rst,
  input start,
  input [4-1:0] acc_user_done_rd_data,
  input [4-1:0] acc_user_done_wr_data,
  input [4-1:0] acc_user_available_read,
  input [2048-1:0] acc_user_read_data,
  output [4-1:0] acc_user_request_read,
  input [4-1:0] acc_user_read_data_valid,
  input [4-1:0] acc_user_available_write,
  output [2048-1:0] acc_user_write_data,
  output [4-1:0] acc_user_request_write,
  output reg acc_user_done
);

  wire [18-1:0] n0;
  wire [18-1:0] n1;
  wire [18-1:0] n10;
  wire [18-1:0] n11;
  wire [18-1:0] n12;
  wire [18-1:0] n13;
  wire [18-1:0] n14;
  wire [18-1:0] n15;
  wire [18-1:0] n16;
  wire [18-1:0] n17;
  wire [18-1:0] n18;
  wire [18-1:0] n19;
  wire [18-1:0] n2;
  wire [18-1:0] n20;
  wire [18-1:0] n21;
  wire [18-1:0] n22;
  wire [18-1:0] n23;
  wire [18-1:0] n24;
  wire [18-1:0] n25;
  wire [18-1:0] n26;
  wire [18-1:0] n27;
  wire [18-1:0] n28;
  wire [18-1:0] n29;
  wire [18-1:0] n3;
  wire [18-1:0] n30;
  wire [18-1:0] n31;
  wire [18-1:0] n32;
  wire [18-1:0] n33;
  wire [18-1:0] n34;
  wire [18-1:0] n35;
  wire [2-1:0] n36;
  wire [18-1:0] n37;
  wire [18-1:0] n38;
  wire [18-1:0] n39;
  wire [18-1:0] n4;
  wire [18-1:0] n40;
  wire [18-1:0] n41;
  wire [2-1:0] n42;
  wire [18-1:0] n43;
  wire [18-1:0] n44;
  wire [18-1:0] n45;
  wire [18-1:0] n46;
  wire [18-1:0] n47;
  wire [2-1:0] n48;
  wire [18-1:0] n49;
  wire [18-1:0] n5;
  wire [18-1:0] n50;
  wire [18-1:0] n51;
  wire [18-1:0] n6;
  wire [18-1:0] n7;
  wire [18-1:0] n8;
  wire [18-1:0] n9;
  wire [8-1:0] streams_ready;
  wire en;
  assign en = &streams_ready;
  wire [4-1:0] done_wire;

  always @(posedge clk) begin
    acc_user_done <= &done_wire;
  end


  SyncAbs
  i17
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n1),
    .din0(n2)
  );


  SyncAbs
  i18
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n0),
    .din0(n3)
  );


  SyncAbs
  i23
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n10),
    .din0(n5)
  );


  SyncAbs
  i24
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n11),
    .din0(n4)
  );


  SyncAdd
  i25
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n0),
    .din0(n1),
    .dout0(n12)
  );


  SyncAdd
  i26
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n10),
    .din1(n11),
    .dout0(n13)
  );


  SyncAdd
  i27
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n12),
    .din1(n13),
    .dout0(n26)
  );


  SyncIn
  in0
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[0]),
    .rd_available(acc_user_available_read[0]),
    .rd_valid(acc_user_read_data_valid[0]),
    .rd_data(acc_user_read_data[511:0]),
    .rd_en(acc_user_request_read[0]),
    .component_ready(streams_ready[0]),
    .dout0(n14)
  );


  SyncIn
  in1
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[1]),
    .rd_available(acc_user_available_read[1]),
    .rd_valid(acc_user_read_data_valid[1]),
    .rd_data(acc_user_read_data[1023:512]),
    .rd_en(acc_user_request_read[1]),
    .component_ready(streams_ready[1]),
    .dout0(n9)
  );


  SyncIn
  in2
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[2]),
    .rd_available(acc_user_available_read[2]),
    .rd_valid(acc_user_read_data_valid[2]),
    .rd_data(acc_user_read_data[1535:1024]),
    .rd_en(acc_user_request_read[2]),
    .component_ready(streams_ready[2]),
    .dout0(n8)
  );


  SyncIn
  in3
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[3]),
    .rd_available(acc_user_available_read[3]),
    .rd_valid(acc_user_read_data_valid[3]),
    .rd_data(acc_user_read_data[2047:1536]),
    .rd_en(acc_user_request_read[3]),
    .component_ready(streams_ready[3]),
    .dout0(n7)
  );


  SyncKnnCtrl
  i0
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n26),
    .dout0(n31),
    .dout1(n36)
  );


  SyncKnnCtrl
  i1
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n31),
    .dout0(n37),
    .dout1(n42)
  );


  SyncKnnCtrl
  i2
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n37),
    .dout1(n48)
  );


  SyncKnnQueue
  i35
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n33),
    .din0(n36),
    .dout0(n39)
  );


  SyncKnnQueue
  i36
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n34),
    .din0(n36),
    .dout0(n40)
  );


  SyncKnnQueue
  i37
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n35),
    .din0(n36),
    .dout0(n41)
  );


  SyncKnnQueue
  i38
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n38),
    .din0(n42),
    .dout0(n46)
  );


  SyncKnnQueue
  i39
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n39),
    .din0(n42),
    .dout0(n45)
  );


  SyncKnnQueue
  i40
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n40),
    .din0(n42),
    .dout0(n44)
  );


  SyncKnnQueue
  i41
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n41),
    .din0(n42),
    .dout0(n43)
  );


  SyncKnnQueue
  i42
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n46),
    .dout0(n47),
    .din0(n48)
  );


  SyncKnnQueue
  i43
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n45),
    .din0(n48),
    .dout0(n51)
  );


  SyncKnnQueue
  i44
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n44),
    .din0(n48),
    .dout0(n50)
  );


  SyncKnnQueue
  i45
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n43),
    .din0(n48),
    .dout0(n49)
  );


  SyncKnnQueue
  i8
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din1(n32),
    .din0(n36),
    .dout0(n38)
  );


  SyncOut
  out0
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[0]),
    .wr_data(acc_user_write_data[511:0]),
    .wr_en(acc_user_request_write[0]),
    .component_ready(streams_ready[4]),
    .done(done_wire[0]),
    .din0(n47)
  );


  SyncOut
  out1
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[1]),
    .wr_data(acc_user_write_data[1023:512]),
    .wr_en(acc_user_request_write[1]),
    .component_ready(streams_ready[5]),
    .done(done_wire[1]),
    .din0(n51)
  );


  SyncOut
  out2
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[2]),
    .wr_data(acc_user_write_data[1535:1024]),
    .wr_en(acc_user_request_write[2]),
    .component_ready(streams_ready[6]),
    .done(done_wire[2]),
    .din0(n50)
  );


  SyncOut
  out3
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[3]),
    .wr_data(acc_user_write_data[2047:1536]),
    .wr_en(acc_user_request_write[3]),
    .component_ready(streams_ready[7]),
    .done(done_wire[3]),
    .din0(n49)
  );


  SyncRegister
  i10
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n16),
    .din0(n19)
  );


  SyncRegister
  i11
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n15),
    .din0(n20)
  );


  SyncRegister
  i13
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n21),
    .dout0(n6)
  );


  SyncRegister
  i14
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n17),
    .dout0(n22)
  );


  SyncRegister
  i15
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n16),
    .dout0(n23)
  );


  SyncRegister
  i16
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n15),
    .dout0(n24)
  );


  SyncRegister
  i19
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n25),
    .din0(n6)
  );


  SyncRegister
  i20
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n22),
    .dout0(n30)
  );


  SyncRegister
  i28
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n23),
    .dout0(n29)
  );


  SyncRegister
  i29
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n24),
    .dout0(n28)
  );


  SyncRegister
  i3
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n14),
    .dout0(n18)
  );


  SyncRegister
  i30
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n25),
    .dout0(n27)
  );


  SyncRegister
  i31
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n30),
    .dout0(n32)
  );


  SyncRegister
  i32
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n29),
    .dout0(n33)
  );


  SyncRegister
  i33
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n28),
    .dout0(n34)
  );


  SyncRegister
  i34
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n27),
    .dout0(n35)
  );


  SyncRegister
  i4
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n19),
    .din0(n9)
  );


  SyncRegister
  i5
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n20),
    .din0(n8)
  );


  SyncRegister
  i6
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n21),
    .din0(n7)
  );


  SyncRegister
  i9
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n17),
    .din0(n18)
  );


  SyncSubI
  #(
    .ID(2),
    .IMMEDIATE(0)
  )
  i12
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n3),
    .din0(n9)
  );


  SyncSubI
  #(
    .ID(3),
    .IMMEDIATE(0)
  )
  i21
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n5),
    .din0(n8)
  );


  SyncSubI
  #(
    .ID(4),
    .IMMEDIATE(0)
  )
  i22
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .dout0(n4),
    .din0(n7)
  );


  SyncSubI
  #(
    .ID(1),
    .IMMEDIATE(0)
  )
  i7
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .din0(n14),
    .dout0(n2)
  );


endmodule
