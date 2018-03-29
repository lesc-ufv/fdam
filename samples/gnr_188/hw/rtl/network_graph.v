
module network_graph
(
  input clk,
  input rst,
  input start,
  input reset_nos,
  input start_s0,
  input start_s1,
  input [188-1:0] init_state,
  output [188-1:0] s0,
  output [188-1:0] s1
);

  wire nos2a_s0;
  wire nos2a_s1;
  wire rap1_s0;
  wire rap1_s1;
  wire tyk2_s0;
  wire tyk2_s1;
  wire gab2_s0;
  wire gab2_s1;
  wire dag_s0;
  wire dag_s1;
  wire rhogef_s0;
  wire rhogef_s1;
  wire riam_s0;
  wire riam_s1;
  wire plcg_s0;
  wire plcg_s1;
  wire rock_s0;
  wire rock_s1;
  wire shp2_s0;
  wire shp2_s1;
  wire cd4_s0;
  wire cd4_s1;
  wire gfi1_s0;
  wire gfi1_s1;
  wire galphas_r_s0;
  wire galphas_r_s1;
  wire gads_s0;
  wire gads_s1;
  wire g_actin_s0;
  wire g_actin_s1;
  wire atf2_s0;
  wire atf2_s1;
  wire rhoa_s0;
  wire rhoa_s1;
  wire epac_s0;
  wire epac_s1;
  wire itk_s0;
  wire itk_s1;
  wire galpha_q_s0;
  wire galpha_q_s1;
  wire bintegrin_s0;
  wire bintegrin_s1;
  wire fak_tyr397_s0;
  wire fak_tyr397_s1;
  wire galpha12_13r_s0;
  wire galpha12_13r_s1;
  wire fak_576_577_s0;
  wire fak_576_577_s1;
  wire stat1_s0;
  wire stat1_s1;
  wire mek1_2_s0;
  wire mek1_2_s1;
  wire vav_s0;
  wire vav_s1;
  wire il6r_s0;
  wire il6r_s1;
  wire socs3_s0;
  wire socs3_s1;
  wire f_actin_s0;
  wire f_actin_s1;
  wire nfkb_s0;
  wire nfkb_s1;
  wire gsk3b_s0;
  wire gsk3b_s1;
  wire mekk4_s0;
  wire mekk4_s1;
  wire tgfbr_s0;
  wire tgfbr_s1;
  wire il21_s0;
  wire il21_s1;
  wire il22r_s0;
  wire il22r_s1;
  wire stat5_s0;
  wire stat5_s1;
  wire irf4_s0;
  wire irf4_s1;
  wire ikb_s0;
  wire ikb_s1;
  wire hlx_s0;
  wire hlx_s1;
  wire p38_s0;
  wire p38_s1;
  wire il9r_s0;
  wire il9r_s1;
  wire crk_s0;
  wire crk_s1;
  wire jak3_s0;
  wire jak3_s1;
  wire il22_s0;
  wire il22_s1;
  wire il4r_s0;
  wire il4r_s1;
  wire il15r_s0;
  wire il15r_s1;
  wire arp2_3_s0;
  wire arp2_3_s1;
  wire cav1_scaffold_s0;
  wire cav1_scaffold_s1;
  wire il27r_s0;
  wire il27r_s1;
  wire smad3_s0;
  wire smad3_s1;
  wire stat4_s0;
  wire stat4_s1;
  wire foxp3_s0;
  wire foxp3_s1;
  wire il2_s0;
  wire il2_s1;
  wire lat_s0;
  wire lat_s1;
  wire tak1_s0;
  wire tak1_s1;
  wire cd26_s0;
  wire cd26_s1;
  wire paxillin_s0;
  wire paxillin_s1;
  wire mek4_s0;
  wire mek4_s1;
  wire n_wasp_s0;
  wire n_wasp_s1;
  wire wave2_s0;
  wire wave2_s1;
  wire il4ra_high_s0;
  wire il4ra_high_s1;
  wire il2ra_s0;
  wire il2ra_s1;
  wire raf1_s0;
  wire raf1_s1;
  wire ap1_s0;
  wire ap1_s1;
  wire dec2_s0;
  wire dec2_s1;
  wire src_s0;
  wire src_s1;
  wire galpha_ir_s0;
  wire galpha_ir_s1;
  wire pip3_345_s0;
  wire pip3_345_s1;
  wire gata3_s0;
  wire gata3_s1;
  wire cofilin_s0;
  wire cofilin_s1;
  wire erk_s0;
  wire erk_s1;
  wire il17_s0;
  wire il17_s1;
  wire runx3_s0;
  wire runx3_s1;
  wire traf6_s0;
  wire traf6_s1;
  wire itams_s0;
  wire itams_s1;
  wire gadd45b_s0;
  wire gadd45b_s1;
  wire il4ra_s0;
  wire il4ra_s1;
  wire ikkcomplex_s0;
  wire ikkcomplex_s1;
  wire stat5_high_s0;
  wire stat5_high_s1;
  wire slp76_s0;
  wire slp76_s1;
  wire ifngr_s0;
  wire ifngr_s1;
  wire il12rb1_s0;
  wire il12rb1_s1;
  wire ras_s0;
  wire ras_s1;
  wire cd28_s0;
  wire cd28_s1;
  wire jnk_s0;
  wire jnk_s1;
  wire il23r_s0;
  wire il23r_s1;
  wire tgfb_s0;
  wire tgfb_s1;
  wire pi3k_s0;
  wire pi3k_s1;
  wire lck_s0;
  wire lck_s1;
  wire syk_s0;
  wire syk_s1;
  wire camp_s0;
  wire camp_s1;
  wire nik_s0;
  wire nik_s1;
  wire stat6_s0;
  wire stat6_s1;
  wire il2r_high_s0;
  wire il2r_high_s1;
  wire mek6_s0;
  wire mek6_s1;
  wire braf_s0;
  wire braf_s1;
  wire mlc_s0;
  wire mlc_s1;
  wire mkk7_s0;
  wire mkk7_s1;
  wire nck_s0;
  wire nck_s1;
  wire pka_s0;
  wire pka_s1;
  wire irf1_s0;
  wire irf1_s1;
  wire limk_s0;
  wire limk_s1;
  wire akt_s0;
  wire akt_s1;
  wire carma1_s0;
  wire carma1_s1;
  wire ca2_s0;
  wire ca2_s1;
  wire fyn_s0;
  wire fyn_s1;
  wire bcl10_malt1_s0;
  wire bcl10_malt1_s1;
  wire gadd45g_s0;
  wire gadd45g_s1;
  wire il18r1_s0;
  wire il18r1_s1;
  wire rac1_s0;
  wire rac1_s1;
  wire ip3_s0;
  wire ip3_s1;
  wire irsp53_s0;
  wire irsp53_s1;
  wire tcr_s0;
  wire tcr_s1;
  wire ifnbr_s0;
  wire ifnbr_s1;
  wire sos_s0;
  wire sos_s1;
  wire il4_s0;
  wire il4_s1;
  wire socs1_s0;
  wire socs1_s1;
  wire proliferation_s0;
  wire proliferation_s1;
  wire pkc_s0;
  wire pkc_s1;
  wire mek3_s0;
  wire mek3_s1;
  wire grb2_s0;
  wire grb2_s1;
  wire calcineurin_s0;
  wire calcineurin_s1;
  wire pak_s0;
  wire pak_s1;
  wire il10_s0;
  wire il10_s1;
  wire c3g_s0;
  wire c3g_s1;
  wire il12rb2_s0;
  wire il12rb2_s1;
  wire ifng_s0;
  wire ifng_s1;
  wire erm_s0;
  wire erm_s1;
  wire jak1_s0;
  wire jak1_s1;
  wire stat3_s0;
  wire stat3_s1;
  wire il2r_s0;
  wire il2r_s1;
  wire shc1_s0;
  wire shc1_s1;
  wire pdk1_s0;
  wire pdk1_s1;
  wire cd3_s0;
  wire cd3_s1;
  wire il21r_s0;
  wire il21r_s1;
  wire jak2_s0;
  wire jak2_s1;
  wire profilin_s0;
  wire profilin_s1;
  wire nfat_s0;
  wire nfat_s1;
  wire icos_s0;
  wire icos_s1;
  wire irak1_s0;
  wire irak1_s1;
  wire adenyl_cyclase_s0;
  wire adenyl_cyclase_s1;
  wire was_s0;
  wire was_s1;
  wire rasgrp_s0;
  wire rasgrp_s1;
  wire il10r_s0;
  wire il10r_s1;
  wire il4r_high_s0;
  wire il4r_high_s1;
  wire cas_s0;
  wire cas_s1;
  wire rorgt_s0;
  wire rorgt_s1;
  wire plcb_s0;
  wire plcb_s1;
  wire zap70_s0;
  wire zap70_s1;
  wire bcl10_carma1_malti_s0;
  wire bcl10_carma1_malti_s1;
  wire cdc42_s0;
  wire cdc42_s1;
  wire il23_s0;
  wire il23_s1;
  wire tbet_s0;
  wire tbet_s1;
  wire alpha_13l_s0;
  wire alpha_13l_s1;
  wire ifnb_e_s0;
  wire ifnb_e_s1;
  wire mhc_ii_s0;
  wire mhc_ii_s1;
  wire il12_e_s0;
  wire il12_e_s1;
  wire il23_e_s0;
  wire il23_e_s1;
  wire il15_e_s0;
  wire il15_e_s1;
  wire ifngr2_s0;
  wire ifngr2_s1;
  wire il21_e_s0;
  wire il21_e_s1;
  wire il6_e_s0;
  wire il6_e_s1;
  wire il2rb_s0;
  wire il2rb_s1;
  wire il4_e_s0;
  wire il4_e_s1;
  wire il10_e_s0;
  wire il10_e_s1;
  wire tgfb_e_s0;
  wire tgfb_e_s1;
  wire il15ra_s0;
  wire il15ra_s1;
  wire ifngr1_s0;
  wire ifngr1_s1;
  wire il2_e_s0;
  wire il2_e_s1;
  wire galpha_il_s0;
  wire galpha_il_s1;
  wire gp130_s0;
  wire gp130_s1;
  wire galphas_l_s0;
  wire galphas_l_s1;
  wire il10rb_s0;
  wire il10rb_s1;
  wire il27ra_s0;
  wire il27ra_s1;
  wire il27_e_s0;
  wire il27_e_s1;
  wire il6ra_s0;
  wire il6ra_s1;
  wire cgc_s0;
  wire cgc_s1;
  wire galpha_ql_s0;
  wire galpha_ql_s1;
  wire cav1_activator_s0;
  wire cav1_activator_s1;
  wire apc_s0;
  wire apc_s1;
  wire ifng_e_s0;
  wire ifng_e_s1;
  wire b7_s0;
  wire b7_s1;
  wire il18_e_s0;
  wire il18_e_s1;
  wire il9_e_s0;
  wire il9_e_s1;
  wire ecm_s0;
  wire ecm_s1;
  wire il22_e_s0;
  wire il22_e_s1;
  wire il10ra_s0;
  wire il10ra_s1;

  no_nos2a
  _no_nos2a
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[0]),
    .cav1_scaffold_s0(cav1_scaffold_s0),
    .cav1_scaffold_s1(cav1_scaffold_s1),
    .s0(s0[0]),
    .s1(s1[0]),
    .nos2a_s0(nos2a_s0),
    .nos2a_s1(nos2a_s1)
  );


  no_rap1
  _no_rap1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[1]),
    .epac_s0(epac_s0),
    .epac_s1(epac_s1),
    .c3g_s0(c3g_s0),
    .c3g_s1(c3g_s1),
    .crk_s0(crk_s0),
    .crk_s1(crk_s1),
    .pka_s0(pka_s0),
    .pka_s1(pka_s1),
    .s0(s0[1]),
    .s1(s1[1]),
    .rap1_s0(rap1_s0),
    .rap1_s1(rap1_s1)
  );


  no_tyk2
  _no_tyk2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[2]),
    .il12rb1_s0(il12rb1_s0),
    .il12rb1_s1(il12rb1_s1),
    .il12rb2_s0(il12rb2_s0),
    .il12rb2_s1(il12rb2_s1),
    .s0(s0[2]),
    .s1(s1[2]),
    .tyk2_s0(tyk2_s0),
    .tyk2_s1(tyk2_s1)
  );


  no_gab2
  _no_gab2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[3]),
    .shc1_s0(shc1_s0),
    .shc1_s1(shc1_s1),
    .grb2_s0(grb2_s0),
    .grb2_s1(grb2_s1),
    .s0(s0[3]),
    .s1(s1[3]),
    .gab2_s0(gab2_s0),
    .gab2_s1(gab2_s1)
  );


  no_dag
  _no_dag
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[4]),
    .plcb_s0(plcb_s0),
    .plcb_s1(plcb_s1),
    .plcg_s0(plcg_s0),
    .plcg_s1(plcg_s1),
    .s0(s0[4]),
    .s1(s1[4]),
    .dag_s0(dag_s0),
    .dag_s1(dag_s1)
  );


  no_rhogef
  _no_rhogef
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[5]),
    .galpha12_13r_s0(galpha12_13r_s0),
    .galpha12_13r_s1(galpha12_13r_s1),
    .fak_576_577_s0(fak_576_577_s0),
    .fak_576_577_s1(fak_576_577_s1),
    .s0(s0[5]),
    .s1(s1[5]),
    .rhogef_s0(rhogef_s0),
    .rhogef_s1(rhogef_s1)
  );


  no_riam
  _no_riam
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[6]),
    .rap1_s0(rap1_s0),
    .rap1_s1(rap1_s1),
    .s0(s0[6]),
    .s1(s1[6]),
    .riam_s0(riam_s0),
    .riam_s1(riam_s1)
  );


  no_plcg
  _no_plcg
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[7]),
    .zap70_s0(zap70_s0),
    .zap70_s1(zap70_s1),
    .itk_s0(itk_s0),
    .itk_s1(itk_s1),
    .lat_s0(lat_s0),
    .lat_s1(lat_s1),
    .s0(s0[7]),
    .s1(s1[7]),
    .plcg_s0(plcg_s0),
    .plcg_s1(plcg_s1)
  );


  no_rock
  _no_rock
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[8]),
    .rhoa_s0(rhoa_s0),
    .rhoa_s1(rhoa_s1),
    .s0(s0[8]),
    .s1(s1[8]),
    .rock_s0(rock_s0),
    .rock_s1(rock_s1)
  );


  no_shp2
  _no_shp2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[9]),
    .gab2_s0(gab2_s0),
    .gab2_s1(gab2_s1),
    .il2rb_s0(il2rb_s0),
    .il2rb_s1(il2rb_s1),
    .s0(s0[9]),
    .s1(s1[9]),
    .shp2_s0(shp2_s0),
    .shp2_s1(shp2_s1)
  );


  no_cd4
  _no_cd4
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[10]),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1),
    .mhc_ii_s0(mhc_ii_s0),
    .mhc_ii_s1(mhc_ii_s1),
    .cd3_s0(cd3_s0),
    .cd3_s1(cd3_s1),
    .s0(s0[10]),
    .s1(s1[10]),
    .cd4_s0(cd4_s0),
    .cd4_s1(cd4_s1)
  );


  no_gfi1
  _no_gfi1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[11]),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1),
    .stat6_s0(stat6_s0),
    .stat6_s1(stat6_s1),
    .s0(s0[11]),
    .s1(s1[11]),
    .gfi1_s0(gfi1_s0),
    .gfi1_s1(gfi1_s1)
  );


  no_galphas_r
  _no_galphas_r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[12]),
    .galphas_l_s0(galphas_l_s0),
    .galphas_l_s1(galphas_l_s1),
    .s0(s0[12]),
    .s1(s1[12]),
    .galphas_r_s0(galphas_r_s0),
    .galphas_r_s1(galphas_r_s1)
  );


  no_gads
  _no_gads
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[13]),
    .lat_s0(lat_s0),
    .lat_s1(lat_s1),
    .s0(s0[13]),
    .s1(s1[13]),
    .gads_s0(gads_s0),
    .gads_s1(gads_s1)
  );


  no_g_actin
  _no_g_actin
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[14]),
    .profilin_s0(profilin_s0),
    .profilin_s1(profilin_s1),
    .s0(s0[14]),
    .s1(s1[14]),
    .g_actin_s0(g_actin_s0),
    .g_actin_s1(g_actin_s1)
  );


  no_atf2
  _no_atf2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[15]),
    .p38_s0(p38_s0),
    .p38_s1(p38_s1),
    .s0(s0[15]),
    .s1(s1[15]),
    .atf2_s0(atf2_s0),
    .atf2_s1(atf2_s1)
  );


  no_rhoa
  _no_rhoa
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[16]),
    .rhogef_s0(rhogef_s0),
    .rhogef_s1(rhogef_s1),
    .cav1_scaffold_s0(cav1_scaffold_s0),
    .cav1_scaffold_s1(cav1_scaffold_s1),
    .s0(s0[16]),
    .s1(s1[16]),
    .rhoa_s0(rhoa_s0),
    .rhoa_s1(rhoa_s1)
  );


  no_epac
  _no_epac
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[17]),
    .camp_s0(camp_s0),
    .camp_s1(camp_s1),
    .s0(s0[17]),
    .s1(s1[17]),
    .epac_s0(epac_s0),
    .epac_s1(epac_s1)
  );


  no_itk
  _no_itk
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[18]),
    .slp76_s0(slp76_s0),
    .slp76_s1(slp76_s1),
    .s0(s0[18]),
    .s1(s1[18]),
    .itk_s0(itk_s0),
    .itk_s1(itk_s1)
  );


  no_galpha_q
  _no_galpha_q
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[19]),
    .galpha_ql_s0(galpha_ql_s0),
    .galpha_ql_s1(galpha_ql_s1),
    .s0(s0[19]),
    .s1(s1[19]),
    .galpha_q_s0(galpha_q_s0),
    .galpha_q_s1(galpha_q_s1)
  );


  no_bintegrin
  _no_bintegrin
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[20]),
    .ecm_s0(ecm_s0),
    .ecm_s1(ecm_s1),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1),
    .s0(s0[20]),
    .s1(s1[20]),
    .bintegrin_s0(bintegrin_s0),
    .bintegrin_s1(bintegrin_s1)
  );


  no_fak_tyr397
  _no_fak_tyr397
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[21]),
    .bintegrin_s0(bintegrin_s0),
    .bintegrin_s1(bintegrin_s1),
    .s0(s0[21]),
    .s1(s1[21]),
    .fak_tyr397_s0(fak_tyr397_s0),
    .fak_tyr397_s1(fak_tyr397_s1)
  );


  no_galpha12_13r
  _no_galpha12_13r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[22]),
    .alpha_13l_s0(alpha_13l_s0),
    .alpha_13l_s1(alpha_13l_s1),
    .s0(s0[22]),
    .s1(s1[22]),
    .galpha12_13r_s0(galpha12_13r_s0),
    .galpha12_13r_s1(galpha12_13r_s1)
  );


  no_fak_576_577
  _no_fak_576_577
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[23]),
    .fak_tyr397_s0(fak_tyr397_s0),
    .fak_tyr397_s1(fak_tyr397_s1),
    .src_s0(src_s0),
    .src_s1(src_s1),
    .s0(s0[23]),
    .s1(s1[23]),
    .fak_576_577_s0(fak_576_577_s0),
    .fak_576_577_s1(fak_576_577_s1)
  );


  no_stat1
  _no_stat1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[24]),
    .ifnbr_s0(ifnbr_s0),
    .ifnbr_s1(ifnbr_s1),
    .socs1_s0(socs1_s0),
    .socs1_s1(socs1_s1),
    .ifngr_s0(ifngr_s0),
    .ifngr_s1(ifngr_s1),
    .il27r_s0(il27r_s0),
    .il27r_s1(il27r_s1),
    .s0(s0[24]),
    .s1(s1[24]),
    .stat1_s0(stat1_s0),
    .stat1_s1(stat1_s1)
  );


  no_mek1_2
  _no_mek1_2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[25]),
    .raf1_s0(raf1_s0),
    .raf1_s1(raf1_s1),
    .braf_s0(braf_s0),
    .braf_s1(braf_s1),
    .pak_s0(pak_s0),
    .pak_s1(pak_s1),
    .s0(s0[25]),
    .s1(s1[25]),
    .mek1_2_s0(mek1_2_s0),
    .mek1_2_s1(mek1_2_s1)
  );


  no_vav
  _no_vav
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[26]),
    .slp76_s0(slp76_s0),
    .slp76_s1(slp76_s1),
    .s0(s0[26]),
    .s1(s1[26]),
    .vav_s0(vav_s0),
    .vav_s1(vav_s1)
  );


  no_il6r
  _no_il6r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[27]),
    .gp130_s0(gp130_s0),
    .gp130_s1(gp130_s1),
    .il6_e_s0(il6_e_s0),
    .il6_e_s1(il6_e_s1),
    .il6ra_s0(il6ra_s0),
    .il6ra_s1(il6ra_s1),
    .s0(s0[27]),
    .s1(s1[27]),
    .il6r_s0(il6r_s0),
    .il6r_s1(il6r_s1)
  );


  no_socs3
  _no_socs3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[28]),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .s0(s0[28]),
    .s1(s1[28]),
    .socs3_s0(socs3_s0),
    .socs3_s1(socs3_s1)
  );


  no_f_actin
  _no_f_actin
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[29]),
    .arp2_3_s0(arp2_3_s0),
    .arp2_3_s1(arp2_3_s1),
    .g_actin_s0(g_actin_s0),
    .g_actin_s1(g_actin_s1),
    .s0(s0[29]),
    .s1(s1[29]),
    .f_actin_s0(f_actin_s0),
    .f_actin_s1(f_actin_s1)
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
    .init_state(init_state[30]),
    .ikb_s0(ikb_s0),
    .ikb_s1(ikb_s1),
    .foxp3_s0(foxp3_s0),
    .foxp3_s1(foxp3_s1),
    .s0(s0[30]),
    .s1(s1[30]),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1)
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
    .init_state(init_state[31]),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1),
    .s0(s0[31]),
    .s1(s1[31]),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1)
  );


  no_mekk4
  _no_mekk4
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[32]),
    .gadd45b_s0(gadd45b_s0),
    .gadd45b_s1(gadd45b_s1),
    .gadd45g_s0(gadd45g_s0),
    .gadd45g_s1(gadd45g_s1),
    .s0(s0[32]),
    .s1(s1[32]),
    .mekk4_s0(mekk4_s0),
    .mekk4_s1(mekk4_s1)
  );


  no_tgfbr
  _no_tgfbr
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[33]),
    .tgfb_s0(tgfb_s0),
    .tgfb_s1(tgfb_s1),
    .tgfb_e_s0(tgfb_e_s0),
    .tgfb_e_s1(tgfb_e_s1),
    .s0(s0[33]),
    .s1(s1[33]),
    .tgfbr_s0(tgfbr_s0),
    .tgfbr_s1(tgfbr_s1)
  );


  no_il21
  _no_il21
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[34]),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1),
    .s0(s0[34]),
    .s1(s1[34]),
    .il21_s0(il21_s0),
    .il21_s1(il21_s1)
  );


  no_il22r
  _no_il22r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[35]),
    .il22_e_s0(il22_e_s0),
    .il22_e_s1(il22_e_s1),
    .s0(s0[35]),
    .s1(s1[35]),
    .il22r_s0(il22r_s0),
    .il22r_s1(il22r_s1)
  );


  no_stat5
  _no_stat5
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[36]),
    .il2r_s0(il2r_s0),
    .il2r_s1(il2r_s1),
    .il4r_s0(il4r_s0),
    .il4r_s1(il4r_s1),
    .il15r_s0(il15r_s0),
    .il15r_s1(il15r_s1),
    .syk_s0(syk_s0),
    .syk_s1(syk_s1),
    .lck_s0(lck_s0),
    .lck_s1(lck_s1),
    .jak1_s0(jak1_s0),
    .jak1_s1(jak1_s1),
    .s0(s0[36]),
    .s1(s1[36]),
    .stat5_s0(stat5_s0),
    .stat5_s1(stat5_s1)
  );


  no_irf4
  _no_irf4
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[37]),
    .gata3_s0(gata3_s0),
    .gata3_s1(gata3_s1),
    .s0(s0[37]),
    .s1(s1[37]),
    .irf4_s0(irf4_s0),
    .irf4_s1(irf4_s1)
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
    .init_state(init_state[38]),
    .ikkcomplex_s0(ikkcomplex_s0),
    .ikkcomplex_s1(ikkcomplex_s1),
    .s0(s0[38]),
    .s1(s1[38]),
    .ikb_s0(ikb_s0),
    .ikb_s1(ikb_s1)
  );


  no_hlx
  _no_hlx
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[39]),
    .tbet_s0(tbet_s0),
    .tbet_s1(tbet_s1),
    .s0(s0[39]),
    .s1(s1[39]),
    .hlx_s0(hlx_s0),
    .hlx_s1(hlx_s1)
  );


  no_p38
  _no_p38
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[40]),
    .mek3_s0(mek3_s0),
    .mek3_s1(mek3_s1),
    .mek6_s0(mek6_s0),
    .mek6_s1(mek6_s1),
    .s0(s0[40]),
    .s1(s1[40]),
    .p38_s0(p38_s0),
    .p38_s1(p38_s1)
  );


  no_il9r
  _no_il9r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[41]),
    .il9_e_s0(il9_e_s0),
    .il9_e_s1(il9_e_s1),
    .jak3_s0(jak3_s0),
    .jak3_s1(jak3_s1),
    .s0(s0[41]),
    .s1(s1[41]),
    .il9r_s0(il9r_s0),
    .il9r_s1(il9r_s1)
  );


  no_crk
  _no_crk
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[42]),
    .cas_s0(cas_s0),
    .cas_s1(cas_s1),
    .paxillin_s0(paxillin_s0),
    .paxillin_s1(paxillin_s1),
    .s0(s0[42]),
    .s1(s1[42]),
    .crk_s0(crk_s0),
    .crk_s1(crk_s1)
  );


  no_jak3
  _no_jak3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[43]),
    .il2r_s0(il2r_s0),
    .il2r_s1(il2r_s1),
    .s0(s0[43]),
    .s1(s1[43]),
    .jak3_s0(jak3_s0),
    .jak3_s1(jak3_s1)
  );


  no_il22
  _no_il22
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[44]),
    .stat4_s0(stat4_s0),
    .stat4_s1(stat4_s1),
    .stat1_s0(stat1_s0),
    .stat1_s1(stat1_s1),
    .stat5_s0(stat5_s0),
    .stat5_s1(stat5_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .s0(s0[44]),
    .s1(s1[44]),
    .il22_s0(il22_s0),
    .il22_s1(il22_s1)
  );


  no_il4r
  _no_il4r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[45]),
    .il4_s0(il4_s0),
    .il4_s1(il4_s1),
    .il4ra_s0(il4ra_s0),
    .il4ra_s1(il4ra_s1),
    .cgc_s0(cgc_s0),
    .cgc_s1(cgc_s1),
    .il4_e_s0(il4_e_s0),
    .il4_e_s1(il4_e_s1),
    .s0(s0[45]),
    .s1(s1[45]),
    .il4r_s0(il4r_s0),
    .il4r_s1(il4r_s1)
  );


  no_il15r
  _no_il15r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[46]),
    .cgc_s0(cgc_s0),
    .cgc_s1(cgc_s1),
    .il15ra_s0(il15ra_s0),
    .il15ra_s1(il15ra_s1),
    .il2rb_s0(il2rb_s0),
    .il2rb_s1(il2rb_s1),
    .il15_e_s0(il15_e_s0),
    .il15_e_s1(il15_e_s1),
    .s0(s0[46]),
    .s1(s1[46]),
    .il15r_s0(il15r_s0),
    .il15r_s1(il15r_s1)
  );


  no_arp2_3
  _no_arp2_3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[47]),
    .n_wasp_s0(n_wasp_s0),
    .n_wasp_s1(n_wasp_s1),
    .wave2_s0(wave2_s0),
    .wave2_s1(wave2_s1),
    .s0(s0[47]),
    .s1(s1[47]),
    .arp2_3_s0(arp2_3_s0),
    .arp2_3_s1(arp2_3_s1)
  );


  no_cav1_scaffold
  _no_cav1_scaffold
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[48]),
    .src_s0(src_s0),
    .src_s1(src_s1),
    .cav1_activator_s0(cav1_activator_s0),
    .cav1_activator_s1(cav1_activator_s1),
    .bintegrin_s0(bintegrin_s0),
    .bintegrin_s1(bintegrin_s1),
    .s0(s0[48]),
    .s1(s1[48]),
    .cav1_scaffold_s0(cav1_scaffold_s0),
    .cav1_scaffold_s1(cav1_scaffold_s1)
  );


  no_il27r
  _no_il27r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[49]),
    .gp130_s0(gp130_s0),
    .gp130_s1(gp130_s1),
    .il27ra_s0(il27ra_s0),
    .il27ra_s1(il27ra_s1),
    .il27_e_s0(il27_e_s0),
    .il27_e_s1(il27_e_s1),
    .s0(s0[49]),
    .s1(s1[49]),
    .il27r_s0(il27r_s0),
    .il27r_s1(il27r_s1)
  );


  no_smad3
  _no_smad3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[50]),
    .tgfbr_s0(tgfbr_s0),
    .tgfbr_s1(tgfbr_s1),
    .s0(s0[50]),
    .s1(s1[50]),
    .smad3_s0(smad3_s0),
    .smad3_s1(smad3_s1)
  );


  no_stat4
  _no_stat4
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[51]),
    .p38_s0(p38_s0),
    .p38_s1(p38_s1),
    .tyk2_s0(tyk2_s0),
    .tyk2_s1(tyk2_s1),
    .gata3_s0(gata3_s0),
    .gata3_s1(gata3_s1),
    .jak2_s0(jak2_s0),
    .jak2_s1(jak2_s1),
    .s0(s0[51]),
    .s1(s1[51]),
    .stat4_s0(stat4_s0),
    .stat4_s1(stat4_s1)
  );


  no_foxp3
  _no_foxp3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[52]),
    .smad3_s0(smad3_s0),
    .smad3_s1(smad3_s1),
    .stat5_s0(stat5_s0),
    .stat5_s1(stat5_s1),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .rorgt_s0(rorgt_s0),
    .rorgt_s1(rorgt_s1),
    .stat1_s0(stat1_s0),
    .stat1_s1(stat1_s1),
    .s0(s0[52]),
    .s1(s1[52]),
    .foxp3_s0(foxp3_s0),
    .foxp3_s1(foxp3_s1)
  );


  no_il2
  _no_il2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[53]),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .foxp3_s0(foxp3_s0),
    .foxp3_s1(foxp3_s1),
    .tbet_s0(tbet_s0),
    .tbet_s1(tbet_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .stat5_s0(stat5_s0),
    .stat5_s1(stat5_s1),
    .stat6_s0(stat6_s0),
    .stat6_s1(stat6_s1),
    .s0(s0[53]),
    .s1(s1[53]),
    .il2_s0(il2_s0),
    .il2_s1(il2_s1)
  );


  no_lat
  _no_lat
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[54]),
    .zap70_s0(zap70_s0),
    .zap70_s1(zap70_s1),
    .s0(s0[54]),
    .s1(s1[54]),
    .lat_s0(lat_s0),
    .lat_s1(lat_s1)
  );


  no_tak1
  _no_tak1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[55]),
    .traf6_s0(traf6_s0),
    .traf6_s1(traf6_s1),
    .s0(s0[55]),
    .s1(s1[55]),
    .tak1_s0(tak1_s0),
    .tak1_s1(tak1_s1)
  );


  no_cd26
  _no_cd26
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[56]),
    .cav1_scaffold_s0(cav1_scaffold_s0),
    .cav1_scaffold_s1(cav1_scaffold_s1),
    .s0(s0[56]),
    .s1(s1[56]),
    .cd26_s0(cd26_s0),
    .cd26_s1(cd26_s1)
  );


  no_paxillin
  _no_paxillin
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[57]),
    .fak_576_577_s0(fak_576_577_s0),
    .fak_576_577_s1(fak_576_577_s1),
    .s0(s0[57]),
    .s1(s1[57]),
    .paxillin_s0(paxillin_s0),
    .paxillin_s1(paxillin_s1)
  );


  no_mek4
  _no_mek4
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[58]),
    .mekk4_s0(mekk4_s0),
    .mekk4_s1(mekk4_s1),
    .s0(s0[58]),
    .s1(s1[58]),
    .mek4_s0(mek4_s0),
    .mek4_s1(mek4_s1)
  );


  no_n_wasp
  _no_n_wasp
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[59]),
    .cdc42_s0(cdc42_s0),
    .cdc42_s1(cdc42_s1),
    .nck_s0(nck_s0),
    .nck_s1(nck_s1),
    .vav_s0(vav_s0),
    .vav_s1(vav_s1),
    .s0(s0[59]),
    .s1(s1[59]),
    .n_wasp_s0(n_wasp_s0),
    .n_wasp_s1(n_wasp_s1)
  );


  no_wave2
  _no_wave2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[60]),
    .irsp53_s0(irsp53_s0),
    .irsp53_s1(irsp53_s1),
    .rac1_s0(rac1_s0),
    .rac1_s1(rac1_s1),
    .s0(s0[60]),
    .s1(s1[60]),
    .wave2_s0(wave2_s0),
    .wave2_s1(wave2_s1)
  );


  no_il4ra_high
  _no_il4ra_high
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[61]),
    .stat5_high_s0(stat5_high_s0),
    .stat5_high_s1(stat5_high_s1),
    .s0(s0[61]),
    .s1(s1[61]),
    .il4ra_high_s0(il4ra_high_s0),
    .il4ra_high_s1(il4ra_high_s1)
  );


  no_il2ra
  _no_il2ra
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[62]),
    .foxp3_s0(foxp3_s0),
    .foxp3_s1(foxp3_s1),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .stat5_s0(stat5_s0),
    .stat5_s1(stat5_s1),
    .smad3_s0(smad3_s0),
    .smad3_s1(smad3_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .s0(s0[62]),
    .s1(s1[62]),
    .il2ra_s0(il2ra_s0),
    .il2ra_s1(il2ra_s1)
  );


  no_raf1
  _no_raf1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[63]),
    .ras_s0(ras_s0),
    .ras_s1(ras_s1),
    .s0(s0[63]),
    .s1(s1[63]),
    .raf1_s0(raf1_s0),
    .raf1_s1(raf1_s1)
  );


  no_ap1
  _no_ap1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[64]),
    .stat4_s0(stat4_s0),
    .stat4_s1(stat4_s1),
    .jnk_s0(jnk_s0),
    .jnk_s1(jnk_s1),
    .erk_s0(erk_s0),
    .erk_s1(erk_s1),
    .s0(s0[64]),
    .s1(s1[64]),
    .ap1_s0(ap1_s0),
    .ap1_s1(ap1_s1)
  );


  no_dec2
  _no_dec2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[65]),
    .gata3_s0(gata3_s0),
    .gata3_s1(gata3_s1),
    .s0(s0[65]),
    .s1(s1[65]),
    .dec2_s0(dec2_s0),
    .dec2_s1(dec2_s1)
  );


  no_src
  _no_src
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[66]),
    .bintegrin_s0(bintegrin_s0),
    .bintegrin_s1(bintegrin_s1),
    .fak_tyr397_s0(fak_tyr397_s0),
    .fak_tyr397_s1(fak_tyr397_s1),
    .s0(s0[66]),
    .s1(s1[66]),
    .src_s0(src_s0),
    .src_s1(src_s1)
  );


  no_galpha_ir
  _no_galpha_ir
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[67]),
    .galpha_il_s0(galpha_il_s0),
    .galpha_il_s1(galpha_il_s1),
    .s0(s0[67]),
    .s1(s1[67]),
    .galpha_ir_s0(galpha_ir_s0),
    .galpha_ir_s1(galpha_ir_s1)
  );


  no_pip3_345
  _no_pip3_345
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[68]),
    .pi3k_s0(pi3k_s0),
    .pi3k_s1(pi3k_s1),
    .s0(s0[68]),
    .s1(s1[68]),
    .pip3_345_s0(pip3_345_s0),
    .pip3_345_s1(pip3_345_s1)
  );


  no_gata3
  _no_gata3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[69]),
    .dec2_s0(dec2_s0),
    .dec2_s1(dec2_s1),
    .stat6_s0(stat6_s0),
    .stat6_s1(stat6_s1),
    .tbet_s0(tbet_s0),
    .tbet_s1(tbet_s1),
    .s0(s0[69]),
    .s1(s1[69]),
    .gata3_s0(gata3_s0),
    .gata3_s1(gata3_s1)
  );


  no_cofilin
  _no_cofilin
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[70]),
    .limk_s0(limk_s0),
    .limk_s1(limk_s1),
    .s0(s0[70]),
    .s1(s1[70]),
    .cofilin_s0(cofilin_s0),
    .cofilin_s1(cofilin_s1)
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
    .init_state(init_state[71]),
    .mek1_2_s0(mek1_2_s0),
    .mek1_2_s1(mek1_2_s1),
    .s0(s0[71]),
    .s1(s1[71]),
    .erk_s0(erk_s0),
    .erk_s1(erk_s1)
  );


  no_il17
  _no_il17
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[72]),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .nfkb_s0(nfkb_s0),
    .nfkb_s1(nfkb_s1),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1),
    .rorgt_s0(rorgt_s0),
    .rorgt_s1(rorgt_s1),
    .stat1_s0(stat1_s0),
    .stat1_s1(stat1_s1),
    .foxp3_s0(foxp3_s0),
    .foxp3_s1(foxp3_s1),
    .stat6_s0(stat6_s0),
    .stat6_s1(stat6_s1),
    .stat5_s0(stat5_s0),
    .stat5_s1(stat5_s1),
    .s0(s0[72]),
    .s1(s1[72]),
    .il17_s0(il17_s0),
    .il17_s1(il17_s1)
  );


  no_runx3
  _no_runx3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[73]),
    .tbet_s0(tbet_s0),
    .tbet_s1(tbet_s1),
    .gata3_s0(gata3_s0),
    .gata3_s1(gata3_s1),
    .s0(s0[73]),
    .s1(s1[73]),
    .runx3_s0(runx3_s0),
    .runx3_s1(runx3_s1)
  );


  no_traf6
  _no_traf6
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[74]),
    .irak1_s0(irak1_s0),
    .irak1_s1(irak1_s1),
    .s0(s0[74]),
    .s1(s1[74]),
    .traf6_s0(traf6_s0),
    .traf6_s1(traf6_s1)
  );


  no_itams
  _no_itams
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[75]),
    .lck_s0(lck_s0),
    .lck_s1(lck_s1),
    .s0(s0[75]),
    .s1(s1[75]),
    .itams_s0(itams_s0),
    .itams_s1(itams_s1)
  );


  no_gadd45b
  _no_gadd45b
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[76]),
    .il12_e_s0(il12_e_s0),
    .il12_e_s1(il12_e_s1),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1),
    .s0(s0[76]),
    .s1(s1[76]),
    .gadd45b_s0(gadd45b_s0),
    .gadd45b_s1(gadd45b_s1)
  );


  no_il4ra
  _no_il4ra
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[77]),
    .stat5_high_s0(stat5_high_s0),
    .stat5_high_s1(stat5_high_s1),
    .s0(s0[77]),
    .s1(s1[77]),
    .il4ra_s0(il4ra_s0),
    .il4ra_s1(il4ra_s1)
  );


  no_ikkcomplex
  _no_ikkcomplex
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[78]),
    .bcl10_carma1_malti_s0(bcl10_carma1_malti_s0),
    .bcl10_carma1_malti_s1(bcl10_carma1_malti_s1),
    .nik_s0(nik_s0),
    .nik_s1(nik_s1),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1),
    .s0(s0[78]),
    .s1(s1[78]),
    .ikkcomplex_s0(ikkcomplex_s0),
    .ikkcomplex_s1(ikkcomplex_s1)
  );


  no_stat5_high
  _no_stat5_high
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[79]),
    .il4r_high_s0(il4r_high_s0),
    .il4r_high_s1(il4r_high_s1),
    .il2r_high_s0(il2r_high_s0),
    .il2r_high_s1(il2r_high_s1),
    .s0(s0[79]),
    .s1(s1[79]),
    .stat5_high_s0(stat5_high_s0),
    .stat5_high_s1(stat5_high_s1)
  );


  no_slp76
  _no_slp76
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[80]),
    .zap70_s0(zap70_s0),
    .zap70_s1(zap70_s1),
    .gads_s0(gads_s0),
    .gads_s1(gads_s1),
    .s0(s0[80]),
    .s1(s1[80]),
    .slp76_s0(slp76_s0),
    .slp76_s1(slp76_s1)
  );


  no_ifngr
  _no_ifngr
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[81]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1),
    .ifngr2_s0(ifngr2_s0),
    .ifngr2_s1(ifngr2_s1),
    .ifngr1_s0(ifngr1_s0),
    .ifngr1_s1(ifngr1_s1),
    .ifng_e_s0(ifng_e_s0),
    .ifng_e_s1(ifng_e_s1),
    .s0(s0[81]),
    .s1(s1[81]),
    .ifngr_s0(ifngr_s0),
    .ifngr_s1(ifngr_s1)
  );


  no_il12rb1
  _no_il12rb1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[82]),
    .irf1_s0(irf1_s0),
    .irf1_s1(irf1_s1),
    .il12_e_s0(il12_e_s0),
    .il12_e_s1(il12_e_s1),
    .s0(s0[82]),
    .s1(s1[82]),
    .il12rb1_s0(il12rb1_s0),
    .il12rb1_s1(il12rb1_s1)
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
    .init_state(init_state[83]),
    .sos_s0(sos_s0),
    .sos_s1(sos_s1),
    .rasgrp_s0(rasgrp_s0),
    .rasgrp_s1(rasgrp_s1),
    .s0(s0[83]),
    .s1(s1[83]),
    .ras_s0(ras_s0),
    .ras_s1(ras_s1)
  );


  no_cd28
  _no_cd28
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[84]),
    .apc_s0(apc_s0),
    .apc_s1(apc_s1),
    .b7_s0(b7_s0),
    .b7_s1(b7_s1),
    .s0(s0[84]),
    .s1(s1[84]),
    .cd28_s0(cd28_s0),
    .cd28_s1(cd28_s1)
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
    .init_state(init_state[85]),
    .rac1_s0(rac1_s0),
    .rac1_s1(rac1_s1),
    .crk_s0(crk_s0),
    .crk_s1(crk_s1),
    .mek4_s0(mek4_s0),
    .mek4_s1(mek4_s1),
    .mkk7_s0(mkk7_s0),
    .mkk7_s1(mkk7_s1),
    .s0(s0[85]),
    .s1(s1[85]),
    .jnk_s0(jnk_s0),
    .jnk_s1(jnk_s1)
  );


  no_il23r
  _no_il23r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[86]),
    .il23_s0(il23_s0),
    .il23_s1(il23_s1),
    .il12rb1_s0(il12rb1_s0),
    .il12rb1_s1(il12rb1_s1),
    .gp130_s0(gp130_s0),
    .gp130_s1(gp130_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .rorgt_s0(rorgt_s0),
    .rorgt_s1(rorgt_s1),
    .il23_e_s0(il23_e_s0),
    .il23_e_s1(il23_e_s1),
    .s0(s0[86]),
    .s1(s1[86]),
    .il23r_s0(il23r_s0),
    .il23r_s1(il23r_s1)
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
    .init_state(init_state[87]),
    .foxp3_s0(foxp3_s0),
    .foxp3_s1(foxp3_s1),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .s0(s0[87]),
    .s1(s1[87]),
    .tgfb_s0(tgfb_s0),
    .tgfb_s1(tgfb_s1)
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
    .init_state(init_state[88]),
    .cd28_s0(cd28_s0),
    .cd28_s1(cd28_s1),
    .icos_s0(icos_s0),
    .icos_s1(icos_s1),
    .il2r_s0(il2r_s0),
    .il2r_s1(il2r_s1),
    .shp2_s0(shp2_s0),
    .shp2_s1(shp2_s1),
    .gab2_s0(gab2_s0),
    .gab2_s1(gab2_s1),
    .ras_s0(ras_s0),
    .ras_s1(ras_s1),
    .fak_576_577_s0(fak_576_577_s0),
    .fak_576_577_s1(fak_576_577_s1),
    .s0(s0[88]),
    .s1(s1[88]),
    .pi3k_s0(pi3k_s0),
    .pi3k_s1(pi3k_s1)
  );


  no_lck
  _no_lck
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[89]),
    .cd28_s0(cd28_s0),
    .cd28_s1(cd28_s1),
    .jak3_s0(jak3_s0),
    .jak3_s1(jak3_s1),
    .il2rb_s0(il2rb_s0),
    .il2rb_s1(il2rb_s1),
    .cd4_s0(cd4_s0),
    .cd4_s1(cd4_s1),
    .s0(s0[89]),
    .s1(s1[89]),
    .lck_s0(lck_s0),
    .lck_s1(lck_s1)
  );


  no_syk
  _no_syk
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[90]),
    .il2r_s0(il2r_s0),
    .il2r_s1(il2r_s1),
    .s0(s0[90]),
    .s1(s1[90]),
    .syk_s0(syk_s0),
    .syk_s1(syk_s1)
  );


  no_camp
  _no_camp
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[91]),
    .adenyl_cyclase_s0(adenyl_cyclase_s0),
    .adenyl_cyclase_s1(adenyl_cyclase_s1),
    .s0(s0[91]),
    .s1(s1[91]),
    .camp_s0(camp_s0),
    .camp_s1(camp_s1)
  );


  no_nik
  _no_nik
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[92]),
    .traf6_s0(traf6_s0),
    .traf6_s1(traf6_s1),
    .s0(s0[92]),
    .s1(s1[92]),
    .nik_s0(nik_s0),
    .nik_s1(nik_s1)
  );


  no_stat6
  _no_stat6
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[93]),
    .il4r_s0(il4r_s0),
    .il4r_s1(il4r_s1),
    .s0(s0[93]),
    .s1(s1[93]),
    .stat6_s0(stat6_s0),
    .stat6_s1(stat6_s1)
  );


  no_il2r_high
  _no_il2r_high
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[94]),
    .il2_s0(il2_s0),
    .il2_s1(il2_s1),
    .il2ra_s0(il2ra_s0),
    .il2ra_s1(il2ra_s1),
    .cgc_s0(cgc_s0),
    .cgc_s1(cgc_s1),
    .il2rb_s0(il2rb_s0),
    .il2rb_s1(il2rb_s1),
    .il2_e_s0(il2_e_s0),
    .il2_e_s1(il2_e_s1),
    .s0(s0[94]),
    .s1(s1[94]),
    .il2r_high_s0(il2r_high_s0),
    .il2r_high_s1(il2r_high_s1)
  );


  no_mek6
  _no_mek6
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[95]),
    .mekk4_s0(mekk4_s0),
    .mekk4_s1(mekk4_s1),
    .s0(s0[95]),
    .s1(s1[95]),
    .mek6_s0(mek6_s0),
    .mek6_s1(mek6_s1)
  );


  no_braf
  _no_braf
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[96]),
    .rap1_s0(rap1_s0),
    .rap1_s1(rap1_s1),
    .s0(s0[96]),
    .s1(s1[96]),
    .braf_s0(braf_s0),
    .braf_s1(braf_s1)
  );


  no_mlc
  _no_mlc
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[97]),
    .rock_s0(rock_s0),
    .rock_s1(rock_s1),
    .s0(s0[97]),
    .s1(s1[97]),
    .mlc_s0(mlc_s0),
    .mlc_s1(mlc_s1)
  );


  no_mkk7
  _no_mkk7
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[98]),
    .tak1_s0(tak1_s0),
    .tak1_s1(tak1_s1),
    .s0(s0[98]),
    .s1(s1[98]),
    .mkk7_s0(mkk7_s0),
    .mkk7_s1(mkk7_s1)
  );


  no_nck
  _no_nck
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[99]),
    .slp76_s0(slp76_s0),
    .slp76_s1(slp76_s1),
    .s0(s0[99]),
    .s1(s1[99]),
    .nck_s0(nck_s0),
    .nck_s1(nck_s1)
  );


  no_pka
  _no_pka
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[100]),
    .camp_s0(camp_s0),
    .camp_s1(camp_s1),
    .s0(s0[100]),
    .s1(s1[100]),
    .pka_s0(pka_s0),
    .pka_s1(pka_s1)
  );


  no_irf1
  _no_irf1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[101]),
    .stat1_s0(stat1_s0),
    .stat1_s1(stat1_s1),
    .s0(s0[101]),
    .s1(s1[101]),
    .irf1_s0(irf1_s0),
    .irf1_s1(irf1_s1)
  );


  no_limk
  _no_limk
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[102]),
    .pak_s0(pak_s0),
    .pak_s1(pak_s1),
    .rock_s0(rock_s0),
    .rock_s1(rock_s1),
    .s0(s0[102]),
    .s1(s1[102]),
    .limk_s0(limk_s0),
    .limk_s1(limk_s1)
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
    .init_state(init_state[103]),
    .pdk1_s0(pdk1_s0),
    .pdk1_s1(pdk1_s1),
    .s0(s0[103]),
    .s1(s1[103]),
    .akt_s0(akt_s0),
    .akt_s1(akt_s1)
  );


  no_carma1
  _no_carma1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[104]),
    .cd26_s0(cd26_s0),
    .cd26_s1(cd26_s1),
    .pkc_s0(pkc_s0),
    .pkc_s1(pkc_s1),
    .s0(s0[104]),
    .s1(s1[104]),
    .carma1_s0(carma1_s0),
    .carma1_s1(carma1_s1)
  );


  no_ca2
  _no_ca2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[105]),
    .ip3_s0(ip3_s0),
    .ip3_s1(ip3_s1),
    .s0(s0[105]),
    .s1(s1[105]),
    .ca2_s0(ca2_s0),
    .ca2_s1(ca2_s1)
  );


  no_fyn
  _no_fyn
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[106]),
    .cd3_s0(cd3_s0),
    .cd3_s1(cd3_s1),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1),
    .cav1_scaffold_s0(cav1_scaffold_s0),
    .cav1_scaffold_s1(cav1_scaffold_s1),
    .s0(s0[106]),
    .s1(s1[106]),
    .fyn_s0(fyn_s0),
    .fyn_s1(fyn_s1)
  );


  no_bcl10_malt1
  _no_bcl10_malt1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[107]),
    .carma1_s0(carma1_s0),
    .carma1_s1(carma1_s1),
    .s0(s0[107]),
    .s1(s1[107]),
    .bcl10_malt1_s0(bcl10_malt1_s0),
    .bcl10_malt1_s1(bcl10_malt1_s1)
  );


  no_gadd45g
  _no_gadd45g
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[108]),
    .cd3_s0(cd3_s0),
    .cd3_s1(cd3_s1),
    .il12_e_s0(il12_e_s0),
    .il12_e_s1(il12_e_s1),
    .s0(s0[108]),
    .s1(s1[108]),
    .gadd45g_s0(gadd45g_s0),
    .gadd45g_s1(gadd45g_s1)
  );


  no_il18r1
  _no_il18r1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[109]),
    .il18_e_s0(il18_e_s0),
    .il18_e_s1(il18_e_s1),
    .s0(s0[109]),
    .s1(s1[109]),
    .il18r1_s0(il18r1_s0),
    .il18r1_s1(il18r1_s1)
  );


  no_rac1
  _no_rac1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[110]),
    .was_s0(was_s0),
    .was_s1(was_s1),
    .nos2a_s0(nos2a_s0),
    .nos2a_s1(nos2a_s1),
    .crk_s0(crk_s0),
    .crk_s1(crk_s1),
    .paxillin_s0(paxillin_s0),
    .paxillin_s1(paxillin_s1),
    .vav_s0(vav_s0),
    .vav_s1(vav_s1),
    .s0(s0[110]),
    .s1(s1[110]),
    .rac1_s0(rac1_s0),
    .rac1_s1(rac1_s1)
  );


  no_ip3
  _no_ip3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[111]),
    .plcg_s0(plcg_s0),
    .plcg_s1(plcg_s1),
    .s0(s0[111]),
    .s1(s1[111]),
    .ip3_s0(ip3_s0),
    .ip3_s1(ip3_s1)
  );


  no_irsp53
  _no_irsp53
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[112]),
    .rac1_s0(rac1_s0),
    .rac1_s1(rac1_s1),
    .s0(s0[112]),
    .s1(s1[112]),
    .irsp53_s0(irsp53_s0),
    .irsp53_s1(irsp53_s1)
  );


  no_tcr
  _no_tcr
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[113]),
    .apc_s0(apc_s0),
    .apc_s1(apc_s1),
    .cd28_s0(cd28_s0),
    .cd28_s1(cd28_s1),
    .s0(s0[113]),
    .s1(s1[113]),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1)
  );


  no_ifnbr
  _no_ifnbr
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[114]),
    .ifnb_e_s0(ifnb_e_s0),
    .ifnb_e_s1(ifnb_e_s1),
    .s0(s0[114]),
    .s1(s1[114]),
    .ifnbr_s0(ifnbr_s0),
    .ifnbr_s1(ifnbr_s1)
  );


  no_sos
  _no_sos
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[115]),
    .grb2_s0(grb2_s0),
    .grb2_s1(grb2_s1),
    .s0(s0[115]),
    .s1(s1[115]),
    .sos_s0(sos_s0),
    .sos_s1(sos_s1)
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
    .init_state(init_state[116]),
    .irf4_s0(irf4_s0),
    .irf4_s1(irf4_s1),
    .gata3_s0(gata3_s0),
    .gata3_s1(gata3_s1),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .foxp3_s0(foxp3_s0),
    .foxp3_s1(foxp3_s1),
    .tbet_s0(tbet_s0),
    .tbet_s1(tbet_s1),
    .runx3_s0(runx3_s0),
    .runx3_s1(runx3_s1),
    .irf1_s0(irf1_s0),
    .irf1_s1(irf1_s1),
    .s0(s0[116]),
    .s1(s1[116]),
    .il4_s0(il4_s0),
    .il4_s1(il4_s1)
  );


  no_socs1
  _no_socs1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[117]),
    .stat6_s0(stat6_s0),
    .stat6_s1(stat6_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .s0(s0[117]),
    .s1(s1[117]),
    .socs1_s0(socs1_s0),
    .socs1_s1(socs1_s1)
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
    .init_state(init_state[118]),
    .stat5_high_s0(stat5_high_s0),
    .stat5_high_s1(stat5_high_s1),
    .s0(s0[118]),
    .s1(s1[118]),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1)
  );


  no_pkc
  _no_pkc
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[119]),
    .dag_s0(dag_s0),
    .dag_s1(dag_s1),
    .s0(s0[119]),
    .s1(s1[119]),
    .pkc_s0(pkc_s0),
    .pkc_s1(pkc_s1)
  );


  no_mek3
  _no_mek3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[120]),
    .mekk4_s0(mekk4_s0),
    .mekk4_s1(mekk4_s1),
    .s0(s0[120]),
    .s1(s1[120]),
    .mek3_s0(mek3_s0),
    .mek3_s1(mek3_s1)
  );


  no_grb2
  _no_grb2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[121]),
    .lat_s0(lat_s0),
    .lat_s1(lat_s1),
    .shc1_s0(shc1_s0),
    .shc1_s1(shc1_s1),
    .s0(s0[121]),
    .s1(s1[121]),
    .grb2_s0(grb2_s0),
    .grb2_s1(grb2_s1)
  );


  no_calcineurin
  _no_calcineurin
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[122]),
    .ca2_s0(ca2_s0),
    .ca2_s1(ca2_s1),
    .s0(s0[122]),
    .s1(s1[122]),
    .calcineurin_s0(calcineurin_s0),
    .calcineurin_s1(calcineurin_s1)
  );


  no_pak
  _no_pak
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[123]),
    .rac1_s0(rac1_s0),
    .rac1_s1(rac1_s1),
    .cdc42_s0(cdc42_s0),
    .cdc42_s1(cdc42_s1),
    .nck_s0(nck_s0),
    .nck_s1(nck_s1),
    .s0(s0[123]),
    .s1(s1[123]),
    .pak_s0(pak_s0),
    .pak_s1(pak_s1)
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
    .init_state(init_state[124]),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .gata3_s0(gata3_s0),
    .gata3_s1(gata3_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1),
    .s0(s0[124]),
    .s1(s1[124]),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1)
  );


  no_c3g
  _no_c3g
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[125]),
    .crk_s0(crk_s0),
    .crk_s1(crk_s1),
    .s0(s0[125]),
    .s1(s1[125]),
    .c3g_s0(c3g_s0),
    .c3g_s1(c3g_s1)
  );


  no_il12rb2
  _no_il12rb2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[126]),
    .il12_e_s0(il12_e_s0),
    .il12_e_s1(il12_e_s1),
    .s0(s0[126]),
    .s1(s1[126]),
    .il12rb2_s0(il12rb2_s0),
    .il12rb2_s1(il12rb2_s1)
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
    .init_state(init_state[127]),
    .stat4_s0(stat4_s0),
    .stat4_s1(stat4_s1),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .foxp3_s0(foxp3_s0),
    .foxp3_s1(foxp3_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .atf2_s0(atf2_s0),
    .atf2_s1(atf2_s1),
    .ap1_s0(ap1_s0),
    .ap1_s1(ap1_s1),
    .runx3_s0(runx3_s0),
    .runx3_s1(runx3_s1),
    .tbet_s0(tbet_s0),
    .tbet_s1(tbet_s1),
    .hlx_s0(hlx_s0),
    .hlx_s1(hlx_s1),
    .s0(s0[127]),
    .s1(s1[127]),
    .ifng_s0(ifng_s0),
    .ifng_s1(ifng_s1)
  );


  no_erm
  _no_erm
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[128]),
    .stat4_s0(stat4_s0),
    .stat4_s1(stat4_s1),
    .s0(s0[128]),
    .s1(s1[128]),
    .erm_s0(erm_s0),
    .erm_s1(erm_s1)
  );


  no_jak1
  _no_jak1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[129]),
    .jak3_s0(jak3_s0),
    .jak3_s1(jak3_s1),
    .socs3_s0(socs3_s0),
    .socs3_s1(socs3_s1),
    .il2r_s0(il2r_s0),
    .il2r_s1(il2r_s1),
    .il9r_s0(il9r_s0),
    .il9r_s1(il9r_s1),
    .il22r_s0(il22r_s0),
    .il22r_s1(il22r_s1),
    .s0(s0[129]),
    .s1(s1[129]),
    .jak1_s0(jak1_s0),
    .jak1_s1(jak1_s1)
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
    .init_state(init_state[130]),
    .il21r_s0(il21r_s0),
    .il21r_s1(il21r_s1),
    .il27r_s0(il27r_s0),
    .il27r_s1(il27r_s1),
    .il6r_s0(il6r_s0),
    .il6r_s1(il6r_s1),
    .il23r_s0(il23r_s0),
    .il23r_s1(il23r_s1),
    .il10r_s0(il10r_s0),
    .il10r_s1(il10r_s1),
    .s0(s0[130]),
    .s1(s1[130]),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1)
  );


  no_il2r
  _no_il2r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[131]),
    .il2_s0(il2_s0),
    .il2_s1(il2_s1),
    .cgc_s0(cgc_s0),
    .cgc_s1(cgc_s1),
    .il2rb_s0(il2rb_s0),
    .il2rb_s1(il2rb_s1),
    .il2ra_s0(il2ra_s0),
    .il2ra_s1(il2ra_s1),
    .il2_e_s0(il2_e_s0),
    .il2_e_s1(il2_e_s1),
    .s0(s0[131]),
    .s1(s1[131]),
    .il2r_s0(il2r_s0),
    .il2r_s1(il2r_s1)
  );


  no_shc1
  _no_shc1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[132]),
    .fyn_s0(fyn_s0),
    .fyn_s1(fyn_s1),
    .il2rb_s0(il2rb_s0),
    .il2rb_s1(il2rb_s1),
    .il2r_s0(il2r_s0),
    .il2r_s1(il2r_s1),
    .s0(s0[132]),
    .s1(s1[132]),
    .shc1_s0(shc1_s0),
    .shc1_s1(shc1_s1)
  );


  no_pdk1
  _no_pdk1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[133]),
    .pip3_345_s0(pip3_345_s0),
    .pip3_345_s1(pip3_345_s1),
    .s0(s0[133]),
    .s1(s1[133]),
    .pdk1_s0(pdk1_s0),
    .pdk1_s1(pdk1_s1)
  );


  no_cd3
  _no_cd3
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[134]),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1),
    .s0(s0[134]),
    .s1(s1[134]),
    .cd3_s0(cd3_s0),
    .cd3_s1(cd3_s1)
  );


  no_il21r
  _no_il21r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[135]),
    .il21_e_s0(il21_e_s0),
    .il21_e_s1(il21_e_s1),
    .gp130_s0(gp130_s0),
    .gp130_s1(gp130_s1),
    .cgc_s0(cgc_s0),
    .cgc_s1(cgc_s1),
    .il21_s0(il21_s0),
    .il21_s1(il21_s1),
    .s0(s0[135]),
    .s1(s1[135]),
    .il21r_s0(il21r_s0),
    .il21r_s1(il21r_s1)
  );


  no_jak2
  _no_jak2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[136]),
    .il12rb1_s0(il12rb1_s0),
    .il12rb1_s1(il12rb1_s1),
    .il12rb2_s0(il12rb2_s0),
    .il12rb2_s1(il12rb2_s1),
    .s0(s0[136]),
    .s1(s1[136]),
    .jak2_s0(jak2_s0),
    .jak2_s1(jak2_s1)
  );


  no_profilin
  _no_profilin
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[137]),
    .riam_s0(riam_s0),
    .riam_s1(riam_s1),
    .s0(s0[137]),
    .s1(s1[137]),
    .profilin_s0(profilin_s0),
    .profilin_s1(profilin_s1)
  );


  no_nfat
  _no_nfat
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[138]),
    .cd28_s0(cd28_s0),
    .cd28_s1(cd28_s1),
    .tcr_s0(tcr_s0),
    .tcr_s1(tcr_s1),
    .calcineurin_s0(calcineurin_s0),
    .calcineurin_s1(calcineurin_s1),
    .p38_s0(p38_s0),
    .p38_s1(p38_s1),
    .gsk3b_s0(gsk3b_s0),
    .gsk3b_s1(gsk3b_s1),
    .s0(s0[138]),
    .s1(s1[138]),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1)
  );


  no_icos
  _no_icos
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[139]),
    .apc_s0(apc_s0),
    .apc_s1(apc_s1),
    .s0(s0[139]),
    .s1(s1[139]),
    .icos_s0(icos_s0),
    .icos_s1(icos_s1)
  );


  no_irak1
  _no_irak1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[140]),
    .il18r1_s0(il18r1_s0),
    .il18r1_s1(il18r1_s1),
    .s0(s0[140]),
    .s1(s1[140]),
    .irak1_s0(irak1_s0),
    .irak1_s1(irak1_s1)
  );


  no_adenyl_cyclase
  _no_adenyl_cyclase
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[141]),
    .galphas_r_s0(galphas_r_s0),
    .galphas_r_s1(galphas_r_s1),
    .s0(s0[141]),
    .s1(s1[141]),
    .adenyl_cyclase_s0(adenyl_cyclase_s0),
    .adenyl_cyclase_s1(adenyl_cyclase_s1)
  );


  no_was
  _no_was
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[142]),
    .src_s0(src_s0),
    .src_s1(src_s1),
    .s0(s0[142]),
    .s1(s1[142]),
    .was_s0(was_s0),
    .was_s1(was_s1)
  );


  no_rasgrp
  _no_rasgrp
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[143]),
    .dag_s0(dag_s0),
    .dag_s1(dag_s1),
    .s0(s0[143]),
    .s1(s1[143]),
    .rasgrp_s0(rasgrp_s0),
    .rasgrp_s1(rasgrp_s1)
  );


  no_il10r
  _no_il10r
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[144]),
    .il10_s0(il10_s0),
    .il10_s1(il10_s1),
    .il10rb_s0(il10rb_s0),
    .il10rb_s1(il10rb_s1),
    .il10ra_s0(il10ra_s0),
    .il10ra_s1(il10ra_s1),
    .il10_e_s0(il10_e_s0),
    .il10_e_s1(il10_e_s1),
    .s0(s0[144]),
    .s1(s1[144]),
    .il10r_s0(il10r_s0),
    .il10r_s1(il10r_s1)
  );


  no_il4r_high
  _no_il4r_high
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[145]),
    .il4_s0(il4_s0),
    .il4_s1(il4_s1),
    .cgc_s0(cgc_s0),
    .cgc_s1(cgc_s1),
    .il4ra_high_s0(il4ra_high_s0),
    .il4ra_high_s1(il4ra_high_s1),
    .il4_e_s0(il4_e_s0),
    .il4_e_s1(il4_e_s1),
    .s0(s0[145]),
    .s1(s1[145]),
    .il4r_high_s0(il4r_high_s0),
    .il4r_high_s1(il4r_high_s1)
  );


  no_cas
  _no_cas
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[146]),
    .fak_576_577_s0(fak_576_577_s0),
    .fak_576_577_s1(fak_576_577_s1),
    .bintegrin_s0(bintegrin_s0),
    .bintegrin_s1(bintegrin_s1),
    .s0(s0[146]),
    .s1(s1[146]),
    .cas_s0(cas_s0),
    .cas_s1(cas_s1)
  );


  no_rorgt
  _no_rorgt
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[147]),
    .tgfbr_s0(tgfbr_s0),
    .tgfbr_s1(tgfbr_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .s0(s0[147]),
    .s1(s1[147]),
    .rorgt_s0(rorgt_s0),
    .rorgt_s1(rorgt_s1)
  );


  no_plcb
  _no_plcb
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[148]),
    .galpha_q_s0(galpha_q_s0),
    .galpha_q_s1(galpha_q_s1),
    .s0(s0[148]),
    .s1(s1[148]),
    .plcb_s0(plcb_s0),
    .plcb_s1(plcb_s1)
  );


  no_zap70
  _no_zap70
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[149]),
    .itams_s0(itams_s0),
    .itams_s1(itams_s1),
    .cd3_s0(cd3_s0),
    .cd3_s1(cd3_s1),
    .s0(s0[149]),
    .s1(s1[149]),
    .zap70_s0(zap70_s0),
    .zap70_s1(zap70_s1)
  );


  no_bcl10_carma1_malti
  _no_bcl10_carma1_malti
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[150]),
    .bcl10_malt1_s0(bcl10_malt1_s0),
    .bcl10_malt1_s1(bcl10_malt1_s1),
    .carma1_s0(carma1_s0),
    .carma1_s1(carma1_s1),
    .s0(s0[150]),
    .s1(s1[150]),
    .bcl10_carma1_malti_s0(bcl10_carma1_malti_s0),
    .bcl10_carma1_malti_s1(bcl10_carma1_malti_s1)
  );


  no_cdc42
  _no_cdc42
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[151]),
    .c3g_s0(c3g_s0),
    .c3g_s1(c3g_s1),
    .rhogef_s0(rhogef_s0),
    .rhogef_s1(rhogef_s1),
    .s0(s0[151]),
    .s1(s1[151]),
    .cdc42_s0(cdc42_s0),
    .cdc42_s1(cdc42_s1)
  );


  no_il23
  _no_il23
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[152]),
    .nfat_s0(nfat_s0),
    .nfat_s1(nfat_s1),
    .stat3_s0(stat3_s0),
    .stat3_s1(stat3_s1),
    .proliferation_s0(proliferation_s0),
    .proliferation_s1(proliferation_s1),
    .s0(s0[152]),
    .s1(s1[152]),
    .il23_s0(il23_s0),
    .il23_s1(il23_s1)
  );


  no_tbet
  _no_tbet
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[153]),
    .gata3_s0(gata3_s0),
    .gata3_s1(gata3_s1),
    .stat1_s0(stat1_s0),
    .stat1_s1(stat1_s1),
    .s0(s0[153]),
    .s1(s1[153]),
    .tbet_s0(tbet_s0),
    .tbet_s1(tbet_s1)
  );


  no_alpha_13l
  _no_alpha_13l
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[154]),
    .s0(s0[154]),
    .s1(s1[154]),
    .alpha_13l_s0(alpha_13l_s0),
    .alpha_13l_s1(alpha_13l_s1)
  );


  no_ifnb_e
  _no_ifnb_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[155]),
    .s0(s0[155]),
    .s1(s1[155]),
    .ifnb_e_s0(ifnb_e_s0),
    .ifnb_e_s1(ifnb_e_s1)
  );


  no_mhc_ii
  _no_mhc_ii
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[156]),
    .s0(s0[156]),
    .s1(s1[156]),
    .mhc_ii_s0(mhc_ii_s0),
    .mhc_ii_s1(mhc_ii_s1)
  );


  no_il12_e
  _no_il12_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[157]),
    .s0(s0[157]),
    .s1(s1[157]),
    .il12_e_s0(il12_e_s0),
    .il12_e_s1(il12_e_s1)
  );


  no_il23_e
  _no_il23_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[158]),
    .s0(s0[158]),
    .s1(s1[158]),
    .il23_e_s0(il23_e_s0),
    .il23_e_s1(il23_e_s1)
  );


  no_il15_e
  _no_il15_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[159]),
    .s0(s0[159]),
    .s1(s1[159]),
    .il15_e_s0(il15_e_s0),
    .il15_e_s1(il15_e_s1)
  );


  no_ifngr2
  _no_ifngr2
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[160]),
    .s0(s0[160]),
    .s1(s1[160]),
    .ifngr2_s0(ifngr2_s0),
    .ifngr2_s1(ifngr2_s1)
  );


  no_il21_e
  _no_il21_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[161]),
    .s0(s0[161]),
    .s1(s1[161]),
    .il21_e_s0(il21_e_s0),
    .il21_e_s1(il21_e_s1)
  );


  no_il6_e
  _no_il6_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[162]),
    .s0(s0[162]),
    .s1(s1[162]),
    .il6_e_s0(il6_e_s0),
    .il6_e_s1(il6_e_s1)
  );


  no_il2rb
  _no_il2rb
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[163]),
    .s0(s0[163]),
    .s1(s1[163]),
    .il2rb_s0(il2rb_s0),
    .il2rb_s1(il2rb_s1)
  );


  no_il4_e
  _no_il4_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[164]),
    .s0(s0[164]),
    .s1(s1[164]),
    .il4_e_s0(il4_e_s0),
    .il4_e_s1(il4_e_s1)
  );


  no_il10_e
  _no_il10_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[165]),
    .s0(s0[165]),
    .s1(s1[165]),
    .il10_e_s0(il10_e_s0),
    .il10_e_s1(il10_e_s1)
  );


  no_tgfb_e
  _no_tgfb_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[166]),
    .s0(s0[166]),
    .s1(s1[166]),
    .tgfb_e_s0(tgfb_e_s0),
    .tgfb_e_s1(tgfb_e_s1)
  );


  no_il15ra
  _no_il15ra
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[167]),
    .s0(s0[167]),
    .s1(s1[167]),
    .il15ra_s0(il15ra_s0),
    .il15ra_s1(il15ra_s1)
  );


  no_ifngr1
  _no_ifngr1
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[168]),
    .s0(s0[168]),
    .s1(s1[168]),
    .ifngr1_s0(ifngr1_s0),
    .ifngr1_s1(ifngr1_s1)
  );


  no_il2_e
  _no_il2_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[169]),
    .s0(s0[169]),
    .s1(s1[169]),
    .il2_e_s0(il2_e_s0),
    .il2_e_s1(il2_e_s1)
  );


  no_galpha_il
  _no_galpha_il
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[170]),
    .s0(s0[170]),
    .s1(s1[170]),
    .galpha_il_s0(galpha_il_s0),
    .galpha_il_s1(galpha_il_s1)
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
    .init_state(init_state[171]),
    .s0(s0[171]),
    .s1(s1[171]),
    .gp130_s0(gp130_s0),
    .gp130_s1(gp130_s1)
  );


  no_galphas_l
  _no_galphas_l
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[172]),
    .s0(s0[172]),
    .s1(s1[172]),
    .galphas_l_s0(galphas_l_s0),
    .galphas_l_s1(galphas_l_s1)
  );


  no_il10rb
  _no_il10rb
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[173]),
    .s0(s0[173]),
    .s1(s1[173]),
    .il10rb_s0(il10rb_s0),
    .il10rb_s1(il10rb_s1)
  );


  no_il27ra
  _no_il27ra
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[174]),
    .s0(s0[174]),
    .s1(s1[174]),
    .il27ra_s0(il27ra_s0),
    .il27ra_s1(il27ra_s1)
  );


  no_il27_e
  _no_il27_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[175]),
    .s0(s0[175]),
    .s1(s1[175]),
    .il27_e_s0(il27_e_s0),
    .il27_e_s1(il27_e_s1)
  );


  no_il6ra
  _no_il6ra
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[176]),
    .s0(s0[176]),
    .s1(s1[176]),
    .il6ra_s0(il6ra_s0),
    .il6ra_s1(il6ra_s1)
  );


  no_cgc
  _no_cgc
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[177]),
    .s0(s0[177]),
    .s1(s1[177]),
    .cgc_s0(cgc_s0),
    .cgc_s1(cgc_s1)
  );


  no_galpha_ql
  _no_galpha_ql
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[178]),
    .s0(s0[178]),
    .s1(s1[178]),
    .galpha_ql_s0(galpha_ql_s0),
    .galpha_ql_s1(galpha_ql_s1)
  );


  no_cav1_activator
  _no_cav1_activator
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[179]),
    .s0(s0[179]),
    .s1(s1[179]),
    .cav1_activator_s0(cav1_activator_s0),
    .cav1_activator_s1(cav1_activator_s1)
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
    .init_state(init_state[180]),
    .s0(s0[180]),
    .s1(s1[180]),
    .apc_s0(apc_s0),
    .apc_s1(apc_s1)
  );


  no_ifng_e
  _no_ifng_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[181]),
    .s0(s0[181]),
    .s1(s1[181]),
    .ifng_e_s0(ifng_e_s0),
    .ifng_e_s1(ifng_e_s1)
  );


  no_b7
  _no_b7
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[182]),
    .s0(s0[182]),
    .s1(s1[182]),
    .b7_s0(b7_s0),
    .b7_s1(b7_s1)
  );


  no_il18_e
  _no_il18_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[183]),
    .s0(s0[183]),
    .s1(s1[183]),
    .il18_e_s0(il18_e_s0),
    .il18_e_s1(il18_e_s1)
  );


  no_il9_e
  _no_il9_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[184]),
    .s0(s0[184]),
    .s1(s1[184]),
    .il9_e_s0(il9_e_s0),
    .il9_e_s1(il9_e_s1)
  );


  no_ecm
  _no_ecm
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[185]),
    .s0(s0[185]),
    .s1(s1[185]),
    .ecm_s0(ecm_s0),
    .ecm_s1(ecm_s1)
  );


  no_il22_e
  _no_il22_e
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[186]),
    .s0(s0[186]),
    .s1(s1[186]),
    .il22_e_s0(il22_e_s0),
    .il22_e_s1(il22_e_s1)
  );


  no_il10ra
  _no_il10ra
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[187]),
    .s0(s0[187]),
    .s1(s1[187]),
    .il10ra_s0(il10ra_s0),
    .il10ra_s1(il10ra_s1)
  );


endmodule
