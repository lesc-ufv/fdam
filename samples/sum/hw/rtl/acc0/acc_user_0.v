module acc_user_0 #
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


  //Sample code for sum c[i] = a[i] + b[i]:
  
  reg [3-1:0]fsm;

  wire [512-1:0] a;
  wire [512-1:0] b;
  wire [512-1:0] c;
  
  assign a = acc_user_read_data[512-1:0];
  assign b = acc_user_read_data[1024-1:512];
  
  always @(posedge clk) begin
    if(rst) begin
      acc_user_request_read <= 0;
      acc_user_request_write <= 0;
      acc_user_write_data <= 0;
      acc_user_done <= 0;
      fsm <= 0;
    end else begin
      if(start) begin
          acc_user_done <= acc_user_done_rd_data & acc_user_done_wr_data;
          acc_user_request_read[0] <=  0;
          acc_user_request_read[1] <=  0;
          acc_user_request_write <= 0;
          case(fsm)
             0: begin
                if(acc_user_available_read[0] & acc_user_available_read[1])begin
                      acc_user_request_read[0] <=  1;
                      acc_user_request_read[1] <=  1;
                      fsm <= 1;
                end                 
             end 
             1:begin
                if(acc_user_read_data_valid[0]&acc_user_read_data_valid[1])begin
                   fsm <= 2;
                end 
             end 
             2:begin 
                acc_user_write_data <= c;
                acc_user_request_write <= 1;
                fsm <= 0;
             end 
         endcase
      end 
    end
  end
  
  genvar i;
    
  generate for(i = 0; i < 16;i = i+1)begin 
      sum sum(
        .clk(clk),
        .rst(rst),
        .a(a[((i+1)*32) -1:i*32]),
        .b(b[((i+1)*32) -1:i*32]),
        .c(c[((i+1)*32) -1:i*32])
      );
  end  
  
 endgenerate

endmodule



module sum(
  input clk,
  input rst,
  input [32-1:0]a,
  input [32-1:0]b,
  output reg [32-1:0]c

);

   always @(posedge clk)begin
      if(rst)begin
         c <= 0;
      end else begin
         c <= a + b;
      end 
   end 

endmodule

