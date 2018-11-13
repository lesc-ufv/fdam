
module cgra0_conf_reader_pc_net #
(
  parameter PE_ID = 0
)
(
  input clk,
  input rst,
  input [60-1:0] conf_bus_in,
  output reg [8-1:0] pc_max,
  output reg [8-1:0] pc_loop
);

  localparam SET_PC_MAX = 2;
  localparam SET_LOOP = 3;
  reg [4-1:0] op_conf;
  reg [16-1:0] id_conf;
  reg [40-1:0] conf_data;

  initial begin
    pc_max = 0;
    pc_loop = 0;
    op_conf = 0;
    id_conf = 0;
    conf_data = 0;
  end


  always @(posedge clk) begin
    if(rst) begin
      op_conf <= 0;
      id_conf <= 0;
      conf_data <= 0;
    end else begin
      op_conf <= conf_bus_in[3:0];
      id_conf <= conf_bus_in[23:8];
      conf_data <= conf_bus_in[59:24];
    end
  end


  always @(posedge clk) begin
    if(rst) begin
      pc_max <= 0;
      pc_loop <= 0;
    end else begin
      if(PE_ID == id_conf) begin
        case(op_conf)
          SET_PC_MAX: begin
            pc_max <= conf_data[8:0];
          end
          SET_LOOP: begin
            pc_loop <= conf_data[8:0];
          end
          default: begin
          end
        endcase
      end 
    end
  end


endmodule
