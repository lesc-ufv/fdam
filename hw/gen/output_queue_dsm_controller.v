
module output_queue_dsm_controller #
(
  parameter AFU_ID = 0,
  parameter CONF_AFU_ID_WIDTH = 32,
  parameter ID_QUEUE = 0,
  parameter ADDR_WIDTH = 64,
  parameter QTD_WIDTH = 32,
  parameter DATA_WIDTH = 512,
  parameter CONF_ID_QUEUE_WIDTH = 32,
  parameter TAG_WIDTH = 16,
  parameter FIFO_DEPTH_BITS = 4,
  parameter FIFO_FULL = 2 ** FIFO_DEPTH_BITS,
  parameter DSM_ADDR_WIDTH = 2,
  parameter DSM_DATA_WIDTH = 512,
  parameter DSM_NUM_CL = 3
)
(
  input clk,
  input rst,
  input start,
  input [2-1:0] conf_valid,
  input [ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0] conf,
  input available_write,
  output reg request_write,
  output reg [DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH-1:0] write_data,
  input write_data_valid,
  input [TAG_WIDTH-1:0] write_queue_id,
  output afu_user_available_write,
  input afu_user_request_write,
  input [DATA_WIDTH-1:0] afu_user_write_data,
  input afu_dsm_update,
  input afu_dsm_valid,
  input [DSM_DATA_WIDTH-1:0] afu_dsm_data,
  output reg afu_dsm_req_rd,
  output reg [DSM_ADDR_WIDTH-1:0] afu_dsm_addr,
  output done
);

  localparam TAG_DSM = 65535;
  reg conf_ready;
  reg conf_read;
  reg [((ADDR_WIDTH+QTD_WIDTH)+CONF_ID_QUEUE_WIDTH)-1:0] conf_reg;
  reg [ADDR_WIDTH-1:0] addr_base;
  reg [ADDR_WIDTH-1:0] addr_write_next;
  reg [QTD_WIDTH-1:0] qtd_data_cl;
  reg [QTD_WIDTH-1:0] count_req_cl;
  reg [QTD_WIDTH-1:0] write_peding;
  reg flag_addr_init;
  reg fifo_re;
  reg request_write_data;
  reg request_write_dsm;
  reg flag_addr_dsm_init;
  reg dsm_conf_read;
  reg dsm_conf_ready;
  reg [DSM_ADDR_WIDTH-1:0] count_req_dsm;
  reg [ADDR_WIDTH-1:0] dsm_addr_base;
  reg [ADDR_WIDTH-1:0] dsm_addr_next;
  reg [(ADDR_WIDTH+CONF_AFU_ID_WIDTH)-1:0] dsm_conf_reg;
  reg [DSM_ADDR_WIDTH-1:0] afu_dsm_addr_next;
  reg [ADDR_WIDTH+1-1:0] write_peding_dsm;
  wire issue_req_wr_data;
  wire fifo_empty;
  wire [FIFO_DEPTH_BITS+1-1:0] fifo_count;
  wire fifo_full;
  wire fifo_almostfull;
  wire fifo_almostempty;
  wire write_data_valid_queue;
  wire fifo_dout_valid;
  wire [DATA_WIDTH-1:0] fifo_dout;
  wire end_req_wr_data;
  wire write_data_valid_dsm;
  wire end_req_rd_dsm;
  wire issue_req_dsm;

  fifo
  #(
    .FIFO_WIDTH(DATA_WIDTH),
    .FIFO_DEPTH_BITS(FIFO_DEPTH_BITS),
    .FIFO_ALMOSTFULL_THRESHOLD(FIFO_FULL - 4),
    .FIFO_ALMOSTEMPTY_THRESHOLD(2)
  )
  fifo
  (
    .clk(clk),
    .rst(rst),
    .we(afu_user_request_write),
    .din(afu_user_write_data),
    .re(fifo_re),
    .valid(fifo_dout_valid),
    .dout(fifo_dout),
    .count(fifo_count),
    .empty(fifo_empty),
    .full(fifo_full),
    .almostfull(fifo_almostfull),
    .almostempty(fifo_almostempty)
  );

  assign end_req_wr_data = count_req_cl >= qtd_data_cl;
  assign done = end_req_wr_data && (write_peding == 0) && start;
  assign issue_req_wr_data = start & conf_ready & ~end_req_wr_data & available_write && ~fifo_empty && ~fifo_re;
  assign afu_user_available_write = ~fifo_full & ~afu_user_request_write;
  assign write_data_valid_queue = write_data_valid && (write_queue_id == ID_QUEUE);
  assign end_req_rd_dsm = count_req_dsm >= DSM_NUM_CL;
  assign issue_req_dsm = start && dsm_conf_ready && available_write && ~end_req_rd_dsm && afu_dsm_update;
  assign write_data_valid_dsm = write_data_valid && (write_queue_id == TAG_DSM);

  always @(posedge clk) begin
    if(rst) begin
      addr_base <= 0;
      qtd_data_cl <= 0;
      conf_reg <= 0;
      conf_read <= 1'b0;
      conf_ready <= 1'b0;
      dsm_addr_base <= 0;
      dsm_conf_reg <= 0;
      dsm_conf_read <= 1'b0;
      dsm_conf_ready <= 1'b0;
    end else begin
      case(conf_valid)
        2'd2: begin
          conf_reg <= conf;
          conf_read <= 1'b1;
        end
        2'd3: begin
          dsm_conf_reg <= conf[DSM_ADDR_WIDTH+CONF_AFU_ID_WIDTH-1:0];
          dsm_conf_read <= 1'b1;
        end
        default: begin
        end
      endcase
      if((conf_reg[CONF_ID_QUEUE_WIDTH-1:0] == ID_QUEUE) && conf_read) begin
        qtd_data_cl <= conf_reg[CONF_ID_QUEUE_WIDTH+QTD_WIDTH-1:CONF_ID_QUEUE_WIDTH];
        addr_base <= conf_reg[CONF_ID_QUEUE_WIDTH+QTD_WIDTH+ADDR_WIDTH-1:CONF_ID_QUEUE_WIDTH+QTD_WIDTH];
        conf_read <= 1'b0;
        conf_ready <= 1'b1;
      end 
      if((dsm_conf_reg[CONF_AFU_ID_WIDTH-1:0] == AFU_ID) && dsm_conf_read) begin
        dsm_addr_base <= conf_reg[DSM_ADDR_WIDTH+CONF_AFU_ID_WIDTH-1:CONF_AFU_ID_WIDTH];
        dsm_conf_read <= 1'b0;
        dsm_conf_ready <= 1'b1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      addr_write_next <= 0;
      count_req_cl <= 0;
      flag_addr_init <= 1'b1;
    end else begin
      if(conf_ready & flag_addr_init) begin
        addr_write_next <= addr_base;
        flag_addr_init <= 1'b0;
      end else if(request_write_data) begin
        addr_write_next <= addr_write_next + 1;
        count_req_cl <= count_req_cl + 1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      fifo_re <= 1'b0;
      afu_dsm_req_rd <= 1'b0;
    end else begin
      fifo_re <= 1'b0;
      afu_dsm_req_rd <= 1'b0;
      if(issue_req_wr_data) begin
        fifo_re <= 1'b1;
      end else if(issue_req_dsm) begin
        afu_dsm_req_rd <= 1'b1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      write_peding <= 0;
    end else begin
      case({ write_data_valid_queue, request_write_data })
        2'd0: begin
          write_peding <= write_peding;
        end
        2'd1: begin
          write_peding <= write_peding + 1;
        end
        2'd2: begin
          write_peding <= write_peding - 1;
        end
        2'd3: begin
          write_peding <= write_peding;
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      request_write <= 1'b0;
      request_write_data <= 1'b0;
      request_write_dsm <= 1'b0;
      write_data <= 0;
    end else begin
      request_write <= 1'b0;
      request_write_data <= 1'b0;
      request_write_dsm <= 1'b0;
      if(fifo_dout_valid) begin
        request_write <= 1'b1;
        request_write_data <= 1'b1;
        write_data <= { fifo_dout, addr_write_next, ID_QUEUE[TAG_WIDTH-1:0] };
      end else if(afu_dsm_valid) begin
        request_write <= 1'b1;
        request_write_dsm <= 1'b1;
        write_data <= { afu_dsm_data, dsm_addr_next, TAG_DSM[TAG_WIDTH-1:0] };
      end 
    end
  end


  always @(posedge clk) begin
    if(rst | ~afu_dsm_update) begin
      dsm_addr_next <= 0;
      count_req_dsm <= 0;
      flag_addr_dsm_init <= 1'b1;
    end else begin
      if(dsm_conf_ready & flag_addr_dsm_init) begin
        dsm_addr_next <= dsm_addr_base;
        flag_addr_dsm_init <= 1'b0;
      end else if(request_write_dsm) begin
        dsm_addr_next <= dsm_addr_next + 1;
        count_req_dsm <= count_req_dsm + 1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst | ~afu_dsm_update) begin
      afu_dsm_addr <= 0;
      afu_dsm_addr_next <= 0;
    end else begin
      if(afu_dsm_req_rd) begin
        afu_dsm_addr <= afu_dsm_addr_next;
        addr_write_next <= addr_write_next + 1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      write_peding_dsm <= 0;
    end else begin
      case({ write_data_valid_dsm, request_write_dsm })
        2'd0: begin
          write_peding_dsm <= write_peding_dsm;
        end
        2'd1: begin
          write_peding_dsm <= write_peding_dsm + 1;
        end
        2'd2: begin
          write_peding_dsm <= write_peding_dsm - 1;
        end
        2'd3: begin
          write_peding_dsm <= write_peding_dsm;
        end
      endcase
    end
  end


endmodule
