module input_queue_controller #
(
  parameter ID_QUEUE = 0,
  parameter ADDR_WIDTH = 64,
  parameter QTD_WIDTH = 32,
  parameter DATA_WIDTH = 512,
  parameter CONF_ID_QUEUE_WIDTH = 32,
  parameter TAG_WIDTH = 16
)
(
  input clk,
  input rst,
  input start,
  input [2-1:0] conf_valid,
  input [ADDR_WIDTH+QTD_WIDTH+CONF_ID_QUEUE_WIDTH-1:0] conf,
  input available_read,
  output reg has_rd_peding,
  output reg request_read,
  output reg [ADDR_WIDTH+TAG_WIDTH-1:0] request_data,
  input read_data_valid,
  input [TAG_WIDTH-1:0] read_queue_id,
  input [DATA_WIDTH-1:0] read_data,
  output acc_user_available_read,
  input acc_user_request_read,
  output reg [DATA_WIDTH-1:0] acc_user_read_data,
  output reg acc_user_read_data_valid,
  output reg done
);

  localparam FIFO_DEPTH_BITS = 10;
  localparam FIFO_FULL = 2 ** FIFO_DEPTH_BITS;
  localparam CONF_TYPE_IN = 1;
  reg conf_ready;
  reg [ADDR_WIDTH-1:0] addr_base;
  reg [ADDR_WIDTH-1:0] addr_read_next;
  reg [QTD_WIDTH-1:0] qtd_data_cl;
  reg [QTD_WIDTH-1:0] count_req_cl;
  reg [QTD_WIDTH-1:0] count_cl;
  reg [FIFO_DEPTH_BITS+1-1:0] read_peding;
  reg [FIFO_DEPTH_BITS+1-1:0] read_peding_1;
  reg flag_addr_init;
  reg fifo_we;
  reg [DATA_WIDTH-1:0] din;
  wire fifo_fit;
  reg issue_req_data;
  reg issue_req_data_next;
  wire fifo_empty;
  wire [FIFO_DEPTH_BITS+1-1:0] fifo_count;
  wire read_data_valid_queue;
  wire fifo_full;
  wire fifo_almostfull;
  wire fifo_almostempty;
  wire end_req_rd_data;
  wire end_req_rd_data_next;
  wire conf_rd_valid;
  wire [((ADDR_WIDTH+QTD_WIDTH)+CONF_ID_QUEUE_WIDTH)-1:0] conf_rd;
  wire rst_internal;
  wire [DATA_WIDTH-1:0] acc_user_read_data_out;
  wire acc_user_read_data_valid_out;

  conf_receiver
  #(
    .CONF_TYPE(CONF_TYPE_IN),
    .CONF_ID(ID_QUEUE),
    .CONF_ID_WIDTH(CONF_ID_QUEUE_WIDTH),
    .CONF_WIDTH(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH)
  )
  conf_receiver
  (
    .clk(clk),
    .rst(rst),
    .conf_in_valid(conf_valid),
    .conf_in_data(conf),
    .conf_out_valid(conf_rd_valid),
    .conf_out_data(conf_rd),
    .conf_reset_out(rst_internal)
  );


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
    .rst(rst_internal),
    .we(fifo_we),
    .din(din),
    .re(acc_user_request_read),
    .valid(acc_user_read_data_valid_out),
    .dout(acc_user_read_data_out),
    .count(fifo_count),
    .empty(fifo_empty),
    .full(fifo_full),
    .almostfull(fifo_almostfull),
    .almostempty(fifo_almostempty)
  );

  assign end_req_rd_data = count_req_cl < qtd_data_cl;
  assign end_req_rd_data_next = count_req_cl + 12 < qtd_data_cl;
  assign acc_user_available_read = (fifo_almostempty)? ~fifo_empty & ~acc_user_request_read : 1'b1;
  assign fifo_fit = read_peding < FIFO_FULL - 12;
  assign read_data_valid_queue = read_data_valid && (read_queue_id == ID_QUEUE);

  always @(posedge clk) begin
    if(rst_internal) begin
      done <= 1'b0;
      acc_user_read_data <= 0;
      acc_user_read_data_valid <= 1'b0;
    end else begin
      done <= (count_cl >= qtd_data_cl) && start;
      acc_user_read_data <= acc_user_read_data_out;
      acc_user_read_data_valid <= acc_user_read_data_valid_out;
    end
  end


  always @(posedge clk) begin
    if(rst_internal) begin
      has_rd_peding <= 1'b0;
    end else begin
      has_rd_peding <= (read_peding_1 > 0)? 1'b1 : 1'b0;
    end
  end


  always @(posedge clk) begin
    if(rst_internal) begin
      addr_base <= 0;
      qtd_data_cl <= 0;
      conf_ready <= 1'b0;
    end else begin
      if(conf_rd_valid) begin
        qtd_data_cl <= conf_rd[CONF_ID_QUEUE_WIDTH+QTD_WIDTH-1:CONF_ID_QUEUE_WIDTH];
        addr_base <= conf_rd[CONF_ID_QUEUE_WIDTH+QTD_WIDTH+ADDR_WIDTH-1:CONF_ID_QUEUE_WIDTH+QTD_WIDTH];
        conf_ready <= 1'b1;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst_internal) begin
      addr_read_next <= 0;
      flag_addr_init <= 1'b1;
    end else begin
      if(conf_ready & flag_addr_init) begin
        addr_read_next <= addr_base;
        flag_addr_init <= 1'b0;
      end else if(issue_req_data) begin
        addr_read_next <= addr_read_next + 4;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst_internal) begin
      request_read <= 1'b0;
      request_data <= 0;
      count_req_cl <= 0;
      issue_req_data <= 1'b0;
      issue_req_data_next <= 1'b0;
    end else begin
      request_read <= 1'b0;
      issue_req_data <= start & conf_ready & end_req_rd_data & available_read & fifo_fit;
      issue_req_data_next <= start & conf_ready & end_req_rd_data_next & available_read & fifo_fit;
      if(issue_req_data) begin
        request_read <= 1'b1;
        request_data <= { addr_read_next, ID_QUEUE[TAG_WIDTH-1:0] };
        count_req_cl <= count_req_cl + 4;
        issue_req_data <= issue_req_data_next;
      end 
    end
  end


  always @(posedge clk) begin
    if(rst_internal) begin
      read_peding <= 0;
      read_peding_1 <= 0;
    end else begin
      case({ acc_user_request_read, request_read })
        2'd0: begin
          read_peding <= read_peding;
        end
        2'd1: begin
          read_peding <= read_peding + 4;
        end
        2'd2: begin
          read_peding <= read_peding - 1;
        end
        2'd3: begin
          read_peding <= read_peding + 3;
        end
      endcase
      case({ read_data_valid_queue, request_read })
        2'd0: begin
          read_peding_1 <= read_peding_1;
        end
        2'd1: begin
          read_peding_1 <= read_peding_1 + 4;
        end
        2'd2: begin
          read_peding_1 <= read_peding_1 - 1;
        end
        2'd3: begin
          read_peding_1 <= read_peding_1 + 3;
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst_internal) begin
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