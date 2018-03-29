
module network_graph
(
  input clk,
  input rst,
  input start,
  input reset_nos,
  input start_s0,
  input start_s1,
  input [70-1:0] init_state,
  output [70-1:0] s0,
  output [70-1:0] s1
);

  wire atm_s0;
  wire atm_s1;
  wire ask1_s0;
  wire ask1_s1;
  wire akt_s0;
  wire akt_s1;
  wire bax_s0;
  wire bax_s1;
  wire apc_s0;
  wire apc_s1;
  wire bcatenin_s0;
  wire bcatenin_s1;
  wire bcl2_s0;
  wire bcl2_s1;
  wire casp3_s0;
  wire casp3_s1;
  wire casp8_s0;
  wire casp8_s1;
  wire casp9_s0;
  wire casp9_s1;
  wire ceramide_s0;
  wire ceramide_s1;
  wire cflip_s0;
  wire cflip_s1;
  wire cox2_s0;
  wire cox2_s1;
  wire cyclind1_s0;
  wire cyclind1_s1;
  wire cytc_s0;
  wire cytc_s1;
  wire ep2_s0;
  wire ep2_s1;
  wire erk_s0;
  wire erk_s1;
  wire fas_s0;
  wire fas_s1;
  wire fadd_s0;
  wire fadd_s1;
  wire fos_s0;
  wire fos_s1;
  wire gp130_s0;
  wire gp130_s1;
  wire gsk3b_s0;
  wire gsk3b_s1;
  wire iap_s0;
  wire iap_s1;
  wire ikb_s0;
  wire ikb_s1;
  wire ikk_s0;
  wire ikk_s1;
  wire jak_s0;
  wire jak_s1;
  wire jnk_s0;
  wire jnk_s1;
  wire jun_s0;
  wire jun_s1;
  wire mdm2_s0;
  wire mdm2_s1;
  wire mek_s0;
  wire mek_s1;
  wire mekk1_s0;
  wire mekk1_s1;
  wire momp_s0;
  wire momp_s1;
  wire nfkb_s0;
  wire nfkb_s1;
  wire p21_s0;
  wire p21_s1;
  wire p53_s0;
  wire p53_s1;
  wire pge2_s0;
  wire pge2_s1;
  wire pi3k_s0;
  wire pi3k_s1;
  wire pp2a_s0;
  wire pp2a_s1;
  wire pten_s0;
  wire pten_s1;
  wire raf_s0;
  wire raf_s1;
  wire ras_s0;
  wire ras_s1;
  wire ros_s0;
  wire ros_s1;
  wire sod_s0;
  wire sod_s1;
  wire s1p_s0;
  wire s1p_s1;
  wire smac_s0;
  wire smac_s1;
  wire smad_s0;
  wire smad_s1;
  wire smad7_s0;
  wire smad7_s1;
  wire smase_s0;
  wire smase_s1;
  wire sphk1_s0;
  wire sphk1_s1;
  wire stat3_s0;
  wire stat3_s1;
  wire socs_s0;
  wire socs_s1;
  wire tbid_s0;
  wire tbid_s1;
  wire tgfr_s0;
  wire tgfr_s1;
  wire tnfr_s0;
  wire tnfr_s1;
  wire treg_s0;
  wire treg_s1;
  wire tnfa_s0;
  wire tnfa_s1;
  wire th2_s0;
  wire th2_s1;
  wire th1_s0;
  wire th1_s1;
  wire tgfb_s0;
  wire tgfb_s1;
  wire mac_s0;
  wire mac_s1;
  wire il6_s0;
  wire il6_s1;
  wire il4_s0;
  wire il4_s1;
  wire il12_s0;
  wire il12_s1;
  wire il10_s0;
  wire il10_s1;
  wire ifng_s0;
  wire ifng_s1;
  wire ctl_s0;
  wire ctl_s1;
  wire dc_s0;
  wire dc_s1;
  wire ccl2_s0;
  wire ccl2_s1;
  wire proliferation_s0;
  wire proliferation_s1;
  wire apoptosis_s0;
  wire apoptosis_s1;

  no_atm
  _no_atm
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[0]),
    .ros_s0(ros_s0),
    .ros_s1(ros_s1),
    .s0(s0[0]),
    .s1(s1[0]),
    .atm_s0(atm_s0),
    .atm_s1(atm_s1)
  );


  no_ask1
  _no_ask1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[1]),
    .ros_s0(ros_s0),
    .ros_s1(ros_s1),
    .p21_s0(p21_s0),
    .p21_s1(p21_s1),
    .s0(s0[1]),
    .s1(s1[1]),
    .ask1_s0(ask1_s0),
    .ask1_s1(ask1_s1)
  );


  no_akt
  _no_akt
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[2]),
    .pi3k_s0(pi3k_s0),
    .pi3k_s1(pi3k_s1),
    .pp2a_s0(pp2a_s0),
    .pp2a_s1(pp2a_s1),
    .casp3_s0(casp3_s0),
    .casp3_s1(casp3_s1),
    .s0(s0[2]),
    .s1(s1[2]),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1)
  );


  no_bax
  _no_bax
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[3]),
    .tbid_s0(tbid_s0),
    .tbid_s1(tbid_s1),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .pp2a_s0(pp2a_s0),
    .pp2a_s1(pp2a_s1),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .s0(s0[3]),
    .s1(s1[3]),
    .bax_s0(bax_s0),
    .bax_s1(bax_s1)
  );


  no_apc
  _no_apc
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[4]),
    .s0(s0[4]),
    .s1(s1[4]),
    .apc_s0(apc_s0),
    .apc_s1(apc_s1)
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
    .init_state(init_state[5]),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .apc_s0(apc_s0),
    .apc_s1(apc_s1),
    .s0(s0[5]),
    .s1(s1[5]),
    .bcatenin_s0(bcatenin_s0),
    .bcatenin_s1(bcatenin_s1)
  );


  no_bcl2
  _no_bcl2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[6]),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .pp2a_s0(pp2a_s0),
    .pp2a_s1(pp2a_s1),
    .s0(s0[6]),
    .s1(s1[6]),
    .bcl2_s0(bcl2_s0),
    .bcl2_s1(bcl2_s1)
  );


  no_casp3
  _no_casp3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[7]),
    .casp8_s0(casp8_s0),
    .casp8_s1(casp8_s1),
    .casp9_s0(casp9_s0),
    .casp9_s1(casp9_s1),
    .iap_s0(iap_s0),
    .iap_s1(iap_s1),
    .s0(s0[7]),
    .s1(s1[7]),
    .casp3_s0(casp3_s0),
    .casp3_s1(casp3_s1)
  );


  no_casp8
  _no_casp8
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[8]),
    .fadd_s0(fadd_s0),
    .fadd_s1(fadd_s1),
    .cflip_s0(cflip_s0),
    .cflip_s1(cflip_s1),
    .p21_s0(p21_s0),
    .p21_s1(p21_s1),
    .s0(s0[8]),
    .s1(s1[8]),
    .casp8_s0(casp8_s0),
    .casp8_s1(casp8_s1)
  );


  no_casp9
  _no_casp9
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[9]),
    .cytc_s0(cytc_s0),
    .cytc_s1(cytc_s1),
    .iap_s0(iap_s0),
    .iap_s1(iap_s1),
    .p21_s0(p21_s0),
    .p21_s1(p21_s1),
    .s0(s0[9]),
    .s1(s1[9]),
    .casp9_s0(casp9_s0),
    .casp9_s1(casp9_s1)
  );


  no_ceramide
  _no_ceramide
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[10]),
    .smase_s0(smase_s0),
    .smase_s1(smase_s1),
    .sphk1_s0(sphk1_s0),
    .sphk1_s1(sphk1_s1),
    .s0(s0[10]),
    .s1(s1[10]),
    .ceramide_s0(ceramide_s0),
    .ceramide_s1(ceramide_s1)
  );


  no_cflip
  _no_cflip
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[11]),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .s0(s0[11]),
    .s1(s1[11]),
    .cflip_s0(cflip_s0),
    .cflip_s1(cflip_s1)
  );


  no_cox2
  _no_cox2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[12]),
    .s1p_s0(s1p_s0),
    .s1p_s1(s1p_s1),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .s0(s0[12]),
    .s1(s1[12]),
    .cox2_s0(cox2_s0),
    .cox2_s1(cox2_s1)
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
    .init_state(init_state[13]),
    .bcatenin_s0(bcatenin_s0),
    .bcatenin_s1(bcatenin_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .jun_s0(jun_s0),
    .jun_s1(jun_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .s0(s0[13]),
    .s1(s1[13]),
    .cyclind1_s0(cyclind1_s0),
    .cyclind1_s1(cyclind1_s1)
  );


  no_cytc
  _no_cytc
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[14]),
    .momp_s0(momp_s0),
    .momp_s1(momp_s1),
    .s0(s0[14]),
    .s1(s1[14]),
    .cytc_s0(cytc_s0),
    .cytc_s1(cytc_s1)
  );


  no_ep2
  _no_ep2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[15]),
    .pge2_s0(pge2_s0),
    .pge2_s1(pge2_s1),
    .s0(s0[15]),
    .s1(s1[15]),
    .ep2_s0(ep2_s0),
    .ep2_s1(ep2_s1)
  );


  no_erk
  _no_erk
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[16]),
    .mek_s0(mek_s0),
    .mek_s1(mek_s1),
    .s0(s0[16]),
    .s1(s1[16]),
    .erk_s0(erk_s0),
    .erk_s1(erk_s1)
  );


  no_fas
  _no_fas
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[17]),
    .ctl_s0(ctl_s0),
    .ctl_s1(ctl_s1),
    .s0(s0[17]),
    .s1(s1[17]),
    .fas_s0(fas_s0),
    .fas_s1(fas_s1)
  );


  no_fadd
  _no_fadd
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[18]),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .fas_s0(fas_s0),
    .fas_s1(fas_s1),
    .s0(s0[18]),
    .s1(s1[18]),
    .fadd_s0(fadd_s0),
    .fadd_s1(fadd_s1)
  );


  no_fos
  _no_fos
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[19]),
    .erk_s0(erk_s0),
    .erk_s1(erk_s1),
    .s0(s0[19]),
    .s1(s1[19]),
    .fos_s0(fos_s0),
    .fos_s1(fos_s1)
  );


  no_gp130
  _no_gp130
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[20]),
    .il6_s0(il6_s0),
    .il6_s1(il6_s1),
    .s0(s0[20]),
    .s1(s1[20]),
    .gp130_s0(gp130_s0),
    .gp130_s1(gp130_s1)
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
    .init_state(init_state[21]),
    .ep2_s0(ep2_s0),
    .ep2_s1(ep2_s1),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .s0(s0[21]),
    .s1(s1[21]),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1)
  );


  no_iap
  _no_iap
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[22]),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .smac_s0(smac_s0),
    .smac_s1(smac_s1),
    .s0(s0[22]),
    .s1(s1[22]),
    .iap_s0(iap_s0),
    .iap_s1(iap_s1)
  );


  no_ikb
  _no_ikb
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[23]),
    .ikk_s0(ikk_s0),
    .ikk_s1(ikk_s1),
    .s0(s0[23]),
    .s1(s1[23]),
    .ikb_s0(ikb_s0),
    .ikb_s1(ikb_s1)
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
    .init_state(init_state[24]),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .s1p_s0(s1p_s0),
    .s1p_s1(s1p_s1),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .s0(s0[24]),
    .s1(s1[24]),
    .ikk_s0(ikk_s0),
    .ikk_s1(ikk_s1)
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
    .init_state(init_state[25]),
    .gp130_s0(gp130_s0),
    .gp130_s1(gp130_s1),
    .socs_s0(socs_s0),
    .socs_s1(socs_s1),
    .s0(s0[25]),
    .s1(s1[25]),
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
    .init_state(init_state[26]),
    .ask1_s0(ask1_s0),
    .ask1_s1(ask1_s1),
    .mekk1_s0(mekk1_s0),
    .mekk1_s1(mekk1_s1),
    .s0(s0[26]),
    .s1(s1[26]),
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
    .init_state(init_state[27]),
    .bcatenin_s0(bcatenin_s0),
    .bcatenin_s1(bcatenin_s1),
    .erk_s0(erk_s0),
    .erk_s1(erk_s1),
    .jnk_s0(jnk_s0),
    .jnk_s1(jnk_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .s0(s0[27]),
    .s1(s1[27]),
    .jun_s0(jun_s0),
    .jun_s1(jun_s1)
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
    .init_state(init_state[28]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .atm_s0(atm_s0),
    .atm_s1(atm_s1),
    .s0(s0[28]),
    .s1(s1[28]),
    .mdm2_s0(mdm2_s0),
    .mdm2_s1(mdm2_s1)
  );


  no_mek
  _no_mek
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[29]),
    .raf_s0(raf_s0),
    .raf_s1(raf_s1),
    .ros_s0(ros_s0),
    .ros_s1(ros_s1),
    .s0(s0[29]),
    .s1(s1[29]),
    .mek_s0(mek_s0),
    .mek_s1(mek_s1)
  );


  no_mekk1
  _no_mekk1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[30]),
    .ceramide_s0(ceramide_s0),
    .ceramide_s1(ceramide_s1),
    .tgfr_s0(tgfr_s0),
    .tgfr_s1(tgfr_s1),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .s0(s0[30]),
    .s1(s1[30]),
    .mekk1_s0(mekk1_s0),
    .mekk1_s1(mekk1_s1)
  );


  no_momp
  _no_momp
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[31]),
    .bax_s0(bax_s0),
    .bax_s1(bax_s1),
    .tbid_s0(tbid_s0),
    .tbid_s1(tbid_s1),
    .ceramide_s0(ceramide_s0),
    .ceramide_s1(ceramide_s1),
    .bcl2_s0(bcl2_s0),
    .bcl2_s1(bcl2_s1),
    .s0(s0[31]),
    .s1(s1[31]),
    .momp_s0(momp_s0),
    .momp_s1(momp_s1)
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
    .init_state(init_state[32]),
    .ikb_s0(ikb_s0),
    .ikb_s1(ikb_s1),
    .s0(s0[32]),
    .s1(s1[32]),
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
    .init_state(init_state[33]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .smad_s0(smad_s0),
    .smad_s1(smad_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .casp3_s0(casp3_s0),
    .casp3_s1(casp3_s1),
    .s0(s0[33]),
    .s1(s1[33]),
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
    .init_state(init_state[34]),
    .pten_s0(pten_s0),
    .pten_s1(pten_s1),
    .jnk_s0(jnk_s0),
    .jnk_s1(jnk_s1),
    .atm_s0(atm_s0),
    .atm_s1(atm_s1),
    .mdm2_s0(mdm2_s0),
    .mdm2_s1(mdm2_s1),
    .s0(s0[34]),
    .s1(s1[34]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1)
  );


  no_pge2
  _no_pge2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[35]),
    .cox2_s0(cox2_s0),
    .cox2_s1(cox2_s1),
    .s0(s0[35]),
    .s1(s1[35]),
    .pge2_s0(pge2_s0),
    .pge2_s1(pge2_s1)
  );


  no_pi3k
  _no_pi3k
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[36]),
    .ep2_s0(ep2_s0),
    .ep2_s1(ep2_s1),
    .ras_s0(ras_s0),
    .ras_s1(ras_s1),
    .pten_s0(pten_s0),
    .pten_s1(pten_s1),
    .s0(s0[36]),
    .s1(s1[36]),
    .pi3k_s0(pi3k_s0),
    .pi3k_s1(pi3k_s1)
  );


  no_pp2a
  _no_pp2a
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[37]),
    .ceramide_s0(ceramide_s0),
    .ceramide_s1(ceramide_s1),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .s0(s0[37]),
    .s1(s1[37]),
    .pp2a_s0(pp2a_s0),
    .pp2a_s1(pp2a_s1)
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
    .init_state(init_state[38]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .jun_s0(jun_s0),
    .jun_s1(jun_s1),
    .s0(s0[38]),
    .s1(s1[38]),
    .pten_s0(pten_s0),
    .pten_s1(pten_s1)
  );


  no_raf
  _no_raf
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[39]),
    .ceramide_s0(ceramide_s0),
    .ceramide_s1(ceramide_s1),
    .ras_s0(ras_s0),
    .ras_s1(ras_s1),
    .s0(s0[39]),
    .s1(s1[39]),
    .raf_s0(raf_s0),
    .raf_s1(raf_s1)
  );


  no_ras
  _no_ras
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[40]),
    .ep2_s0(ep2_s0),
    .ep2_s1(ep2_s1),
    .gp130_s0(gp130_s0),
    .gp130_s1(gp130_s1),
    .s0(s0[40]),
    .s1(s1[40]),
    .ras_s0(ras_s0),
    .ras_s1(ras_s1)
  );


  no_ros
  _no_ros
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[41]),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .sod_s0(sod_s0),
    .sod_s1(sod_s1),
    .s0(s0[41]),
    .s1(s1[41]),
    .ros_s0(ros_s0),
    .ros_s1(ros_s1)
  );


  no_sod
  _no_sod
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[42]),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .s0(s0[42]),
    .s1(s1[42]),
    .sod_s0(sod_s0),
    .sod_s1(sod_s1)
  );


  no_s1p
  _no_s1p
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[43]),
    .sphk1_s0(sphk1_s0),
    .sphk1_s1(sphk1_s1),
    .s0(s0[43]),
    .s1(s1[43]),
    .s1p_s0(s1p_s0),
    .s1p_s1(s1p_s1)
  );


  no_smac
  _no_smac
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[44]),
    .momp_s0(momp_s0),
    .momp_s1(momp_s1),
    .s0(s0[44]),
    .s1(s1[44]),
    .smac_s0(smac_s0),
    .smac_s1(smac_s1)
  );


  no_smad
  _no_smad
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[45]),
    .tgfr_s0(tgfr_s0),
    .tgfr_s1(tgfr_s1),
    .jun_s0(jun_s0),
    .jun_s1(jun_s1),
    .s0(s0[45]),
    .s1(s1[45]),
    .smad_s0(smad_s0),
    .smad_s1(smad_s1)
  );


  no_smad7
  _no_smad7
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[46]),
    .smad_s0(smad_s0),
    .smad_s1(smad_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .s0(s0[46]),
    .s1(s1[46]),
    .smad7_s0(smad7_s0),
    .smad7_s1(smad7_s1)
  );


  no_smase
  _no_smase
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[47]),
    .p53_s0(p53_s0),
    .p53_s1(p53_s1),
    .fadd_s0(fadd_s0),
    .fadd_s1(fadd_s1),
    .s0(s0[47]),
    .s1(s1[47]),
    .smase_s0(smase_s0),
    .smase_s1(smase_s1)
  );


  no_sphk1
  _no_sphk1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[48]),
    .erk_s0(erk_s0),
    .erk_s1(erk_s1),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1),
    .s0(s0[48]),
    .s1(s1[48]),
    .sphk1_s0(sphk1_s0),
    .sphk1_s1(sphk1_s1)
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
    .init_state(init_state[49]),
    .jak_s0(jak_s0),
    .jak_s1(jak_s1),
    .s0(s0[49]),
    .s1(s1[49]),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1)
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
    .init_state(init_state[50]),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .s0(s0[50]),
    .s1(s1[50]),
    .socs_s0(socs_s0),
    .socs_s1(socs_s1)
  );


  no_tbid
  _no_tbid
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[51]),
    .casp8_s0(casp8_s0),
    .casp8_s1(casp8_s1),
    .bcl2_s0(bcl2_s0),
    .bcl2_s1(bcl2_s1),
    .s0(s0[51]),
    .s1(s1[51]),
    .tbid_s0(tbid_s0),
    .tbid_s1(tbid_s1)
  );


  no_tgfr
  _no_tgfr
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[52]),
    .tgfb_s0(tgfb_s0),
    .tgfb_s1(tgfb_s1),
    .smad7_s0(smad7_s0),
    .smad7_s1(smad7_s1),
    .s0(s0[52]),
    .s1(s1[52]),
    .tgfr_s0(tgfr_s0),
    .tgfr_s1(tgfr_s1)
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
    .init_state(init_state[53]),
    .tnfa_s0(tnfa_s0),
    .tnfa_s1(tnfa_s1),
    .s0(s0[53]),
    .s1(s1[53]),
    .tnfr_s0(tnfr_s0),
    .tnfr_s1(tnfr_s1)
  );


  no_treg
  _no_treg
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[54]),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1),
    .dc_s0(dc_s0),
    .dc_s1(dc_s1),
    .il6_s0(il6_s0),
    .il6_s1(il6_s1),
    .s0(s0[54]),
    .s1(s1[54]),
    .treg_s0(treg_s0),
    .treg_s1(treg_s1)
  );


  no_tnfa
  _no_tnfa
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[55]),
    .mac_s0(mac_s0),
    .mac_s1(mac_s1),
    .s0(s0[55]),
    .s1(s1[55]),
    .tnfa_s0(tnfa_s0),
    .tnfa_s1(tnfa_s1)
  );


  no_th2
  _no_th2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[56]),
    .il4_s0(il4_s0),
    .il4_s1(il4_s1),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1),
    .tgfb_s0(tgfb_s0),
    .tgfb_s1(tgfb_s1),
    .s0(s0[56]),
    .s1(s1[56]),
    .th2_s0(th2_s0),
    .th2_s1(th2_s1)
  );


  no_th1
  _no_th1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[57]),
    .il12_s0(il12_s0),
    .il12_s1(il12_s1),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1),
    .tgfb_s0(tgfb_s0),
    .tgfb_s1(tgfb_s1),
    .il4_s0(il4_s0),
    .il4_s1(il4_s1),
    .s0(s0[57]),
    .s1(s1[57]),
    .th1_s0(th1_s0),
    .th1_s1(th1_s1)
  );


  no_tgfb
  _no_tgfb
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[58]),
    .treg_s0(treg_s0),
    .treg_s1(treg_s1),
    .s0(s0[58]),
    .s1(s1[58]),
    .tgfb_s0(tgfb_s0),
    .tgfb_s1(tgfb_s1)
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
    .init_state(init_state[59]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1),
    .ccl2_s0(ccl2_s0),
    .ccl2_s1(ccl2_s1),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1),
    .s0(s0[59]),
    .s1(s1[59]),
    .mac_s0(mac_s0),
    .mac_s1(mac_s1)
  );


  no_il6
  _no_il6
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[60]),
    .mac_s0(mac_s0),
    .mac_s1(mac_s1),
    .dc_s0(dc_s0),
    .dc_s1(dc_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .s0(s0[60]),
    .s1(s1[60]),
    .il6_s0(il6_s0),
    .il6_s1(il6_s1)
  );


  no_il4
  _no_il4
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[61]),
    .dc_s0(dc_s0),
    .dc_s1(dc_s1),
    .th2_s0(th2_s0),
    .th2_s1(th2_s1),
    .s0(s0[61]),
    .s1(s1[61]),
    .il4_s0(il4_s0),
    .il4_s1(il4_s1)
  );


  no_il12
  _no_il12
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[62]),
    .dc_s0(dc_s0),
    .dc_s1(dc_s1),
    .mac_s0(mac_s0),
    .mac_s1(mac_s1),
    .s0(s0[62]),
    .s1(s1[62]),
    .il12_s0(il12_s0),
    .il12_s1(il12_s1)
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
    .init_state(init_state[63]),
    .treg_s0(treg_s0),
    .treg_s1(treg_s1),
    .th2_s0(th2_s0),
    .th2_s1(th2_s1),
    .s0(s0[63]),
    .s1(s1[63]),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1)
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
    .init_state(init_state[64]),
    .th1_s0(th1_s0),
    .th1_s1(th1_s1),
    .ctl_s0(ctl_s0),
    .ctl_s1(ctl_s1),
    .s0(s0[64]),
    .s1(s1[64]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1)
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
    .init_state(init_state[65]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1),
    .tgfb_s0(tgfb_s0),
    .tgfb_s1(tgfb_s1),
    .s0(s0[65]),
    .s1(s1[65]),
    .ctl_s0(ctl_s0),
    .ctl_s1(ctl_s1)
  );


  no_dc
  _no_dc
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[66]),
    .ccl2_s0(ccl2_s0),
    .ccl2_s1(ccl2_s1),
    .tnfa_s0(tnfa_s0),
    .tnfa_s1(tnfa_s1),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1),
    .s0(s0[66]),
    .s1(s1[66]),
    .dc_s0(dc_s0),
    .dc_s1(dc_s1)
  );


  no_ccl2
  _no_ccl2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[67]),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .s0(s0[67]),
    .s1(s1[67]),
    .ccl2_s0(ccl2_s0),
    .ccl2_s1(ccl2_s1)
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
    .init_state(init_state[68]),
    .fos_s0(fos_s0),
    .fos_s1(fos_s1),
    .cyclind1_s0(cyclind1_s0),
    .cyclind1_s1(cyclind1_s1),
    .p21_s0(p21_s0),
    .p21_s1(p21_s1),
    .casp3_s0(casp3_s0),
    .casp3_s1(casp3_s1),
    .s0(s0[68]),
    .s1(s1[68]),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1)
  );


  no_apoptosis
  _no_apoptosis
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[69]),
    .casp3_s0(casp3_s0),
    .casp3_s1(casp3_s1),
    .s0(s0[69]),
    .s1(s1[69]),
    .apoptosis_s0(apoptosis_s0),
    .apoptosis_s1(apoptosis_s1)
  );


endmodule
