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


  //MD5 controller:
  
  wire valid_out;
  wire [128-1:0] data_out;
  reg  [3-1:0] bit_shift;
  reg  [512-1:0] shift_reg_data_out;
  reg  valid_shift_reg_data_out;
  reg  [512-1:0] data_in;
  reg valid_in;
  
  reg [5-1:0]rst_pipe;
  integer i;
  always @(posedge clk)begin
      rst_pipe[0] <= rst;
      for(i=1;i < 5;i=i+1)begin
        rst_pipe[i] <= rst_pipe[i-1]; 
      end 
  end 
  
  always @(posedge clk) begin
    if(rst_pipe[4]) begin
      acc_user_request_read <= 0;
      acc_user_request_write <= 0;
      acc_user_write_data <= 0;
      acc_user_done <= 0;
      data_in <= 512'd0;
      valid_in <= 0;
    end else begin
      if(start) begin
        acc_user_done <= acc_user_done_rd_data & acc_user_done_wr_data;
        acc_user_request_read <= acc_user_available_write & acc_user_available_read;
        acc_user_write_data <= shift_reg_data_out;
        acc_user_request_write <= valid_shift_reg_data_out;        
        valid_in <= acc_user_read_data_valid;
        data_in <= acc_user_read_data;
      end 
    end
  end
  
    always @(posedge clk) begin
    if(rst_pipe[4]) begin
       shift_reg_data_out <= 512'd0;
       bit_shift <= 0;
       valid_shift_reg_data_out <=0;
    end else begin
      if(start) begin
          valid_shift_reg_data_out <=0;
          case(bit_shift)
            0:begin
                if(valid_out)begin
                    shift_reg_data_out[128-1:0] <= data_out;
                    bit_shift <= 1;
                end 
            end 
            1:begin
                if(valid_out)begin
                    shift_reg_data_out[256-1:128] <= data_out;
                    bit_shift <= 2;
                end 
            end 
            2:begin
                if(valid_out)begin
                    shift_reg_data_out[384-1:256] <= data_out;
                    bit_shift <= 3;
                end 
            end 
            3:begin
                if(valid_out)begin
                    shift_reg_data_out[512-1:384] <= data_out;
                    valid_shift_reg_data_out <=1; 
                    bit_shift <= 0;
                end 
            end 
          endcase 
      end 
    end
  end
  
  Md5Core uu_md5_core
  (
    .clk       (clk),
    .reset     (rst_pipe[4]),
    .wb        (data_in),
    .valid_in  (valid_in),
    .a0        ('h67452301), 
    .b0        ('hefcdab89), 
    .c0        ('h98badcfe), 
    .d0        ('h10325476),
    .a64       (data_out[31:0]),
    .b64       (data_out[63:32]),
    .c64       (data_out[95:64]),
    .d64       (data_out[127:96]),
    .valid_out (valid_out)
  );
    

endmodule
