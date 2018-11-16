
module dispath_data #
(
  parameter INPUT_DATA_WIDTH = 16,
  parameter OUTPUT_DATA_WIDTH = 512
)
(
  input clk,
  input en,
  input rst,
  input available_write,
  output reg request_write,
  output reg [OUTPUT_DATA_WIDTH-1:0] write_data,
  input push_data,
  output reg available_push,
  input [INPUT_DATA_WIDTH-1:0] data_in
);

  localparam NUM = OUTPUT_DATA_WIDTH / INPUT_DATA_WIDTH;

  reg [2-1:0] fsm_control;
  reg [OUTPUT_DATA_WIDTH-1:0] buffer1;
  reg [OUTPUT_DATA_WIDTH-1:0] buffer2;
  reg [NUM-1:0] count1;
  reg [NUM-1:0] count2;
  reg request_write1;
  reg request_write2;
  reg request_write11;
  reg request_write22;

  always @(posedge clk) begin
    if(rst) begin
      request_write <= 0;
      request_write11 <= 0;
      request_write22 <= 0;
    end else begin
      request_write11 <= request_write1;
      request_write22 <= request_write2;
      request_write <= request_write11 | request_write22;
      if(en) begin
        if(request_write11) begin
          write_data <= buffer1;
        end else if(request_write22) begin
          write_data <= buffer2;
        end 
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      available_push <= 1;
      fsm_control <= 0;
      count1 <= 1;
      count2 <= 1;
      request_write1 <= 0;
      request_write2 <= 0;
    end else begin
      request_write1 <= 0;
      request_write2 <= 0;
      if(en) begin
        case(fsm_control)
          0: begin
            if(push_data) begin
              buffer1 <= { data_in, { OUTPUT_DATA_WIDTH - INPUT_DATA_WIDTH{ 1'b0 } } } | (buffer1 >> INPUT_DATA_WIDTH);
              count1 <= count1 << 1;
            end 
            if(count1[NUM - 1] & push_data) begin
              fsm_control <= 1;
            end 
          end
          1: begin
            if(available_write) begin
              count1 <= 1;
              request_write1 <= 1;
              available_push <= 1;
            end 
            if(available_write & available_push) begin
              fsm_control <= 2;
            end 
            if(available_write & ~available_push) begin
              fsm_control <= 3;
            end 
            if(push_data) begin
              buffer2 <= { data_in, { OUTPUT_DATA_WIDTH - INPUT_DATA_WIDTH{ 1'b0 } } } | (buffer2 >> INPUT_DATA_WIDTH);
              count2 <= count2 << 1;
            end 
            if(count2[NUM - 2] & push_data & ~available_write) begin
              available_push <= 0;
            end 
          end
          2: begin
            if(push_data) begin
              buffer2 <= { data_in, { OUTPUT_DATA_WIDTH - INPUT_DATA_WIDTH{ 1'b0 } } } | (buffer2 >> INPUT_DATA_WIDTH);
              count2 <= count2 << 1;
            end 
            if(count2[NUM - 1] & push_data) begin
              fsm_control <= 3;
            end 
          end
          3: begin
            if(available_write) begin
              request_write2 <= 1;
              available_push <= 1;
              count2 <= 1;
            end 
            if(available_write & available_push) begin
              fsm_control <= 0;
            end 
            if(available_write & ~available_push) begin
              fsm_control <= 1;
            end 
            if(push_data) begin
              buffer1 <= { data_in, { OUTPUT_DATA_WIDTH - INPUT_DATA_WIDTH{ 1'b0 } } } | (buffer1 >> INPUT_DATA_WIDTH);
              count1 <= count1 << 1;
            end 
            if(count1[NUM - 2] & push_data & ~available_write) begin
              available_push <= 0;
            end 
          end
        endcase
      end 
    end
  end


endmodule
