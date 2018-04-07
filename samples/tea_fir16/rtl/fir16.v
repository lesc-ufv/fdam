
module fir16
(
  input clk,
  input rst,
  input [15:0] din,
  input rin,
  input [31:0] dconf,
  output [15:0] dout,
  output rout,
  input en
);

  wire [1-1:0] n64;
  wire [16-1:0] n63;
  wire [1-1:0] n62;
  wire [16-1:0] n61;
  wire [16-1:0] n46;
  wire [1-1:0] n45;
  wire [16-1:0] n9;
  wire [1-1:0] n18;
  wire [16-1:0] n17;
  wire [1-1:0] n12;
  wire [16-1:0] n11;
  wire [1-1:0] n10;
  wire [32-1:0] n3;
  wire [1-1:0] n2;
  wire [16-1:0] n1;
  wire [16-1:0] n16;
  wire [1-1:0] n15;
  wire [16-1:0] n38;
  wire [1-1:0] n37;
  wire [16-1:0] n8;
  wire [1-1:0] n7;
  wire [16-1:0] n4;
  wire [1-1:0] n0;
  wire [16-1:0] n42;
  wire [1-1:0] n41;
  wire [16-1:0] n28;
  wire [1-1:0] n27;
  wire [1-1:0] n24;
  wire [16-1:0] n23;
  wire [16-1:0] n22;
  wire [1-1:0] n21;
  wire [16-1:0] n26;
  wire [1-1:0] n25;
  wire [16-1:0] n60;
  wire [1-1:0] n59;
  wire [16-1:0] n44;
  wire [1-1:0] n43;
  wire [1-1:0] n20;
  wire [16-1:0] n19;
  wire [1-1:0] n32;
  wire [16-1:0] n31;
  wire [1-1:0] n30;
  wire [16-1:0] n29;
  wire [1-1:0] n50;
  wire [16-1:0] n49;
  wire [1-1:0] n39;
  wire [1-1:0] n58;
  wire [16-1:0] n57;
  wire [1-1:0] n56;
  wire [16-1:0] n55;
  wire [16-1:0] n40;
  wire [16-1:0] n14;
  wire [1-1:0] n13;
  wire [1-1:0] rdy_wire;
  wire [16-1:0] n66;
  wire [1-1:0] n65;
  wire [1-1:0] n6;
  wire [16-1:0] n5;
  wire [16-1:0] n48;
  wire [1-1:0] n47;
  wire [16-1:0] n36;
  wire [1-1:0] n35;
  wire [1-1:0] n54;
  wire [16-1:0] n53;
  wire [16-1:0] n52;
  wire [1-1:0] n51;
  wire [16-1:0] n34;
  wire [1-1:0] n33;

  assign n3 = dconf;
  assign n1 = din;
  assign n2 = rin;
  assign rout = n65;
  assign dout = n66;
  
  hebe_add
  #(
    .N(16)
  )
  i32
  (
    .clk(clk),
    .din1(n61),
    .din2(n46),
    .dout(n63),
    .en(en),
    .rin1(n62),
    .rin2(n45),
    .rout(n64),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i12
  (
    .clk(clk),
    .din1(n11),
    .din2(n9),
    .dout(n17),
    .en(en),
    .rin1(n12),
    .rin2(n10),
    .rout(n18),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(12)
  )
  i8
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n16),
    .en(en),
    .rin(n2),
    .rout(n15),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(6)
  )
  i20
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n38),
    .en(en),
    .rin(n2),
    .rout(n37),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(15)
  )
  i5
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n8),
    .en(en),
    .rin(n2),
    .rout(n7),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(16)
  )
  i1
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n4),
    .en(en),
    .rin(n2),
    .rout(n0),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(4)
  )
  i22
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n42),
    .en(en),
    .rin(n2),
    .rout(n41),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i15
  (
    .clk(clk),
    .din1(n23),
    .din2(n22),
    .dout(n28),
    .en(en),
    .rin1(n24),
    .rin2(n21),
    .rout(n27),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(10)
  )
  i10
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n26),
    .en(en),
    .rin(n2),
    .rout(n25),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i31
  (
    .clk(clk),
    .din1(n60),
    .din2(n44),
    .dout(n61),
    .en(en),
    .rin1(n59),
    .rin2(n43),
    .rout(n62),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(14)
  )
  i6
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n9),
    .en(en),
    .rin(n2),
    .rout(n10),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i14
  (
    .clk(clk),
    .din1(n19),
    .din2(n16),
    .dout(n23),
    .en(en),
    .rin1(n20),
    .rin2(n15),
    .rout(n24),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i17
  (
    .clk(clk),
    .din1(n31),
    .din2(n29),
    .dout(n49),
    .en(en),
    .rin1(n32),
    .rin2(n30),
    .rout(n50),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i29
  (
    .clk(clk),
    .din1(n55),
    .din2(n40),
    .dout(n57),
    .en(en),
    .rin1(n56),
    .rin2(n39),
    .rout(n58),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i13
  (
    .clk(clk),
    .din1(n17),
    .din2(n14),
    .dout(n19),
    .en(en),
    .rin1(n18),
    .rin2(n13),
    .rout(n20),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(9)
  )
  i11
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n29),
    .en(en),
    .rin(n2),
    .rout(n30),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(11)
  )
  i9
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n22),
    .en(en),
    .rin(n2),
    .rout(n21),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(5)
  )
  i21
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n40),
    .en(en),
    .rin(n2),
    .rout(n39),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i16
  (
    .clk(clk),
    .din1(n28),
    .din2(n26),
    .dout(n31),
    .en(en),
    .rin1(n27),
    .rin2(n25),
    .rout(n32),
    .rst(rst)
  );


  hebe_register
  #(
    .N(16)
  )
  i2
  (
    .clk(clk),
    .din(n4),
    .dout(n5),
    .en(en),
    .rin(n0),
    .rout(n6),
    .rst(rst)
  );

  hebe_add
  #(
    .N(16)
  )
  i33
  (
    .clk(clk),
    .din1(n63),
    .din2(n48),
    .dout(n66),
    .en(en),
    .rin1(n64),
    .rin2(n47),
    .rout(n65),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i27
  (
    .clk(clk),
    .din1(n52),
    .din2(n36),
    .dout(n53),
    .en(en),
    .rin1(n51),
    .rin2(n35),
    .rout(n54),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i0
  (
    .clk(clk),
    .din1(n5),
    .din2(n8),
    .dout(n11),
    .en(en),
    .rin1(n6),
    .rin2(n7),
    .rout(n12),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(3)
  )
  i23
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n44),
    .en(en),
    .rin(n2),
    .rout(n43),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i28
  (
    .clk(clk),
    .din1(n53),
    .din2(n38),
    .dout(n55),
    .en(en),
    .rin1(n54),
    .rin2(n37),
    .rout(n56),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i30
  (
    .clk(clk),
    .din1(n57),
    .din2(n42),
    .dout(n60),
    .en(en),
    .rin1(n58),
    .rin2(n41),
    .rout(n59),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(13)
  )
  i7
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n14),
    .en(en),
    .rin(n2),
    .rout(n13),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(7)
  )
  i19
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n36),
    .en(en),
    .rin(n2),
    .rout(n35),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(8)
  )
  i18
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n34),
    .en(en),
    .rin(n2),
    .rout(n33),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(1)
  )
  i25
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n48),
    .en(en),
    .rin(n2),
    .rout(n47),
    .rst(rst)
  );


  hebe_muli
  #(
    .N(16),
    .ID(2)
  )
  i24
  (
    .clk(clk),
    .dconf(n3),
    .din(n1),
    .dout(n46),
    .en(en),
    .rin(n2),
    .rout(n45),
    .rst(rst)
  );


  hebe_add
  #(
    .N(16)
  )
  i26
  (
    .clk(clk),
    .din1(n49),
    .din2(n34),
    .dout(n52),
    .en(en),
    .rin1(n50),
    .rin2(n33),
    .rout(n51),
    .rst(rst)
  );


endmodule
