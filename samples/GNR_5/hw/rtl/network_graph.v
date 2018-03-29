
module network_graph
(
  input clk,
  input rst,
  input start,
  input reset_nos,
  input start_s0,
  input start_s1,
  input [5-1:0] init_state,
  output [5-1:0] s0,
  output [5-1:0] s1
);

  wire ctra_s0;
  wire ctra_s1;
  wire gcra_s0;
  wire gcra_s1;
  wire scip_s0;
  wire scip_s1;
  wire dnaa_s0;
  wire dnaa_s1;
  wire ccrm_s0;
  wire ccrm_s1;

  no_ctra
  _no_ctra
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[0]),
    .gcra_s0(gcra_s0),
    .gcra_s1(gcra_s1),
    .ccrm_s0(ccrm_s0),
    .ccrm_s1(ccrm_s1),
    .scip_s0(scip_s0),
    .scip_s1(scip_s1),
    .s0(s0[0]),
    .s1(s1[0]),
    .ctra_s0(ctra_s0),
    .ctra_s1(ctra_s1)
  );


  no_gcra
  _no_gcra
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[1]),
    .dnaa_s0(dnaa_s0),
    .dnaa_s1(dnaa_s1),
    .ctra_s0(ctra_s0),
    .ctra_s1(ctra_s1),
    .s0(s0[1]),
    .s1(s1[1]),
    .gcra_s0(gcra_s0),
    .gcra_s1(gcra_s1)
  );


  no_scip
  _no_scip
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[2]),
    .ctra_s0(ctra_s0),
    .ctra_s1(ctra_s1),
    .dnaa_s0(dnaa_s0),
    .dnaa_s1(dnaa_s1),
    .s0(s0[2]),
    .s1(s1[2]),
    .scip_s0(scip_s0),
    .scip_s1(scip_s1)
  );


  no_dnaa
  _no_dnaa
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[3]),
    .ctra_s0(ctra_s0),
    .ctra_s1(ctra_s1),
    .ccrm_s0(ccrm_s0),
    .ccrm_s1(ccrm_s1),
    .gcra_s0(gcra_s0),
    .gcra_s1(gcra_s1),
    .s0(s0[3]),
    .s1(s1[3]),
    .dnaa_s0(dnaa_s0),
    .dnaa_s1(dnaa_s1)
  );


  no_ccrm
  _no_ccrm
  (
    .clk(clk),
    .start(start),
    .rst(rst),
    .reset_nos(reset_nos),
    .start_s0(start_s0),
    .start_s1(start_s1),
    .init_state(init_state[4]),
    .ctra_s0(ctra_s0),
    .ctra_s1(ctra_s1),
    .scip_s0(scip_s0),
    .scip_s1(scip_s1),
    .s0(s0[4]),
    .s1(s1[4]),
    .ccrm_s0(ccrm_s0),
    .ccrm_s1(ccrm_s1)
  );


endmodule
