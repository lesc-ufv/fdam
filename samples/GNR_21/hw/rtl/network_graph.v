
module network_graph
(
  input clk,
  input rst,
  input start,
  input reset_nos,
  input start_s0,
  input start_s1,
  input [21-1:0] init_state,
  output [21-1:0] s0,
  output [21-1:0] s1
);

  wire akt_s0;
  wire akt_s1;
  wire bcatenin_s0;
  wire bcatenin_s1;
  wire ctl_s0;
  wire ctl_s1;
  wire cyclind1_s0;
  wire cyclind1_s1;
  wire gsk3b_s0;
  wire gsk3b_s1;
  wire ifng_s0;
  wire ifng_s1;
  wire ikk_s0;
  wire ikk_s1;
  wire il10_s0;
  wire il10_s1;
  wire jak_s0;
  wire jak_s1;
  wire jnk_s0;
  wire jnk_s1;
  wire jun_s0;
  wire jun_s1;
  wire mac_s0;
  wire mac_s1;
  wire mdm2_s0;
  wire mdm2_s1;
  wire nfkb_s0;
  wire nfkb_s1;
  wire p21_s0;
  wire p21_s1;
  wire p53_s0;
  wire p53_s1;
  wire pten_s0;
  wire pten_s1;
  wire socs_s0;
  wire socs_s1;
  wire stat3_s0;
  wire stat3_s1;
  wire tnfr_s0;
  wire tnfr_s1;
  wire proliferation_s0;
  wire proliferation_s1;

  no_akt
  _no_akt
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[0]),
    .pten_s0(pten_s0),
    .pten_s1(pten_s1),
    .s0(s0[0]),
    .s1(s1[0]),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1)
  );


  no_bcatenin
  _no_bcatenin
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[1]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .s0(s0[1]),
    .s1(s1[1]),
    .bcatenin_s0(bcatenin_s0),
    .bcatenin_s1(bcatenin_s1)
  );


  no_ctl
  _no_ctl
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[2]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1),
    .s0(s0[2]),
    .s1(s1[2]),
    .ctl_s0(ctl_s0),
    .ctl_s1(ctl_s1)
  );


  no_cyclind1
  _no_cyclind1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[3]),
    .bcatenin_s0(bcatenin_s0),
    .bcatenin_s1(bcatenin_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .jun_s0(jun_s0),
    .jun_s1(jun_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .s0(s0[3]),
    .s1(s1[3]),
    .cyclind1_s0(cyclind1_s0),
    .cyclind1_s1(cyclind1_s1)
  );


  no_gsk3b
  _no_gsk3b
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[4]),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .s0(s0[4]),
    .s1(s1[4]),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1)
  );


  no_ifng
  _no_ifng
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[5]),
    .ctl_s0(ctl_s0),
    .ctl_s1(ctl_s1),
    .s0(s0[5]),
    .s1(s1[5]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1)
  );


  no_ikk
  _no_ikk
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[6]),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .s0(s0[6]),
    .s1(s1[6]),
    .ikk_s0(ikk_s0),
    .ikk_s1(ikk_s1)
  );


  no_il10
  _no_il10
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[7]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1),
    .s0(s0[7]),
    .s1(s1[7]),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1)
  );


  no_jak
  _no_jak
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[8]),
    .socs_s0(socs_s0),
    .socs_s1(socs_s1),
    .s0(s0[8]),
    .s1(s1[8]),
    .jak_s0(jak_s0),
    .jak_s1(jak_s1)
  );


  no_jnk
  _no_jnk
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[9]),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .s0(s0[9]),
    .s1(s1[9]),
    .jnk_s0(jnk_s0),
    .jnk_s1(jnk_s1)
  );


  no_jun
  _no_jun
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[10]),
    .jnk_s0(jnk_s0),
    .jnk_s1(jnk_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .s0(s0[10]),
    .s1(s1[10]),
    .jun_s0(jun_s0),
    .jun_s1(jun_s1)
  );


  no_mac
  _no_mac
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[11]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1),
    .s0(s0[11]),
    .s1(s1[11]),
    .mac_s0(mac_s0),
    .mac_s1(mac_s1)
  );


  no_mdm2
  _no_mdm2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[12]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .s0(s0[12]),
    .s1(s1[12]),
    .mdm2_s0(mdm2_s0),
    .mdm2_s1(mdm2_s1)
  );


  no_nfkb
  _no_nfkb
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[13]),
    .ikk_s0(ikk_s0),
    .ikk_s1(ikk_s1),
    .s0(s0[13]),
    .s1(s1[13]),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1)
  );


  no_p21
  _no_p21
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[14]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .s0(s0[14]),
    .s1(s1[14]),
    .p21_s0(p21_s0),
    .p21_s1(p21_s1)
  );


  no_p53
  _no_p53
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[15]),
    .pten_s0(pten_s0),
    .pten_s1(pten_s1),
    .jnk_s0(jnk_s0),
    .jnk_s1(jnk_s1),
    .mdm2_s0(mdm2_s0),
    .mdm2_s1(mdm2_s1),
    .s0(s0[15]),
    .s1(s1[15]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1)
  );


  no_pten
  _no_pten
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[16]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .jun_s0(jun_s0),
    .jun_s1(jun_s1),
    .s0(s0[16]),
    .s1(s1[16]),
    .pten_s0(pten_s0),
    .pten_s1(pten_s1)
  );


  no_socs
  _no_socs
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[17]),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .s0(s0[17]),
    .s1(s1[17]),
    .socs_s0(socs_s0),
    .socs_s1(socs_s1)
  );


  no_stat3
  _no_stat3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[18]),
    .jak_s0(jak_s0),
    .jak_s1(jak_s1),
    .s0(s0[18]),
    .s1(s1[18]),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1)
  );


  no_tnfr
  _no_tnfr
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[19]),
    .mac_s0(mac_s0),
    .mac_s1(mac_s1),
    .s0(s0[19]),
    .s1(s1[19]),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1)
  );


  no_proliferation
  _no_proliferation
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[20]),
    .cyclind1_s0(cyclind1_s0),
    .cyclind1_s1(cyclind1_s1),
    .p21_s0(p21_s0),
    .p21_s1(p21_s1),
    .s0(s0[20]),
    .s1(s1[20]),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1)
  );


endmodule
