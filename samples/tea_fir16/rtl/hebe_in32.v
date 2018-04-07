
module hebe_in32
(
  input clk,
  input rst,
  input start,
  input [32-1:0] num_data,
  input [32-1:0] num_conf,
  input rdy,
  input available_read,
  input [512-1:0] rd_data,
  output reg req_rd_data,
  output reg [32-1:0] dconf,
  output en,
  output rout1,
  output rout2,
  output rout3,
  output rout4,
  output rout5,
  output rout6,
  output rout7,
  output rout8,
  output rout9,
  output rout10,
  output rout11,
  output rout12,
  output rout13,
  output rout14,
  output rout15,
  output rout16,
  output rout17,
  output rout18,
  output rout19,
  output rout20,
  output rout21,
  output rout22,
  output rout23,
  output rout24,
  output rout25,
  output rout26,
  output rout27,
  output rout28,
  output rout29,
  output rout30,
  output rout31,
  output rout32,
  output [16-1:0] dout1,
  output [16-1:0] dout2,
  output [16-1:0] dout3,
  output [16-1:0] dout4,
  output [16-1:0] dout5,
  output [16-1:0] dout6,
  output [16-1:0] dout7,
  output [16-1:0] dout8,
  output [16-1:0] dout9,
  output [16-1:0] dout10,
  output [16-1:0] dout11,
  output [16-1:0] dout12,
  output [16-1:0] dout13,
  output [16-1:0] dout14,
  output [16-1:0] dout15,
  output [16-1:0] dout16,
  output [16-1:0] dout17,
  output [16-1:0] dout18,
  output [16-1:0] dout19,
  output [16-1:0] dout20,
  output [16-1:0] dout21,
  output [16-1:0] dout22,
  output [16-1:0] dout23,
  output [16-1:0] dout24,
  output [16-1:0] dout25,
  output [16-1:0] dout26,
  output [16-1:0] dout27,
  output [16-1:0] dout28,
  output [16-1:0] dout29,
  output [16-1:0] dout30,
  output [16-1:0] dout31,
  output [16-1:0] dout32
);

  reg [4-1:0] index_conf;
  reg reg_en;
  reg [512-1:0] data;
  reg [512-1:0] data_out;
  reg r_out;
  reg [32-1:0] cont_data;
  reg [32-1:0] cont_conf;
  reg [3-1:0] fms_cs;
  reg flag_cpy_data;
  wire [32-1:0] wconf [0:16-1];
  genvar j;

  generate for(j=0; j<16; j=j+1) begin : gen_2
    assign wconf[j] = data[j*32+32-1:j*32];
  end
  endgenerate

  assign rout1 = r_out;
  assign dout1 = data_out[15:0];
  assign rout2 = r_out;
  assign dout2 = data_out[31:16];
  assign rout3 = r_out;
  assign dout3 = data_out[47:32];
  assign rout4 = r_out;
  assign dout4 = data_out[63:48];
  assign rout5 = r_out;
  assign dout5 = data_out[79:64];
  assign rout6 = r_out;
  assign dout6 = data_out[95:80];
  assign rout7 = r_out;
  assign dout7 = data_out[111:96];
  assign rout8 = r_out;
  assign dout8 = data_out[127:112];
  assign rout9 = r_out;
  assign dout9 = data_out[143:128];
  assign rout10 = r_out;
  assign dout10 = data_out[159:144];
  assign rout11 = r_out;
  assign dout11 = data_out[175:160];
  assign rout12 = r_out;
  assign dout12 = data_out[191:176];
  assign rout13 = r_out;
  assign dout13 = data_out[207:192];
  assign rout14 = r_out;
  assign dout14 = data_out[223:208];
  assign rout15 = r_out;
  assign dout15 = data_out[239:224];
  assign rout16 = r_out;
  assign dout16 = data_out[255:240];
  assign rout17 = r_out;
  assign dout17 = data_out[271:256];
  assign rout18 = r_out;
  assign dout18 = data_out[287:272];
  assign rout19 = r_out;
  assign dout19 = data_out[303:288];
  assign rout20 = r_out;
  assign dout20 = data_out[319:304];
  assign rout21 = r_out;
  assign dout21 = data_out[335:320];
  assign rout22 = r_out;
  assign dout22 = data_out[351:336];
  assign rout23 = r_out;
  assign dout23 = data_out[367:352];
  assign rout24 = r_out;
  assign dout24 = data_out[383:368];
  assign rout25 = r_out;
  assign dout25 = data_out[399:384];
  assign rout26 = r_out;
  assign dout26 = data_out[415:400];
  assign rout27 = r_out;
  assign dout27 = data_out[431:416];
  assign rout28 = r_out;
  assign dout28 = data_out[447:432];
  assign rout29 = r_out;
  assign dout29 = data_out[463:448];
  assign rout30 = r_out;
  assign dout30 = data_out[479:464];
  assign rout31 = r_out;
  assign dout31 = data_out[495:480];
  assign rout32 = r_out;
  assign dout32 = data_out[511:496];
  assign en = rdy & reg_en;
  localparam [3-1:0] FSM_WAIT = 0;
  localparam [3-1:0] FSM_RD_DATA = 1;
  localparam [3-1:0] FSM_DONE = 2;

  always @(posedge clk) begin
    if(rst) begin
      req_rd_data <= 1'b0;
      dconf <= 32'd0;
      index_conf <= 4'd0;
      data <= 512'd0;
      data_out <= 512'd0;
      r_out <= 1'b0;
      cont_data <= 32'd0;
      cont_conf <= 32'd0;
      reg_en <= 1'b0;
      flag_cpy_data <= 1'b0;
      fms_cs <= FSM_WAIT;
    end else begin
      if(start) begin
        req_rd_data <= 1'b0;
        reg_en <= 1'b0;
        r_out <= 1'b0;
        case(fms_cs)
          FSM_WAIT: begin
            if(available_read) begin
              req_rd_data <= 1'b1;
              flag_cpy_data <= 1'b0;
              fms_cs <= FSM_RD_DATA;
            end else if(cont_data >= num_data) begin
              fms_cs <= FSM_DONE;
              reg_en <= 1'b1;
            end else begin
              fms_cs <= FSM_WAIT;
            end
          end
          FSM_RD_DATA: begin
            if(cont_conf < num_conf) begin
              if(index_conf < 15) begin
                if(flag_cpy_data == 1'b0) begin
                  data <= rd_data;
                  flag_cpy_data <= 1'b1;
                  dconf <= rd_data[31:0];
                  cont_conf <= cont_conf + 1;
                  index_conf <= index_conf + 1;
                end else begin
                  dconf <= wconf[index_conf];
                  cont_conf <= cont_conf + 1;
                  index_conf <= index_conf + 1;
                end
                fms_cs <= FSM_RD_DATA;
              end else begin
                dconf <= wconf[index_conf];
                cont_conf <= cont_conf + 1;
                index_conf <= 4'd0;
                if(available_read) begin
                  req_rd_data <= 1'b1;
                  flag_cpy_data <= 1'b0;
                  fms_cs <= FSM_RD_DATA;
                end else begin
                  fms_cs <= FSM_WAIT;
                end
              end
            end else if(cont_data < num_data) begin
              if(flag_cpy_data == 1'b0) begin
                data_out <= rd_data[511:0];
                r_out <= 1'b1;
                if(rdy) begin
                  reg_en <= 1'b1;
                  cont_data <= cont_data + 32;
                  flag_cpy_data <= 1'b1;
                end 
              end else begin
                reg_en <= 1'b0;
                fms_cs <= FSM_WAIT;
              end
            end else begin
              reg_en <= 1'b1;
              fms_cs <= FSM_DONE;
            end
          end
          FSM_DONE: begin
            reg_en <= 1'b1;
            fms_cs <= FSM_DONE;
          end
        endcase
      end 
    end
  end


endmodule
