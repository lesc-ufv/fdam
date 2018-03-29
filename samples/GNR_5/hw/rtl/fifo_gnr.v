
module fifo_gnr #
(
  parameter FIFO_WIDTH = 32,
  parameter FIFO_DEPTH_BITS = 8,
  parameter FIFO_ALMOSTFULL_THRESHOLD = 2**FIFO_DEPTH_BITS - 4,
  parameter FIFO_ALMOSTEMPTY_THRESHOLD = 2
)
(
  input clk,
  input rst,
  input we,
  input [FIFO_WIDTH-1:0] din,
  input re,
  output reg [FIFO_WIDTH-1:0] dout,
  output reg empty,
  output reg almostempty,
  output reg full,
  output reg almostfull
);

  reg [FIFO_DEPTH_BITS-1:0] rp;
  reg [FIFO_DEPTH_BITS-1:0] wp;
  reg [FIFO_DEPTH_BITS-1:0] count;
  reg [FIFO_WIDTH-1:0] mem [0:2**FIFO_DEPTH_BITS-1];

  always @(posedge clk) begin
    if(rst) begin
      empty <= 1'b1;
      almostempty <= 1'b1;
      full <= 1'b0;
      almostfull <= 1'b0;
      rp <= 0;
      wp <= 0;
      count <= 0;
    end else begin
      case({ we, re })
        2'b11: begin
          rp <= rp + 1;
          wp <= wp + 1;
        end
        2'b10: begin
          if(~full) begin
            wp <= wp + 1;
            count <= count + 1;
            empty <= 1'b0;
            if(count == FIFO_ALMOSTEMPTY_THRESHOLD - 1) begin
              almostempty <= 1'b0;
            end 
            if(count == 2**FIFO_DEPTH_BITS-1) begin
              full <= 1'b1;
            end 
            if(count == FIFO_ALMOSTFULL_THRESHOLD - 1) begin
              almostfull <= 1'b1;
            end 
          end 
        end
        2'b1: begin
          if(~empty) begin
            rp <= rp + 1;
            count <= count - 1;
            full <= 1'b0;
            if(count == FIFO_ALMOSTFULL_THRESHOLD) begin
              almostfull <= 1'b0;
            end 
            if(count == 1) begin
              empty <= 1'b1;
            end 
            if(count == FIFO_ALMOSTEMPTY_THRESHOLD) begin
              almostempty <= 1'b1;
            end 
          end 
        end
        default: begin
        end
      endcase
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      dout <= 0;
    end else begin
      if(we == 1'b1) begin
        mem[wp] <= din;
      end 
      if(re == 1'b1) begin
        dout <= mem[rp];
      end 
    end
  end


endmodule
