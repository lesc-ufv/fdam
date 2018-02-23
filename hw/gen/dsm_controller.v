
module dsm_controller #
(
  parameter QTD_WIDTH = 32,
  parameter DSM_DATA_WIDTH = 512,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1,
  parameter NUM_CL_DSM_RD = $rtoi($ceil(NUM_INPUT_QUEUES/8)),
  parameter NUM_CL_DSM_WR = $rtoi($ceil(NUM_OUTPUT_QUEUES/8)),
  parameter NUM_CL_DSM_TOTAL = 1 + NUM_CL_DSM_RD + NUM_CL_DSM_WR,
  parameter NUM_CL_DSM_TOTAL_BITS = $rtoi($ceil($clog2(NUM_CL_DSM_TOTAL)))
)
(
  input clk,
  input rst,
  input [NUM_INPUT_QUEUES-1:0] done_rd,
  input [NUM_OUTPUT_QUEUES-1:0] done_wr,
  input done_afu,
  input [NUM_INPUT_QUEUES-1:0] afu_req_rd_data_en,
  input [NUM_OUTPUT_QUEUES-1:0] afu_req_wr_data_en,
  input afu_dsm_req_rd,
  input [NUM_CL_DSM_TOTAL_BITS-1:0] afu_dsm_addr,
  output reg afu_dsm_update,
  output reg afu_dsm_valid,
  output reg [DSM_DATA_WIDTH-1:0] afu_dsm_data
);

  reg [DSM_DATA_WIDTH-1:0] dsm_data [0:NUM_CL_DSM_TOTAL-1];
  reg [DSM_DATA_WIDTH-1:0] done;
  reg [DSM_DATA_WIDTH-1:0] done_last;
  reg [1-1:0] fsm_update_dsm;
  wire update_dsm;
  wire [QTD_WIDTH-1:0] afu_in_rd_count [0:(NUM_CL_DSM_RD*8)-1];
  wire [QTD_WIDTH-1:0] afu_in_wr_count [0:(NUM_CL_DSM_WR*8)-1];
  wire [DSM_DATA_WIDTH-1:0] dsm_data_wire [0:NUM_CL_DSM_TOTAL-1];
  integer idx;
  genvar i;

  generate for(i=0; i<NUM_INPUT_QUEUES; i=i+1) begin : gen_counter_rd

    afu_counter
    #(
      .DEPTH_BITS(QTD_WIDTH)
    )
    counter_rd
    (
      .clk(clk),
      .rst(rst),
      .en(afu_req_rd_data_en[i]),
      .dout(afu_in_rd_count[i])
    );

  end
  endgenerate


  generate for(i=0; i<NUM_OUTPUT_QUEUES; i=i+1) begin : gen_counter_wr

    afu_counter
    #(
      .DEPTH_BITS(QTD_WIDTH)
    )
    counter_wr
    (
      .clk(clk),
      .rst(rst),
      .en(afu_req_wr_data_en[i]),
      .dout(afu_in_wr_count[i])
    );

  end
  endgenerate


  generate for(i=(NUM_CL_DSM_RD*8)-1; i>=NUM_INPUT_QUEUES; i=i-1) begin : gen_zero_rd_count
    assign afu_in_rd_count[i] = { QTD_WIDTH{ 1'b0 } };
  end
  endgenerate


  generate for(i=(NUM_CL_DSM_WR*8)-1; i>=NUM_OUTPUT_QUEUES; i=i-1) begin : gen_zero_wr_count
    assign afu_in_wr_count[i] = { QTD_WIDTH{ 1'b0 } };
  end
  endgenerate


  generate for(i=0; i<NUM_CL_DSM_RD; i=i+1) begin : gen_dsm_data_rd
    assign dsm_data_wire[i] = { afu_in_rd_count[i * 8 + 7], afu_in_rd_count[i * 8 + 6], afu_in_rd_count[i * 8 + 5], afu_in_rd_count[i * 8 + 4], afu_in_rd_count[i * 8 + 3], afu_in_rd_count[i * 8 + 2], afu_in_rd_count[i * 8 + 1], afu_in_rd_count[i * 8 + 0] };
  end
  endgenerate


  generate for(i=0; i<NUM_CL_DSM_WR; i=i+1) begin : gen_dsm_data_wr
    assign dsm_data_wire[i + NUM_CL_DSM_WR] = { afu_in_wr_count[i * 8 + 7], afu_in_wr_count[i * 8 + 6], afu_in_wr_count[i * 8 + 5], afu_in_wr_count[i * 8 + 4], afu_in_wr_count[i * 8 + 3], afu_in_wr_count[i * 8 + 2], afu_in_wr_count[i * 8 + 1], afu_in_wr_count[i * 8 + 0] };
  end
  endgenerate

  assign dsm_data_wire[NUM_CL_DSM_RD + NUM_CL_DSM_WR] = done;
  assign update_dsm = done ^ done_last & done;

  always @(posedge clk) begin
    if(rst) begin
      done <= 512'd0;
    end else begin
      done <= { { DSM_DATA_WIDTH - (NUM_INPUT_QUEUES + NUM_OUTPUT_QUEUES + 1){ 1'b0 } }, done_wr, done_rd, done_afu };
    end
  end


  always @(posedge clk) begin
    afu_dsm_valid <= 1'b0;
    for(idx=0; idx<NUM_CL_DSM_TOTAL; idx=idx+1) begin
      dsm_data[idx] <= dsm_data_wire[idx];
    end
    if(afu_dsm_req_rd) begin
      afu_dsm_data <= dsm_data[afu_dsm_addr];
      afu_dsm_valid <= 1'b1;
    end 
  end


  always @(posedge clk) begin
    if(rst) begin
      afu_dsm_update <= 1'b0;
      fsm_update_dsm <= 2'd0;
      done_last <= 512'd0;
    end else begin
      case(fsm_update_dsm)
        2'd0: begin
          if(update_dsm) begin
            afu_dsm_update <= 1'b1;
            done_last <= done;
            fsm_update_dsm <= 2'd1;
          end 
        end
        2'd1: begin
          if(afu_dsm_req_rd && (afu_dsm_addr == NUM_CL_DSM_TOTAL - 1)) begin
            fsm_update_dsm <= 2'd0;
            afu_dsm_update <= 1'b0;
          end 
        end
        default: begin
        end
      endcase
    end
  end


endmodule
