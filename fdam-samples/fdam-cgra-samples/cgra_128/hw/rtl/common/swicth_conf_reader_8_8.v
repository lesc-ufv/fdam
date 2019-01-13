module swicth_conf_reader_8_8 #
(
  parameter SWICTH_NUMBER = 1
)
(
  input clk,
  input rst,
  input [64-1:0] conf_bus_in,
  output reg [8-1:0] pc_max,
  output reg pc_max_we,
  output reg [8-1:0] pc_loop,
  output reg pc_loop_we,
  output reg [3-1:0] thread_id,
  output reg net_mem_we,
  output reg [8-1:0] net_mem_waddr,
  output reg [8-1:0] net_mem_data
);

  localparam CGRA_CONF_SET_NET_PC_MAX = 8;
  localparam CGRA_CONF_SET_NET_PC_LOOP = 9;
  localparam CGRA_CONF_NET_SWITCH = 10;
  reg [8-1:0] net_conf_type;
  reg [16-1:0] net_swicth_number;
  reg [12-1:0] inst_addr;
  reg [24-1:0] net_swicth_conf;
  reg [32-1:0] net_pc_max;
  reg [32-1:0] net_pc_loop;
  reg [4-1:0] net_thread_id;

  initial begin
    net_conf_type = 0;
    net_swicth_number = 0;
    inst_addr = 0;
    net_swicth_conf = 0;
    net_pc_max = 0;
    net_pc_loop = 0;
    net_thread_id = 0;
    pc_max = 0;
    pc_loop = 0;
    thread_id = 0;
    net_mem_we = 0;
    net_mem_waddr = 0;
    net_mem_data = 0;
    net_thread_id = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      net_swicth_number <= 0;
    end else begin
      net_conf_type <= conf_bus_in[7:0];
      net_swicth_number <= conf_bus_in[23:8];
      net_thread_id <= conf_bus_in[27:24];
      inst_addr <= conf_bus_in[39:28];
      net_swicth_conf <= conf_bus_in[63:40];
      net_pc_max <= conf_bus_in[63:32];
      net_pc_loop <= conf_bus_in[63:32];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      pc_max_we <= 0;
      pc_loop_we <= 0;
      net_mem_we <= 0;
    end else begin
      net_mem_we <= 0;
      pc_max_we <= 0;
      pc_loop_we <= 0;
      if(net_swicth_number == SWICTH_NUMBER) begin
        case(net_conf_type)
          CGRA_CONF_SET_NET_PC_MAX: begin
            pc_max_we <= 1;
          end
          CGRA_CONF_SET_NET_PC_LOOP: begin
            pc_loop_we <= 1;
          end
          CGRA_CONF_NET_SWITCH: begin
            net_mem_we <= 1;
          end
          default: begin
          end
        endcase
      end 
    end
  end


  always @(posedge clk) begin
    thread_id <= net_thread_id[2:0];
    net_mem_waddr <= inst_addr[7:0];
    net_mem_data <= net_swicth_conf[7:0];
    pc_loop <= net_pc_loop[7:0];
    pc_max <= net_pc_max[7:0];
  end

endmodule