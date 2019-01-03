module cgra0_conf_reader_pe #
(
  parameter PE_ID = 0
)
(
  input clk,
  input rst,
  input [64-1:0] conf_bus_in,
  output reg instruction_we,
  output reg [8-1:0] instruction_addr,
  output reg [16-1:0] instruction_data,
  output reg const_we,
  output reg [4-1:0] const_waddr,
  output reg [16-1:0] const_data,
  output reg [8-1:0] pc_max,
  output reg pc_max_we,
  output reg [8-1:0] pc_loop,
  output reg pc_loop_we,
  output reg [8-1:0] ignore_data,
  output reg ignore_we,
  output reg [3-1:0] thread_id
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
  reg [8-1:0] pe_conf_type;
  reg [16-1:0] pe_id;
  reg [4-1:0] pe_thread_id;
  reg [12-1:0] pe_inst_addr;
  reg [16-1:0] pe_instruction;
  reg [4-1:0] pe_const_addr;
  reg [32-1:0] pe_pc_max;
  reg [32-1:0] pe_pc_loop;
  reg [32-1:0] pe_ignore;
  reg [32-1:0] pe_constant;


  initial begin
    thread_id = 0;
    instruction_we = 0;
    instruction_addr = 0;
    instruction_data = 0;
    const_we = 0;
    const_waddr = 0;
    const_data = 0;
    pc_max = 0;
    pc_max_we = 0;
    pc_loop = 0;
    pc_loop_we = 0;
    ignore_data = 0;
    ignore_we = 0;
    pe_conf_type = 0;
    pe_id = 0;
    pe_thread_id = 0;
    pe_inst_addr = 0;
    pe_instruction = 0;
    pe_const_addr = 0;
    pe_constant = 0;
    pe_pc_max = 0;
    pe_pc_loop = 0;
    pe_ignore = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      pe_id <= 0;

    end else begin
      pe_conf_type <= conf_bus_in[7:0];
      pe_id <= conf_bus_in[23:8];
      pe_thread_id <= conf_bus_in[27:24];
      pe_inst_addr <= conf_bus_in[39:28];
      pe_instruction <= conf_bus_in[55:40];
      pe_const_addr <= conf_bus_in[31:28];
      pe_constant[31:0] <= conf_bus_in[63:32];
      pe_pc_max <= conf_bus_in[63:32];
      pe_pc_loop <= conf_bus_in[63:32];
      pe_ignore <= conf_bus_in[63:32];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      instruction_we <= 1'b0;
      const_we <= 1'b0;
      pc_loop_we <= 1'b0;
      pc_max_we <= 1'b0;
      ignore_we <= 1'b0;
    end else begin
      instruction_we <= 1'b0;
      const_we <= 1'b0;
      pc_loop_we <= 1'b0;
      pc_max_we <= 1'b0;
      ignore_we <= 1'b0;
      if(pe_id == PE_ID) begin
        case(pe_conf_type)
          CGRA_NOT_CONF: begin
          end
          CGRA_CONF_SET_PE_INSTRUCTION: begin
            instruction_we <= 1'b1;
          end
          CGRA_CONF_SET_PE_CONST: begin
            const_we <= 1'b1;
          end
          CGRA_CONF_SET_PE_PC_MAX: begin
            pc_max_we <= 1'b1;
          end
          CGRA_CONF_SET_PE_PC_LOOP: begin
            pc_loop_we <= 1'b1;
          end
          CGRA_CONF_SET_PE_STORE_IGNORE: begin
            ignore_we <= 1'b1;
          end
          CGRA_CONF_SET_NET_PC_MAX: begin
          end
          CGRA_CONF_SET_NET_PC_LOOP: begin
          end
          CGRA_CONF_NET_SWITCH: begin
          end
          default: begin
          end
        endcase
      end 
    end
  end


  always @(posedge clk) begin
    thread_id <= pe_thread_id[3:0];
    instruction_addr <= pe_inst_addr[7:0];
    instruction_data <= pe_instruction[15:0];
    thread_id <= pe_thread_id[3:0];
    const_waddr <= pe_const_addr[3:0];
    const_data <= pe_constant[15:0];
    pc_max <= pe_pc_max[7:0];
    pc_loop <= pe_pc_loop[7:0];
    ignore_data <= pe_ignore[7:0];
  end

endmodule