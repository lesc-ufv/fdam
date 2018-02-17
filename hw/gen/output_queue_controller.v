
module output_queue_controller #
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
  input available_write,
  output reg request_write,
  output reg [DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH-1:0] write_data,
  input write_data_valid,
  input [TAG_WIDTH-1:0] write_queue_id,
  output afu_user_available_write,
  input afu_user_request_write,
  input [DATA_WIDTH-1:0] afu_user_write_data,
  output done
);

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
  wire issue_req_data;
  wire fifo_empty;
  wire [FIFO_DEPTH_BITS+1-1:0] fifo_count;
  wire fifo_full;
  wire fifo_almostfull;
  wire fifo_almostempty;
  wire write_data_valid_queue;
  wire fifo_dout_valid;
  wire [DATA_WIDTH-1:0] fifo_dout;
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

  assign end_req_rd_data = count_req_cl >= qtd_data_cl;
  assign done = end_req_rd_data && (write_peding == 0) && start;
  assign issue_req_data = start & conf_ready & ~end_req_rd_data & available_write && ~fifo_empty && ~fifo_re;
  assign afu_user_available_write = ~fifo_full & ~afu_user_request_write;
  assign write_data_valid_queue = write_data_valid && (write_queue_id == ID_QUEUE);

  always @(posedge clk) begin
    if(rst) begin
      addr_base <= 0;
      qtd_data_cl <= 0;
      conf_reg <= 0;
      conf_read <= 1'b0;
      conf_ready <= 1'b0;
    end else begin
      if(conf_valid == 2'd2) begin
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
      addr_write_next <= 0;
      count_req_cl <= 0;
      flag_addr_init <= 1'b1;
    end else begin
      if(conf_ready & flag_addr_init) begin
        addr_write_next <= addr_base;
        flag_addr_init <= 1'b0;
      end else if(request_write) begin
        addr_write_next <= addr_write_next + 1;
        count_req_cl <= count_req_cl + 1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      fifo_re <= 1'b0;
    end else begin
      fifo_re <= 1'b0;
      if(issue_req_data) begin
        fifo_re <= 1'b1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      write_peding <= 0;
    end else begin
      case({ write_data_valid_queue, request_write })
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
      write_data <= 0;
    end else begin
      request_write <= 1'b0;
      if(fifo_dout_valid) begin
        request_write <= 1'b1;
        write_data <= { fifo_dout, addr_write_next, ID_QUEUE[TAG_WIDTH-1:0] };
      end 
    end
  end


endmodule
