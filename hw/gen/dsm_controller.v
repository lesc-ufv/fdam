
module dsm_controller #
(
  parameter QTD_WIDTH = 32,
  parameter DSM_DATA_WIDTH = 512,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1,
  parameter NUM_CL_DSM_RD = $rtoi($ceil(NUM_INPUT_QUEUES/16)),
  parameter NUM_CL_DSM_WR = $rtoi($ceil(NUM_OUTPUT_QUEUES/16)),
  parameter NUM_CL_DSM_TOTAL = 1 + NUM_CL_DSM_RD + NUM_CL_DSM_WR,
  parameter NUM_CL_DSM_TOTAL_BITS = $rtoi($ceil($clog2(NUM_CL_DSM_TOTAL)))
)
(
  input clk,
  input rst,
  input start,
  input [NUM_INPUT_QUEUES-1:0] done_rd,
  input [NUM_OUTPUT_QUEUES-1:0] done_wr,
  input done_afu,
  input has_pending_wr,
  input [NUM_INPUT_QUEUES-1:0] afu_req_rd_data_en,
  input [NUM_OUTPUT_QUEUES-1:0] afu_req_wr_data_en,
  input afu_dsm_available_write,
  output reg afu_dsm_request_write,
  output reg [DSM_DATA_WIDTH-1:0] afu_dsm_write_data
);

  reg [DSM_DATA_WIDTH-1:0] dsm_data [0:NUM_CL_DSM_TOTAL-1];
  reg [DSM_DATA_WIDTH-1:0] dsm_data_last;
  reg [DSM_DATA_WIDTH-1:0] done;
  reg [DSM_DATA_WIDTH-1:0] done_last;
  reg [2-1:0] fsm_update_dsm;
  reg [NUM_CL_DSM_TOTAL_BITS+1-1:0] afu_req_wr_count;
  wire update_dsm;
  wire [QTD_WIDTH-1:0] afu_in_rd_count [0:(NUM_CL_DSM_RD*16)-1];
  wire [QTD_WIDTH-1:0] afu_in_wr_count [0:(NUM_CL_DSM_WR*16)-1];
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


  generate for(i=(NUM_CL_DSM_RD*16)-1; i>=NUM_INPUT_QUEUES; i=i-1) begin : gen_zero_rd_count
    assign afu_in_rd_count[i] = { QTD_WIDTH{ 1'b0 } };
  end
  endgenerate


  generate for(i=(NUM_CL_DSM_WR*16)-1; i>=NUM_OUTPUT_QUEUES; i=i-1) begin : gen_zero_wr_count
    assign afu_in_wr_count[i] = { QTD_WIDTH{ 1'b0 } };
  end
  endgenerate


  generate for(i=0; i<NUM_CL_DSM_RD; i=i+1) begin : gen_dsm_data_rd
    assign dsm_data_wire[i] = { afu_in_rd_count[i * 16 + 15], afu_in_rd_count[i * 16 + 14], afu_in_rd_count[i * 16 + 13], afu_in_rd_count[i * 16 + 12], afu_in_rd_count[i * 16 + 11], afu_in_rd_count[i * 16 + 10], afu_in_rd_count[i * 16 + 9], afu_in_rd_count[i * 16 + 8], afu_in_rd_count[i * 16 + 7], afu_in_rd_count[i * 16 + 6], afu_in_rd_count[i * 16 + 5], afu_in_rd_count[i * 16 + 4], afu_in_rd_count[i * 16 + 3], afu_in_rd_count[i * 16 + 2], afu_in_rd_count[i * 16 + 1], afu_in_rd_count[i * 16 + 0] };
  end
  endgenerate


  generate for(i=0; i<NUM_CL_DSM_WR; i=i+1) begin : gen_dsm_data_wr
    assign dsm_data_wire[i + NUM_CL_DSM_WR] = { afu_in_wr_count[i * 16 + 15], afu_in_wr_count[i * 16 + 14], afu_in_wr_count[i * 16 + 13], afu_in_wr_count[i * 16 + 12], afu_in_wr_count[i * 16 + 11], afu_in_wr_count[i * 16 + 10], afu_in_wr_count[i * 16 + 9], afu_in_wr_count[i * 16 + 8], afu_in_wr_count[i * 16 + 7], afu_in_wr_count[i * 16 + 6], afu_in_wr_count[i * 16 + 5], afu_in_wr_count[i * 16 + 4], afu_in_wr_count[i * 16 + 3], afu_in_wr_count[i * 16 + 2], afu_in_wr_count[i * 16 + 1], afu_in_wr_count[i * 16 + 0] };
  end
  endgenerate

  assign dsm_data_wire[NUM_CL_DSM_RD + NUM_CL_DSM_WR] = done;
  assign update_dsm = |(done ^ done_last & done);

  always @(posedge clk) begin
    if(rst) begin
      done <= 512'd0;
    end else begin
      done <= { { DSM_DATA_WIDTH - (NUM_INPUT_QUEUES + NUM_OUTPUT_QUEUES + 1){ 1'b0 } }, done_wr, done_rd, done_afu };
    end
  end


  always @(posedge clk) begin
    for(idx=0; idx<NUM_CL_DSM_TOTAL; idx=idx+1) begin
      dsm_data[idx] <= dsm_data_wire[idx];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      afu_dsm_request_write <= 1'b0;
      fsm_update_dsm <= 2'd0;
      done_last <= 512'd0;
      afu_req_wr_count <= 0;
      afu_dsm_write_data <= 0;
      dsm_data_last <= 0;
    end else begin
      if(start) begin
        afu_dsm_request_write <= 1'b0;
        case(fsm_update_dsm)
          2'd0: begin
            if(update_dsm) begin
              done_last <= done;
              fsm_update_dsm <= 2'd1;
              afu_req_wr_count <= 0;
            end 
          end
          2'd1: begin
            if(afu_dsm_available_write) begin
              afu_dsm_request_write <= 1'b1;
              afu_dsm_write_data <= dsm_data[afu_req_wr_count];
              dsm_data_last <= dsm_data[NUM_CL_DSM_TOTAL - 1];
              afu_req_wr_count <= afu_req_wr_count + 1;
            end 
            if(afu_req_wr_count == NUM_CL_DSM_TOTAL - 2) begin
              fsm_update_dsm <= 2'd2;
            end 
          end
          2'd2: begin
            if(has_pending_wr) begin
              fsm_update_dsm <= 2'd3;
            end 
          end
          2'd3: begin
            if(!has_pending_wr) begin
              afu_dsm_request_write <= 1'b1;
              afu_dsm_write_data <= dsm_data_last;
              afu_req_wr_count <= afu_req_wr_count + 1;
              fsm_update_dsm <= 2'd0;
            end 
          end
        endcase
      end 
    end
  end


endmodule
