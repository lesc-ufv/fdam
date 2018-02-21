
module input_queue_controller #
(
  parameter ID_QUEUE = 0,
  parameter ADDR_WIDTH = 64,
  parameter QTD_WIDTH = 32,
  parameter DATA_WIDTH = 512,
  parameter CONF_ID_QUEUE_WIDTH = 32,
  parameter TAG_WIDTH = 16,
  parameter FIFO_DEPTH_BITS = 4,
  parameter FIFO_FULL = 2 ** FIFO_DEPTH_BITS
)
(
  input clk,
  input rst,
  input start,
  input [2-1:0] conf_valid,
  input [ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0] conf,
  input available_read,
  output reg has_peding,
  output reg request_read,
  output reg [ADDR_WIDTH+TAG_WIDTH-1:0] request_data,
  input read_data_valid,
  input [TAG_WIDTH-1:0] read_queue_id,
  input [DATA_WIDTH-1:0] read_data,
  output afu_user_available_read,
  input afu_user_request_read,
  output [DATA_WIDTH-1:0] afu_user_read_data,
  output afu_user_read_data_valid,
  output done
);

  reg conf_ready;
  reg conf_read;
  reg [((ADDR_WIDTH+QTD_WIDTH)+CONF_ID_QUEUE_WIDTH)-1:0] conf_reg;
  reg [ADDR_WIDTH-1:0] addr_base;
  reg [ADDR_WIDTH-1:0] addr_read_next;
  reg [QTD_WIDTH-1:0] qtd_data_cl;
  reg [QTD_WIDTH-1:0] count_req_cl;
  reg [QTD_WIDTH-1:0] count_cl;
  reg [QTD_WIDTH-1:0] read_peding;
  reg flag_addr_init;
  reg fifo_we;
  reg [DATA_WIDTH-1:0] din;
  wire fifo_fit;
  wire issue_req_data;
  wire fifo_empty;
  wire [FIFO_DEPTH_BITS+1-1:0] fifo_count;
  wire read_data_valid_queue;
  wire fifo_full;
  wire fifo_almostfull;
  wire fifo_almostempty;
  wire end_req_rd_data;

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
    .we(fifo_we),
    .din(din),
    .re(afu_user_request_read),
    .valid(afu_user_read_data_valid),
    .dout(afu_user_read_data),
    .count(fifo_count),
    .empty(fifo_empty),
    .full(fifo_full),
    .almostfull(fifo_almostfull),
    .almostempty(fifo_almostempty)
  );

  assign end_req_rd_data = count_req_cl >= qtd_data_cl;
  assign done = (count_cl >= qtd_data_cl) && (read_peding == 0) && start;
  assign issue_req_data = start & conf_ready & ~end_req_rd_data & available_read & fifo_fit & ~request_read;
  assign fifo_fit = (read_peding + fifo_count) < FIFO_FULL;
  assign afu_user_available_read = (fifo_almostempty)? ~fifo_empty & ~afu_user_request_read : 1'b1;
  assign read_data_valid_queue = read_data_valid && (read_queue_id == ID_QUEUE);

  always @(posedge clk) begin
    if(rst) begin
      has_peding <= 0;
    end else begin
      has_peding <= (read_peding > 0)? 1'b1 : 1'b0;
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      addr_base <= 0;
      qtd_data_cl <= 0;
      conf_reg <= 0;
      conf_read <= 1'b0;
      conf_ready <= 1'b0;
    end else begin
      if(conf_valid == 2'd1) begin
        conf_reg <= conf;
        conf_read <= 1'b1;
      end 
      if((conf_reg[CONF_ID_QUEUE_WIDTH-1:0] == ID_QUEUE) && conf_read) begin
        qtd_data_cl <= conf_reg[CONF_ID_QUEUE_WIDTH+QTD_WIDTH-1:CONF_ID_QUEUE_WIDTH];
        addr_base <= conf_reg[CONF_ID_QUEUE_WIDTH+QTD_WIDTH+ADDR_WIDTH-1:CONF_ID_QUEUE_WIDTH+QTD_WIDTH];
        conf_read <= 1'b0;
        conf_ready <= 1'b1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      addr_read_next <= 0;
      count_req_cl <= 0;
      flag_addr_init <= 1'b1;
    end else begin
      if(conf_ready & flag_addr_init) begin
        addr_read_next <= addr_base;
        flag_addr_init <= 1'b0;
      end else if(request_read) begin
        addr_read_next <= addr_read_next + 1;
        count_req_cl <= count_req_cl + 1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      request_read <= 1'b0;
      request_data <= 0;
    end else begin
      request_read <= 1'b0;
      if(issue_req_data) begin
        request_read <= 1'b1;
        request_data <= { addr_read_next, ID_QUEUE[TAG_WIDTH-1:0] };
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      read_peding <= 0;
    end else begin
      case({ fifo_we, request_read })
        2'd0: begin
          read_peding <= read_peding;
        end
        2'd1: begin
          read_peding <= read_peding + 1;
        end
        2'd2: begin
          read_peding <= read_peding - 1;
        end
        2'd3: begin
          read_peding <= read_peding;
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      fifo_we <= 1'b0;
      din <= 0;
      count_cl <= 0;
    end else begin
      fifo_we <= 1'b0;
      if(read_data_valid_queue) begin
        fifo_we <= 1'b1;
        din <= read_data;
        count_cl <= count_cl + 1;
      end 
    end
  end


endmodule
