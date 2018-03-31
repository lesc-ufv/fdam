
module acc_user_0 #
(
  parameter DATA_WIDTH = 32,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1
)
(
  input clk,
  input rst,
  input start,
  input [16-1:0] acc_user_done_rd_data,
  input [16-1:0] acc_user_done_wr_data,
  input [16-1:0] acc_user_available_read,
  input [8192-1:0] acc_user_read_data,
  output [16-1:0] acc_user_request_read,
  input [16-1:0] acc_user_read_data_valid,
  input [16-1:0] acc_user_available_write,
  output [8192-1:0] acc_user_write_data,
  output [16-1:0] acc_user_request_write,
  output reg acc_user_done
);

  wire [34-1:0] n0;
  wire [34-1:0] n1;
  wire [34-1:0] n10;
  wire [34-1:0] n11;
  wire [34-1:0] n12;
  wire [34-1:0] n13;
  wire [34-1:0] n14;
  wire [34-1:0] n15;
  wire [34-1:0] n2;
  wire [34-1:0] n3;
  wire [34-1:0] n4;
  wire [34-1:0] n5;
  wire [34-1:0] n6;
  wire [34-1:0] n7;
  wire [34-1:0] n8;
  wire [34-1:0] n9;
  wire [32-1:0] streams_ready;
  wire en;
  assign en = &streams_ready;
  wire [16-1:0] done_wire;

  always @(posedge clk) begin
    acc_user_done <= &done_wire;
  end


  SyncIn
  i0
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
    .dout0(n0)
  );


  SyncIn
  i1
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
    .dout0(n1)
  );


  SyncIn
  i10
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
    .dout0(n10)
  );


  SyncIn
  i11
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
    .dout0(n11)
  );


  SyncIn
  i12
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[4]),
    .rd_available(acc_user_available_read[4]),
    .rd_valid(acc_user_read_data_valid[4]),
    .rd_data(acc_user_read_data[2559:2048]),
    .rd_en(acc_user_request_read[4]),
    .component_ready(streams_ready[4]),
    .dout0(n15)
  );


  SyncIn
  i13
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[5]),
    .rd_available(acc_user_available_read[5]),
    .rd_valid(acc_user_read_data_valid[5]),
    .rd_data(acc_user_read_data[3071:2560]),
    .rd_en(acc_user_request_read[5]),
    .component_ready(streams_ready[5]),
    .dout0(n14)
  );


  SyncIn
  i14
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[6]),
    .rd_available(acc_user_available_read[6]),
    .rd_valid(acc_user_read_data_valid[6]),
    .rd_data(acc_user_read_data[3583:3072]),
    .rd_en(acc_user_request_read[6]),
    .component_ready(streams_ready[6]),
    .dout0(n13)
  );


  SyncIn
  i15
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[7]),
    .rd_available(acc_user_available_read[7]),
    .rd_valid(acc_user_read_data_valid[7]),
    .rd_data(acc_user_read_data[4095:3584]),
    .rd_en(acc_user_request_read[7]),
    .component_ready(streams_ready[7]),
    .dout0(n12)
  );


  SyncIn
  i2
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[8]),
    .rd_available(acc_user_available_read[8]),
    .rd_valid(acc_user_read_data_valid[8]),
    .rd_data(acc_user_read_data[4607:4096]),
    .rd_en(acc_user_request_read[8]),
    .component_ready(streams_ready[8]),
    .dout0(n2)
  );


  SyncIn
  i3
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[9]),
    .rd_available(acc_user_available_read[9]),
    .rd_valid(acc_user_read_data_valid[9]),
    .rd_data(acc_user_read_data[5119:4608]),
    .rd_en(acc_user_request_read[9]),
    .component_ready(streams_ready[9]),
    .dout0(n3)
  );


  SyncIn
  i4
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[10]),
    .rd_available(acc_user_available_read[10]),
    .rd_valid(acc_user_read_data_valid[10]),
    .rd_data(acc_user_read_data[5631:5120]),
    .rd_en(acc_user_request_read[10]),
    .component_ready(streams_ready[10]),
    .dout0(n7)
  );


  SyncIn
  i5
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[11]),
    .rd_available(acc_user_available_read[11]),
    .rd_valid(acc_user_read_data_valid[11]),
    .rd_data(acc_user_read_data[6143:5632]),
    .rd_en(acc_user_request_read[11]),
    .component_ready(streams_ready[11]),
    .dout0(n6)
  );


  SyncIn
  i6
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[12]),
    .rd_available(acc_user_available_read[12]),
    .rd_valid(acc_user_read_data_valid[12]),
    .rd_data(acc_user_read_data[6655:6144]),
    .rd_en(acc_user_request_read[12]),
    .component_ready(streams_ready[12]),
    .dout0(n5)
  );


  SyncIn
  i7
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[13]),
    .rd_available(acc_user_available_read[13]),
    .rd_valid(acc_user_read_data_valid[13]),
    .rd_data(acc_user_read_data[7167:6656]),
    .rd_en(acc_user_request_read[13]),
    .component_ready(streams_ready[13]),
    .dout0(n4)
  );


  SyncIn
  i8
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[14]),
    .rd_available(acc_user_available_read[14]),
    .rd_valid(acc_user_read_data_valid[14]),
    .rd_data(acc_user_read_data[7679:7168]),
    .rd_en(acc_user_request_read[14]),
    .component_ready(streams_ready[14]),
    .dout0(n8)
  );


  SyncIn
  i9
  (
    .clk(clk),
    .rst(rst),
    .en(en),
    .start(start),
    .rd_done(acc_user_done_rd_data[15]),
    .rd_available(acc_user_available_read[15]),
    .rd_valid(acc_user_read_data_valid[15]),
    .rd_data(acc_user_read_data[8191:7680]),
    .rd_en(acc_user_request_read[15]),
    .component_ready(streams_ready[15]),
    .dout0(n9)
  );


  SyncOut
  i16
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[0]),
    .wr_data(acc_user_write_data[511:0]),
    .wr_en(acc_user_request_write[0]),
    .component_ready(streams_ready[16]),
    .done(done_wire[0]),
    .din0(n0)
  );


  SyncOut
  i17
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[1]),
    .wr_data(acc_user_write_data[1023:512]),
    .wr_en(acc_user_request_write[1]),
    .component_ready(streams_ready[17]),
    .done(done_wire[1]),
    .din0(n1)
  );


  SyncOut
  i18
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[2]),
    .wr_data(acc_user_write_data[1535:1024]),
    .wr_en(acc_user_request_write[2]),
    .component_ready(streams_ready[18]),
    .done(done_wire[2]),
    .din0(n2)
  );


  SyncOut
  i19
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[3]),
    .wr_data(acc_user_write_data[2047:1536]),
    .wr_en(acc_user_request_write[3]),
    .component_ready(streams_ready[19]),
    .done(done_wire[3]),
    .din0(n3)
  );


  SyncOut
  i20
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[4]),
    .wr_data(acc_user_write_data[2559:2048]),
    .wr_en(acc_user_request_write[4]),
    .component_ready(streams_ready[20]),
    .done(done_wire[4]),
    .din0(n7)
  );


  SyncOut
  i21
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[5]),
    .wr_data(acc_user_write_data[3071:2560]),
    .wr_en(acc_user_request_write[5]),
    .component_ready(streams_ready[21]),
    .done(done_wire[5]),
    .din0(n6)
  );


  SyncOut
  i22
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[6]),
    .wr_data(acc_user_write_data[3583:3072]),
    .wr_en(acc_user_request_write[6]),
    .component_ready(streams_ready[22]),
    .done(done_wire[6]),
    .din0(n5)
  );


  SyncOut
  i23
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[7]),
    .wr_data(acc_user_write_data[4095:3584]),
    .wr_en(acc_user_request_write[7]),
    .component_ready(streams_ready[23]),
    .done(done_wire[7]),
    .din0(n4)
  );


  SyncOut
  i24
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[8]),
    .wr_data(acc_user_write_data[4607:4096]),
    .wr_en(acc_user_request_write[8]),
    .component_ready(streams_ready[24]),
    .done(done_wire[8]),
    .din0(n8)
  );


  SyncOut
  i25
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[9]),
    .wr_data(acc_user_write_data[5119:4608]),
    .wr_en(acc_user_request_write[9]),
    .component_ready(streams_ready[25]),
    .done(done_wire[9]),
    .din0(n9)
  );


  SyncOut
  i26
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[10]),
    .wr_data(acc_user_write_data[5631:5120]),
    .wr_en(acc_user_request_write[10]),
    .component_ready(streams_ready[26]),
    .done(done_wire[10]),
    .din0(n10)
  );


  SyncOut
  i27
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[11]),
    .wr_data(acc_user_write_data[6143:5632]),
    .wr_en(acc_user_request_write[11]),
    .component_ready(streams_ready[27]),
    .done(done_wire[11]),
    .din0(n11)
  );


  SyncOut
  i28
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[12]),
    .wr_data(acc_user_write_data[6655:6144]),
    .wr_en(acc_user_request_write[12]),
    .component_ready(streams_ready[28]),
    .done(done_wire[12]),
    .din0(n15)
  );


  SyncOut
  i29
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[13]),
    .wr_data(acc_user_write_data[7167:6656]),
    .wr_en(acc_user_request_write[13]),
    .component_ready(streams_ready[29]),
    .done(done_wire[13]),
    .din0(n14)
  );


  SyncOut
  i30
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[14]),
    .wr_data(acc_user_write_data[7679:7168]),
    .wr_en(acc_user_request_write[14]),
    .component_ready(streams_ready[30]),
    .done(done_wire[14]),
    .din0(n13)
  );


  SyncOut
  i31
  (
    .clk(clk),
    .rst(rst),
    .start(start),
    .en(en),
    .wr_available(acc_user_available_write[15]),
    .wr_data(acc_user_write_data[8191:7680]),
    .wr_en(acc_user_request_write[15]),
    .component_ready(streams_ready[31]),
    .done(done_wire[15]),
    .din0(n12)
  );


endmodule
