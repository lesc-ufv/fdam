
module cgra0_program_counter
(
  input clk,
  input rst,
  input en,
  input [8-1:0] max,
  input [8-1:0] loop,
  output reg [8-1:0] pc,
  output reg pc_en
);

  reg [8-1:0] pc_next;
  reg pc_en_next;

  always @(posedge clk) begin
    if(rst) begin
      pc_next <= 0;
      pc <= 0;
      pc_en <= 0;
      pc_en_next <= 0;
    end else begin
      if(en) begin
        pc_en_next <= 1;
        pc_en <= pc_en_next;
        if(pc_next < max) begin
          pc <= pc_next;
          pc_next <= pc_next + 1;
        end else begin
          pc <= pc_next;
          pc_next <= loop;
        end
      end else begin
        pc_en <= 0;
      end
    end
  end


endmodule
