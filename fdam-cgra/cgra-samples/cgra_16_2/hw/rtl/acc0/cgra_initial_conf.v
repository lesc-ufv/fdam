module cgra_initial_conf #
(
  parameter PE_ID = 0
)
(
  input clk,
  input rst,
  input [45-1:0] initial_conf_in,
  output reg [45-1:0] initial_conf_out,
  output reg init_conf_we,
  output reg [16-1:0] init_const,
  output reg [8-1:0] pc_max,
  output reg [8-1:0] pc_loop,
  output reg [8-1:0] ignore_until
);


  initial begin
    init_const = 0;
    pc_max = 0;
    pc_loop = 0;
    ignore_until = 0;
    init_conf_we = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      initial_conf_out <= 0;
      init_conf_we <= 0;
    end else begin
      initial_conf_out <= initial_conf_in;
      init_conf_we <= 0;
      if(initial_conf_out[4:0] == PE_ID) begin
        init_const <= initial_conf_out[20:5];
        pc_max <= initial_conf_out[28:21];
        pc_loop <= initial_conf_out[36:29];
        ignore_until <= initial_conf_out[44:37];
        init_conf_we <= 1;
      end 
    end
  end

endmodule