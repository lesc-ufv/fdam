module fdam_arbiter_controller_rd_req_2 #
(
  parameter DATA_WIDTH = 32,
  parameter INPUT_FIFO_DEPTH_BITS = 4,
  parameter OUTPUT_FIFO_DEPTH_BITS = 4
)
(
  input clk,
  input rst,
  input [2-1:0] req_wr_en_in,
  input [DATA_WIDTH*2-1:0] req_wr_data_in,
  output reg [2-1:0] req_wr_available_in,
  input req_wr_available_out,
  output reg req_wr_en_out,
  output reg [DATA_WIDTH-1:0] req_wr_data_out
);

  localparam IN_FIFO_ALMOSTFULL_THRESHOLD = 2 ** INPUT_FIFO_DEPTH_BITS - 6;
  localparam IN_FIFO_ALMOSTEMPTY_THRESHOLD = 4;

  localparam OUT_FIFO_ALMOSTFULL_THRESHOLD = 2 ** OUTPUT_FIFO_DEPTH_BITS - 6;
  localparam OUT_FIFO_ALMOSTEMPTY_THRESHOLD = 2;

  wire [2-1:0] in_fifo_empty;
  wire [(INPUT_FIFO_DEPTH_BITS+1)*2-1:0] in_fifo_count;
  wire [2-1:0] in_fifo_full;
  wire [2-1:0] in_fifo_almostfull;
  wire [2-1:0] in_fifo_almostempty;

  reg [8-1:0] rst_reg;

  reg [2-1:0] in_fifo_we;
  reg [DATA_WIDTH*2-1:0] in_fifo_din;
  reg [2-1:0] in_fifo_re_flag;

  reg [2-1:0] in_fifo_re;
  wire [2-1:0] in_fifo_dout_valid;
  wire [DATA_WIDTH*2-1:0] in_fifo_dout;

  wire out_fifo_empty;
  wire [OUTPUT_FIFO_DEPTH_BITS+1-1:0] out_fifo_count;
  wire out_fifo_full;
  wire out_fifo_almostfull;
  wire out_fifo_almostempty;

  reg out_fifo_we;
  reg [DATA_WIDTH-1:0] out_fifo_din;

  reg out_fifo_re;
  wire out_fifo_dout_valid;
  wire [DATA_WIDTH-1:0] out_fifo_dout;

  wire [2-1:0] arbiter_request;
  wire [2-1:0] arbiter_grant;
  wire arbiter_grant_valid;
  wire [1-1:0] arbiter_grant_encoded;
  wire [2-1:0] arbiter_request_tt;
  wire [2-1:0] arbiter_grant_tt;
  wire arbiter_grant_valid_tt;
  wire [1-1:0] arbiter_grant_encoded_tt;

  wire select_dout_valid;
  wire [DATA_WIDTH-1:0] select_dout;

  integer it;
  genvar idx;

  fdam_reg_tree_4_8_2
  #(
    .DATA_WIDTH(1)
  )
  fdam_reg_tree_4_8_2
  (
    .clk(clk),
    .in(rst),
    .out_0(rst_reg[0]),
    .out_1(rst_reg[1]),
    .out_2(rst_reg[2]),
    .out_3(rst_reg[3]),
    .out_4(rst_reg[4]),
    .out_5(rst_reg[5]),
    .out_6(rst_reg[6]),
    .out_7(rst_reg[7])
  );


  generate for(idx=0; idx<2; idx=idx+1) begin : gen_in_fifos

    fdam_fifo
    #(
      .FIFO_WIDTH(DATA_WIDTH),
      .FIFO_DEPTH_BITS(INPUT_FIFO_DEPTH_BITS),
      .FIFO_ALMOSTFULL_THRESHOLD(IN_FIFO_ALMOSTFULL_THRESHOLD),
      .FIFO_ALMOSTEMPTY_THRESHOLD(IN_FIFO_ALMOSTEMPTY_THRESHOLD)
    )
    in_fifo
    (
      .clk(clk),
      .rst(rst_reg[idx]),
      .we(in_fifo_we[idx]),
      .din(in_fifo_din[idx*DATA_WIDTH+DATA_WIDTH-1:idx*DATA_WIDTH]),
      .re(in_fifo_re[idx]),
      .valid(in_fifo_dout_valid[idx]),
      .dout(in_fifo_dout[idx*DATA_WIDTH+DATA_WIDTH-1:idx*DATA_WIDTH]),
      .count(in_fifo_count[idx*(INPUT_FIFO_DEPTH_BITS+1)+(INPUT_FIFO_DEPTH_BITS+1)-1:idx*(INPUT_FIFO_DEPTH_BITS+1)]),
      .empty(in_fifo_empty[idx]),
      .full(in_fifo_full[idx]),
      .almostfull(in_fifo_almostfull[idx]),
      .almostempty(in_fifo_almostempty[idx])
    );

  end
  endgenerate


  fdam_select_top_2
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  select
  (
    .clk(clk),
    .rst(rst_reg[2]),
    .data_in_valid(in_fifo_dout_valid),
    .data_in_0(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*0)-1:DATA_WIDTH*0]),
    .data_in_1(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*1)-1:DATA_WIDTH*1]),
    .data_out_valid(select_dout_valid),
    .data_out(select_dout)
  );


  fdam_fifo
  #(
    .FIFO_WIDTH(DATA_WIDTH),
    .FIFO_DEPTH_BITS(OUTPUT_FIFO_DEPTH_BITS),
    .FIFO_ALMOSTFULL_THRESHOLD(OUT_FIFO_ALMOSTFULL_THRESHOLD),
    .FIFO_ALMOSTEMPTY_THRESHOLD(OUT_FIFO_ALMOSTEMPTY_THRESHOLD)
  )
  out_fifo
  (
    .clk(clk),
    .rst(rst_reg[3]),
    .we(out_fifo_we),
    .din(out_fifo_din),
    .re(out_fifo_re),
    .valid(out_fifo_dout_valid),
    .dout(out_fifo_dout),
    .count(out_fifo_count),
    .empty(out_fifo_empty),
    .full(out_fifo_full),
    .almostfull(out_fifo_almostfull),
    .almostempty(out_fifo_almostempty)
  );


  fdam_arbiter
  #(
    .PORTS(2),
    .TYPE("ROUND_ROBIN"),
    .BLOCK("NONE"),
    .LSB_PRIORITY("LOW")
  )
  arbiter
  (
    .clk(clk),
    .rst(rst_reg[4]),
    .request(arbiter_request),
    .acknowledge({ 2{ 1'b0 } }),
    .grant(arbiter_grant),
    .grant_valid(arbiter_grant_valid),
    .grant_encoded(arbiter_grant_encoded)
  );


  fdam_arbiter
  #(
    .PORTS(2),
    .TYPE("ROUND_ROBIN"),
    .BLOCK("NONE"),
    .LSB_PRIORITY("LOW")
  )
  arbiter_tt
  (
    .clk(clk),
    .rst(rst_reg[5]),
    .request(arbiter_request_tt),
    .acknowledge({ 2{ 1'b0 } }),
    .grant(arbiter_grant_tt),
    .grant_valid(arbiter_grant_valid_tt),
    .grant_encoded(arbiter_grant_encoded_tt)
  );


  generate for(idx=0; idx<2; idx=idx+1) begin : gen_req
    assign arbiter_request[idx] = ~in_fifo_empty[idx] & ~out_fifo_almostfull;
    assign arbiter_request_tt[idx] = ~in_fifo_almostempty[idx] & ~out_fifo_almostfull;
  end
  endgenerate


  always @(posedge clk) begin
    if(rst_reg[6]) begin
      req_wr_available_in <= 2'd0;
    end else begin
      for(it=0; it<2; it=it+1) begin
        req_wr_available_in[it] <= ~in_fifo_almostfull[it];
      end
    end
  end


  always @(posedge clk) begin
    if(rst_reg[7]) begin
      in_fifo_we <= 2'd0;
      in_fifo_re <= 2'd0;
      out_fifo_we <= 1'b0;
      out_fifo_re <= 1'b0;
      req_wr_en_out <= 1'b0;
      in_fifo_re_flag <= 2'd0;
    end else begin
      in_fifo_we <= req_wr_en_in;
      in_fifo_din <= req_wr_data_in;
      out_fifo_we <= select_dout_valid;
      out_fifo_din <= select_dout;
      out_fifo_re <= (out_fifo_almostempty)? req_wr_available_out & ~out_fifo_empty & ~out_fifo_re : req_wr_available_out;
      req_wr_en_out <= out_fifo_dout_valid;
      req_wr_data_out <= out_fifo_dout;
      in_fifo_re <= 2'd0;
      if(arbiter_grant_valid_tt) begin
        in_fifo_re <= arbiter_grant_tt;
      end 
      if(~arbiter_grant_valid_tt && arbiter_grant_valid && (in_fifo_re_flag == 2'd0)) begin
        in_fifo_re <= arbiter_grant;
        in_fifo_re_flag <= 2'd2;
      end 
      if(in_fifo_re_flag > 0) begin
        in_fifo_re_flag <= in_fifo_re_flag - 2'd1;
      end 
    end
  end

endmodule