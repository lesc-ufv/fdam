module swicth_conf_reader_8_16 #
(
  parameter SWICTH_NUMBER = 1
)
(
  input clk,
  input rst,
  input [64-1:0] conf_bus_in,
  output reg [16-1:0] pc_max,
  output reg [16-1:0] pc_loop,
  output reg net_mem_we,
  output reg [16-1:0] net_mem_waddr,
  output reg [8-1:0] net_mem_data
);

  localparam CGRA_NOT_CONF = 0;
  localparam CGRA_CONF_SET_PE_INSTRUCTION = 1;
  localparam CGRA_CONF_SET_PE_CONST = 2;
  localparam CGRA_CONF_SET_PE_PC_MAX = 3;
  localparam CGRA_CONF_SET_PE_PC_LOOP = 4;
  localparam CGRA_CONF_SET_PE_STORE_IGNORE = 5;
  localparam CGRA_CONF_SET_NET_PC_MAX = 6;
  localparam CGRA_CONF_SET_NET_PC_LOOP = 7;
  localparam CGRA_CONF_NET_SWITCH = 8;
  reg [8-1:0] net_conf_type;
  reg [16-1:0] net_swicth_number;
  reg [16-1:0] inst_addr;
  reg [24-1:0] net_swicth_conf;
  reg [32-1:0] net_pc_max;
  reg [32-1:0] net_pc_loop;
  reg net_mem_we_p;
  reg [16-1:0] net_mem_waddr_p;
  reg [8-1:0] net_mem_data_p;

  initial begin
    net_conf_type = 0;
    net_swicth_number = 0;
    inst_addr = 0;
    net_swicth_conf = 0;
    net_pc_max = 0;
    net_pc_loop = 0;
    pc_max = 0;
    pc_loop = 0;
    net_mem_we = 0;
    net_mem_waddr = 0;
    net_mem_data = 0;
    net_mem_we_p = 0;
    net_mem_waddr_p = 0;
    net_mem_data_p = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      net_swicth_number <= 0;
    end else begin
      net_conf_type <= conf_bus_in[7:0];
      net_swicth_number <= conf_bus_in[23:8];
      inst_addr <= conf_bus_in[39:24];
      net_swicth_conf <= conf_bus_in[55:40];
      net_pc_max <= conf_bus_in[55:24];
      net_pc_loop <= conf_bus_in[55:24];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      pc_max <= 0;
      pc_loop <= 0;
      net_mem_we_p <= 0;
    end else begin
      net_mem_we_p <= 0;
      if(net_swicth_number == SWICTH_NUMBER) begin
        case(net_conf_type)
          CGRA_NOT_CONF: begin
          end
          CGRA_CONF_SET_PE_INSTRUCTION: begin
          end
          CGRA_CONF_SET_PE_CONST: begin
          end
          CGRA_CONF_SET_PE_PC_MAX: begin
          end
          CGRA_CONF_SET_PE_PC_LOOP: begin
          end
          CGRA_CONF_SET_PE_STORE_IGNORE: begin
          end
          CGRA_CONF_SET_NET_PC_MAX: begin
            pc_max <= net_pc_max[15:0];
          end
          CGRA_CONF_SET_NET_PC_LOOP: begin
            pc_loop <= net_pc_loop[15:0];
          end
          CGRA_CONF_NET_SWITCH: begin
            net_mem_we_p <= 1;
          end
          default: begin
          end
        endcase
      end 
    end
  end


  always @(posedge clk) begin
    net_mem_waddr_p <= inst_addr[15:0];
    net_mem_data_p <= net_swicth_conf[7:0];
    net_mem_we <= net_mem_we_p;
    net_mem_waddr <= net_mem_waddr_p;
    net_mem_data <= net_mem_data_p;
  end

endmodule