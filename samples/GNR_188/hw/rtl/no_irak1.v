
module no_irak1
(
  input clk,
  input start,
  input rst,
  input reset_nos,
  input start_s0,
  input start_s1,
  input init_state,
  input [1-1:0] il18r1_s0,
  input [1-1:0] il18r1_s1,
  output reg [1-1:0] s0,
  output reg [1-1:0] s1,
  output [1-1:0] irak1_s0,
  output [1-1:0] irak1_s1
);

  reg pass;

  always @(posedge clk) begin
    if(rst) begin
      s0 <= 1'd0;
      pass <= 1'b0;
    end else begin
      if(reset_nos) begin
        s0 <= init_state;
        pass <= 1;
      end else begin
        if(start_s0) begin
          if(pass) begin
            s0 <=  ( il18r1_s0 ) ;
            pass <= 0;
          end else begin
            pass <= 1;
          end
        end 
      end
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      s1 <= 1'd0;
    end else begin
      if(reset_nos) begin
        s1 <= init_state;
      end else begin
        if(start_s1) begin
          s1 <=  ( il18r1_s1 ) ;
        end 
      end
    end
  end

  assign irak1_s0 = s0;
  assign irak1_s1 = s1;

endmodule
