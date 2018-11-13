
module cgra0_reg_pipe #
(
  parameter NUM_STAGES = 1,
  parameter DATA_WIDTH = 16
)
(
  input clk,
  input en,
  input [DATA_WIDTH-1:0] in,
  output [DATA_WIDTH-1:0] out
);

  reg [DATA_WIDTH-1:0] regs [0:NUM_STAGES-1];
  integer i;

  assign out = regs[NUM_STAGES - 1];

  always @(posedge clk) begin
    if(en) begin
      regs[0] <= in;
      for(i=1; i<NUM_STAGES; i=i+1) begin
        regs[i] <= regs[i - 1];
      end
    end 
  end


  initial begin
    for(i=0; i<NUM_STAGES; i=i+1) begin
      regs[i] = 0;
    end
  end


endmodule
