
module arbiter_controller_wr_req_8 #
(
  parameter DATA_WIDTH = 32,
  parameter INPUT_FIFO_DEPTH_BITS = 4,
  parameter OUTPUT_FIFO_DEPTH_BITS = 4
)
(
  input clk,
  input rst,
  input [8-1:0] req_wr_en_in,
  input [DATA_WIDTH*8-1:0] req_wr_data_in,
  output reg [8-1:0] req_wr_available_in,
  input req_wr_available_out,
  output reg req_wr_en_out,
  output reg [DATA_WIDTH-1:0] req_wr_data_out
);

  localparam IN_FIFO_ALMOSTFULL_THRESHOLD = 2 ** INPUT_FIFO_DEPTH_BITS - 8;
  localparam IN_FIFO_ALMOSTEMPTY_THRESHOLD = 4;

  localparam OUT_FIFO_ALMOSTFULL_THRESHOLD = 2 ** OUTPUT_FIFO_DEPTH_BITS - 8;
  localparam OUT_FIFO_ALMOSTEMPTY_THRESHOLD = 2;

  wire [8-1:0] in_fifo_empty;
  wire [(INPUT_FIFO_DEPTH_BITS+1)*8-1:0] in_fifo_count;
  wire [8-1:0] in_fifo_full;
  wire [8-1:0] in_fifo_almostfull;
  wire [8-1:0] in_fifo_almostempty;

  reg rst_reg;

  reg [8-1:0] in_fifo_we;
  reg [DATA_WIDTH*8-1:0] in_fifo_din;
  reg [3-1:0] in_fifo_re_count;
  reg [2-1:0] fsm_in_fifo_re;

  reg [8-1:0] in_fifo_re;
  reg [8-1:0] in_fifo_re_next;
  reg in_fifo_flag;
  wire [8-1:0] in_fifo_dout_valid;
  wire [DATA_WIDTH*8-1:0] in_fifo_dout;

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

  wire [8-1:0] arbiter_request;
  wire [8-1:0] arbiter_grant;
  wire arbiter_grant_valid;
  wire [3-1:0] arbiter_grant_encoded;

  wire select_dout_valid;
  wire [DATA_WIDTH-1:0] select_dout;

  integer it;
  genvar idx;

  generate for(idx=0; idx<8; idx=idx+1) begin : gen_in_fifos

    fifo
    #(
      .FIFO_WIDTH(DATA_WIDTH),
      .FIFO_DEPTH_BITS(INPUT_FIFO_DEPTH_BITS),
      .FIFO_ALMOSTFULL_THRESHOLD(IN_FIFO_ALMOSTFULL_THRESHOLD),
      .FIFO_ALMOSTEMPTY_THRESHOLD(IN_FIFO_ALMOSTEMPTY_THRESHOLD)
    )
    in_fifo
    (
      .clk(clk),
      .rst(rst_reg),
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


  select_top_8
  #(
    .DATA_WIDTH(DATA_WIDTH)
  )
  select
  (
    .clk(clk),
    .rst(rst_reg),
    .data_in_valid(in_fifo_dout_valid),
    .data_in_0(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*0)-1:DATA_WIDTH*0]),
    .data_in_1(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*1)-1:DATA_WIDTH*1]),
    .data_in_2(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*2)-1:DATA_WIDTH*2]),
    .data_in_3(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*3)-1:DATA_WIDTH*3]),
    .data_in_4(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*4)-1:DATA_WIDTH*4]),
    .data_in_5(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*5)-1:DATA_WIDTH*5]),
    .data_in_6(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*6)-1:DATA_WIDTH*6]),
    .data_in_7(in_fifo_dout[(DATA_WIDTH+DATA_WIDTH*7)-1:DATA_WIDTH*7]),
    .data_out_valid(select_dout_valid),
    .data_out(select_dout)
  );


  fifo
  #(
    .FIFO_WIDTH(DATA_WIDTH),
    .FIFO_DEPTH_BITS(OUTPUT_FIFO_DEPTH_BITS),
    .FIFO_ALMOSTFULL_THRESHOLD(OUT_FIFO_ALMOSTFULL_THRESHOLD),
    .FIFO_ALMOSTEMPTY_THRESHOLD(OUT_FIFO_ALMOSTEMPTY_THRESHOLD)
  )
  out_fifo
  (
    .clk(clk),
    .rst(rst_reg),
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


  arbiter
  #(
    .PORTS(8),
    .TYPE("ROUND_ROBIN"),
    .BLOCK("NONE"),
    .LSB_PRIORITY("LOW")
  )
  arbiter
  (
    .clk(clk),
    .rst(rst_reg),
    .request(arbiter_request),
    .acknowledge({ 8{ 1'b0 } }),
    .grant(arbiter_grant),
    .grant_valid(arbiter_grant_valid),
    .grant_encoded(arbiter_grant_encoded)
  );


  generate for(idx=0; idx<8; idx=idx+1) begin : gen_req
    assign arbiter_request[idx] = ~in_fifo_almostempty[idx] & ~out_fifo_almostfull;
  end
  endgenerate


  always @(posedge clk) begin
    rst_reg <= rst;
  end


  always @(posedge clk) begin
    if(rst_reg) begin
      in_fifo_we <= 8'd0;
      in_fifo_din <= 0;
    end else begin
      in_fifo_we <= req_wr_en_in;
      in_fifo_din <= req_wr_data_in;
    end
  end


  always @(posedge clk) begin
    if(rst_reg) begin
      out_fifo_we <= 1'b0;
      out_fifo_din <= 0;
      out_fifo_re <= 1'b0;
    end else begin
      out_fifo_we <= select_dout_valid;
      out_fifo_din <= select_dout;
      if(out_fifo_almostempty) begin
        out_fifo_re <= req_wr_available_out & ~out_fifo_empty & ~out_fifo_re;
      end else begin
        out_fifo_re <= req_wr_available_out;
      end
    end
  end


  always @(posedge clk) begin
    if(rst_reg) begin
      req_wr_en_out <= 1'b0;
      req_wr_data_out <= 0;
    end else begin
      req_wr_en_out <= out_fifo_dout_valid;
      req_wr_data_out <= out_fifo_dout;
    end
  end


  always @(posedge clk) begin
    if(rst_reg) begin
      req_wr_available_in <= 8'd0;
    end else begin
      for(it=0; it<8; it=it+1) begin
        req_wr_available_in[it] <= ~in_fifo_almostfull[it];
      end
    end
  end


  always @(posedge clk) begin
    if(rst_reg) begin
      in_fifo_re <= 8'd0;
      in_fifo_re_count <= 3'd0;
      fsm_in_fifo_re <= 2'd0;
      in_fifo_flag <= 1'b0;
      in_fifo_re_next <= 8'd0;
    end else begin
      case(fsm_in_fifo_re)
        2'd0: begin
          if(arbiter_grant_valid & ~in_fifo_flag) begin
            in_fifo_re <= arbiter_grant;
            in_fifo_re_count <= in_fifo_re_count + 3'd1;
            fsm_in_fifo_re <= 1'b1;
          end else if(in_fifo_flag) begin
            in_fifo_flag <= 1'b0;
            in_fifo_re <= in_fifo_re_next;
            in_fifo_re_count <= in_fifo_re_count + 3'd1;
            fsm_in_fifo_re <= 1'b1;
          end else begin
            in_fifo_re <= 8'd0;
          end
        end
        2'd1: begin
          if(in_fifo_re_count[1] & in_fifo_re_count[0]) begin
            in_fifo_re_count <= 3'd0;
            if(arbiter_grant_valid && !(in_fifo_re & arbiter_grant)) begin
              fsm_in_fifo_re <= 2'd0;
              in_fifo_re_next <= arbiter_grant;
              in_fifo_flag <= 1'b1;
            end else begin
              fsm_in_fifo_re <= 2'd2;
            end
          end else begin
            in_fifo_re_count <= in_fifo_re_count + 3'd1;
          end
        end
        2'd2: begin
          in_fifo_re <= 8'd0;
          fsm_in_fifo_re <= 2'd3;
        end
        2'd3: begin
          in_fifo_re <= 8'd0;
          fsm_in_fifo_re <= 2'd0;
        end
        default: begin
        end
      endcase
    end
  end


endmodule
