
module acc_user_3 #
(
  parameter DATA_WIDTH = 512,
  parameter NUM_INPUT_QUEUES = 1,
  parameter NUM_OUTPUT_QUEUES = 1
)
(
  input clk,
  input rst,
  input start,
  input [NUM_INPUT_QUEUES-1:0] acc_user_done_rd_data,
  input [NUM_OUTPUT_QUEUES-1:0] acc_user_done_wr_data,
  input [NUM_INPUT_QUEUES-1:0] acc_user_available_read,
  output reg [NUM_INPUT_QUEUES-1:0] acc_user_request_read,
  input [NUM_INPUT_QUEUES-1:0] acc_user_read_data_valid,
  input [DATA_WIDTH*NUM_INPUT_QUEUES-1:0] acc_user_read_data,
  input [NUM_OUTPUT_QUEUES-1:0] acc_user_available_write,
  output reg [NUM_OUTPUT_QUEUES-1:0] acc_user_request_write,
  output reg [DATA_WIDTH*NUM_OUTPUT_QUEUES-1:0] acc_user_write_data,
  output reg acc_user_done
);

  reg [512-1:0]context_add_ab;
  reg [2-1:0]fsm_read_context_add_ab;
  reg start_add_ab;
  
  wire available_read;
  wire [512-1:0]rd_data;
  
  wire [32-1:0]num_conf;
  wire [32-1:0]num_data_in;
  wire [32-1:0]num_data_out;
  
  wire req_rd_data;
 
  wire empty;
  wire almostempty;
  wire full;
  wire almostfull;

  assign num_conf = context_add_ab[31:0];
  assign num_data_in = context_add_ab[63:32];
  assign num_data_out = context_add_ab[95:64];
    
  assign available_read = almostempty?~empty&~req_rd_data:1'b1;
  
  always @(posedge clk)begin
     if(rst)begin
        context_add_ab <= 512'd0;
        acc_user_request_read <= 1'b0;
        fsm_read_context_add_ab <= 2'd0;
        start_add_ab <= 1'b0;
     end else begin          
        acc_user_request_read <= 1'b0; 
        start_add_ab <= 1'b0;   
        if(start) begin
           case(fsm_read_context_add_ab)
             2'd0:begin
                if(acc_user_available_read)begin
                    acc_user_request_read <= 1'b1;
                    fsm_read_context_add_ab <= 2'd1;
                end 
             end 
             2'd1:begin
                if(acc_user_read_data_valid)begin
                    context_add_ab <= acc_user_read_data;
                    fsm_read_context_add_ab <= 2'd2;
                end 
             end 
             2'd2:begin
                start_add_ab <= 1'b1;    
                acc_user_request_read <= acc_user_available_read & ~almostfull;
             end 
           endcase
        end
     end 
  end 
  
  sim_fifo #(
    .FIFO_WIDTH(DATA_WIDTH),
    .FIFO_DEPTH_BITS(10)
   )sim_fifo (
    .clk(clk),
    .reset_n(~rst),
    .we(acc_user_read_data_valid&start_add_ab),
    .din(acc_user_read_data),         
    .re(req_rd_data),           
    .dout(rd_data),            
    .count(),           
    .empty(empty),          
    .almostempty(almostempty),            
    .full(full),                             
    .almostfull(almostfull)       
   );
    
  fir1632
  fir1632
  (
    .clk(clk),
    .rst(rst),
    .start(start_add_ab),
    .num_conf(num_conf),
    .num_data_in(num_data_in),
    .num_data_out(num_data_out),
    .available_write(acc_user_available_write),
    .available_read(available_read),
    .req_rd_data(req_rd_data),
    .rd_data(rd_data),
    .req_wr_data(acc_user_request_write),
    .wr_data(acc_user_write_data),
    .done(acc_user_done)
  );
  

endmodule
