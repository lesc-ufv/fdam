
module cgra0_conf_reader_pe #
(
  parameter PE_ID = 0
)
(
  input clk,
  input rst,
  input [60-1:0] conf_bus_in,
  output reg conf_wr_en,
  output reg [8-1:0] conf_wr_addr,
  output reg [16-1:0] conf_wr_data,
  output reg init_const_we,
  output reg [3-1:0] init_const_waddr,
  output reg [16-1:0] init_const,
  output reg [8-1:0] pc_max,
  output reg [8-1:0] pc_loop,
  output reg [8-1:0] ignore_until
);

  localparam SET_INSTRUCTION = 0;
  localparam SET_CONST = 1;
  localparam SET_PC_MAX = 2;
  localparam SET_LOOP = 3;
  localparam SET_IGNORE = 4;
  reg [4-1:0] op_conf;
  reg [16-1:0] id_conf;
  reg [40-1:0] conf_data;

  initial begin
    init_const_we = 0;
    init_const_waddr = 0;
    init_const = 0;
    pc_max = 0;
    pc_loop = 0;
    ignore_until = 0;
    op_conf = 0;
    id_conf = 0;
    conf_data = 0;
    conf_wr_en = 0;
    conf_wr_addr = 0;
    conf_wr_data = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      op_conf <= 0;
      id_conf <= 0;
      conf_data <= 0;
    end else begin
      op_conf <= conf_bus_in[3:0];
      id_conf <= conf_bus_in[19:4];
      conf_data <= conf_bus_in[59:20];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      conf_wr_en <= 0;
      init_const_we <= 0;
      pc_max <= 0;
      pc_loop <= 0;
      ignore_until <= 0;
    end else begin
      conf_wr_en <= 0;
      init_const_we <= 0;
      if(id_conf == PE_ID) begin
        case(op_conf)
          SET_INSTRUCTION: begin
            conf_wr_en <= 1;
            conf_wr_addr <= conf_data[7:0];
            conf_wr_data <= conf_data[39:16];
          end
          SET_CONST: begin
            init_const_we <= 1;
            init_const_waddr <= conf_data[34:32];
            init_const <= conf_data[15:0];
          end
          SET_PC_MAX: begin
            pc_max <= conf_data[8:0];
          end
          SET_LOOP: begin
            pc_loop <= conf_data[8:0];
          end
          SET_IGNORE: begin
            ignore_until <= conf_data[8:0];
          end
          default: begin
          end
        endcase
      end 
    end
  end


endmodule
