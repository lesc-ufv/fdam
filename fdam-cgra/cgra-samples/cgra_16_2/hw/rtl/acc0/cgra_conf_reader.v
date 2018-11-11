module cgra_conf_reader #
(
  parameter PE_ID = 0
)
(
  input clk,
  input rst,
  input [21-1:0] conf_in,
  output reg [21-1:0] conf_out,
  output reg [8-1:0] conf_wr_addr,
  output reg [16-1:0] conf_wr_data,
  output reg conf_wr_en
);

  reg [8-1:0] conf_wr_addr_next;

  always @(posedge clk) begin
    if(rst) begin
      conf_wr_en <= 0;
      conf_wr_addr_next <= 0;
      conf_wr_addr <= 0;
    end else begin
      conf_out <= conf_in;
      conf_wr_en <= 0;
      if(conf_out[4:0] == PE_ID) begin
        conf_wr_addr <= conf_wr_addr_next;
        conf_wr_addr_next <= conf_wr_addr_next + 1;
        conf_wr_en <= 1;
        conf_wr_data <= conf_out[20:5];
      end 
    end
  end


  initial begin
    conf_wr_en = 0;
    conf_wr_addr_next = 0;
    conf_wr_addr = 0;
    conf_wr_data = 0;
  end

endmodule