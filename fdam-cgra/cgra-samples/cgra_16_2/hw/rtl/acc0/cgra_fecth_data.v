module cgra_fecth_data #
(
  parameter INPUT_DATA_WIDTH = 512,
  parameter OUTPUT_DATA_WIDTH = 16
)
(
  input clk,
  input en,
  input rst,
  input available_read,
  output reg request_read,
  input data_valid,
  input [INPUT_DATA_WIDTH-1:0] read_data,
  input pop_data,
  output reg available_pop,
  output [OUTPUT_DATA_WIDTH-1:0] data_out
);

  localparam NUM = INPUT_DATA_WIDTH / OUTPUT_DATA_WIDTH;
  reg [2-1:0] fsm_read;
  reg [2-1:0] fsm_control;
  reg [INPUT_DATA_WIDTH-1:0] data;
  reg [INPUT_DATA_WIDTH-1:0] buffer;
  reg [NUM-1:0] count;
  reg has_buffer;
  reg buffer_read;

  assign data_out = data[OUTPUT_DATA_WIDTH-1:0];

  always @(posedge clk) begin
    if(rst) begin
      fsm_read <= 0;
      request_read <= 0;
      has_buffer <= 0;
    end else begin
      request_read <= 0;
      if(en) begin
        case(fsm_read)
          0: begin
            if(available_read) begin
              request_read <= 1;
              fsm_read <= 1;
            end 
          end
          1: begin
            if(data_valid) begin
              buffer <= read_data;
              has_buffer <= 1;
              fsm_read <= 2;
            end 
          end
          2: begin
            if(buffer_read) begin
              has_buffer <= 0;
              fsm_read <= 0;
            end 
          end
        endcase
      end 
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      fsm_control <= 0;
      available_pop <= 0;
      count <= 0;
      buffer_read <= 0;
    end else begin
      buffer_read <= 0;
      if(en) begin
        case(fsm_control)
          0: begin
            if(has_buffer) begin
              data <= buffer;
              count <= 1;
              buffer_read <= 1;
              available_pop <= 1;
              fsm_control <= 1;
            end 
          end
          1: begin
            if(pop_data & ~count[NUM - 1]) begin
              count <= count << 1;
              data <= data[511:OUTPUT_DATA_WIDTH];
            end 
            if(pop_data & count[NUM - 1] & has_buffer) begin
              count <= 1;
              data <= buffer;
              buffer_read <= 1;
            end 
            if(count[NUM - 1] & pop_data & ~has_buffer) begin
              count <= count << 1;
              data <= data[510:OUTPUT_DATA_WIDTH];
              available_pop <= 0;
              fsm_control <= 0;
            end 
          end
        endcase
      end 
    end
  end

endmodule