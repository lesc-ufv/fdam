module hebe_out32
(
  input clk,
  input rst,
  input start,
  input [32-1:0] num_data,
  input en,
  input rin1,
  input rin2,
  input rin3,
  input rin4,
  input rin5,
  input rin6,
  input rin7,
  input rin8,
  input rin9,
  input rin10,
  input rin11,
  input rin12,
  input rin13,
  input rin14,
  input rin15,
  input rin16,
  input rin17,
  input rin18,
  input rin19,
  input rin20,
  input rin21,
  input rin22,
  input rin23,
  input rin24,
  input rin25,
  input rin26,
  input rin27,
  input rin28,
  input rin29,
  input rin30,
  input rin31,
  input rin32,
  input [16-1:0] din1,
  input [16-1:0] din2,
  input [16-1:0] din3,
  input [16-1:0] din4,
  input [16-1:0] din5,
  input [16-1:0] din6,
  input [16-1:0] din7,
  input [16-1:0] din8,
  input [16-1:0] din9,
  input [16-1:0] din10,
  input [16-1:0] din11,
  input [16-1:0] din12,
  input [16-1:0] din13,
  input [16-1:0] din14,
  input [16-1:0] din15,
  input [16-1:0] din16,
  input [16-1:0] din17,
  input [16-1:0] din18,
  input [16-1:0] din19,
  input [16-1:0] din20,
  input [16-1:0] din21,
  input [16-1:0] din22,
  input [16-1:0] din23,
  input [16-1:0] din24,
  input [16-1:0] din25,
  input [16-1:0] din26,
  input [16-1:0] din27,
  input [16-1:0] din28,
  input [16-1:0] din29,
  input [16-1:0] din30,
  input [16-1:0] din31,
  input [16-1:0] din32,
  input available_write,
  output reg req_wr_data,
  output [512-1:0] wr_data,
  output reg [1-1:0] rdy,
  output reg done
);

  reg [2-1:0] index_data;
  reg [512-1:0] data [0:1-1];
  reg [32-1:0] cont_data;
  reg [3-1:0] fms_cs;
  wire wr_en;
  wire [512-1:0] wr_data_in;
  assign wr_en = en & rin1 & rin2 & rin3 & rin4 & rin5 & rin6 & rin7 & rin8 & rin9 & rin10 & rin11 & rin12 & rin13 & rin14 & rin15 & rin16 & rin17 & rin18 & rin19 & rin20 & rin21 & rin22 & rin23 & rin24 & rin25 & rin26 & rin27 & rin28 & rin29 & rin30 & rin31 & rin32;
  assign wr_data_in = { din32, din31, din30, din29, din28, din27, din26, din25, din24, din23, din22, din21, din20, din19, din18, din17, din16, din15, din14, din13, din12, din11, din10, din9, din8, din7, din6, din5, din4, din3, din2, din1 };
  genvar i;

  generate for(i=0; i<1; i=i+1) begin : gen_1
    assign wr_data[i*512+512-1:i*512] = data[i];
  end
  endgenerate

  localparam [3-1:0] FSM_WAIT = 0;
  localparam [3-1:0] FSM_WR_DATA = 1;
  localparam [3-1:0] FSM_DONE = 2;

  always @(posedge clk) begin
    if(rst) begin
      fms_cs <= FSM_WR_DATA;
      index_data <= 2'd0;
      req_wr_data <= 1'b0;
      done <= 1'b0;
      cont_data <= 32'd0;
      rdy <= 1'd1;
    end else begin
      if(start) begin
        req_wr_data <= 1'b0;
        case(fms_cs)
          FSM_WR_DATA: begin
            if(cont_data >= num_data) begin
              fms_cs <= FSM_DONE;
            end else if(wr_en) begin
              if(available_write) begin
                data[index_data] <= wr_data_in;
                req_wr_data <= 1'b1;
                index_data <= 2'd0;
              end else begin
                data[index_data] <= wr_data_in;
                fms_cs <= FSM_WAIT;
                rdy <= 1'd0;
              end
              cont_data <= cont_data + 32;
            end 
          end
          FSM_WAIT: begin
            if(available_write) begin
              rdy <= 1'd1;
              req_wr_data <= 1'b1;
              index_data <= 2'd0;
              fms_cs <= FSM_WR_DATA;
            end else begin
              rdy <= 1'd0;
            end
          end
          FSM_DONE: begin
            fms_cs <= FSM_DONE;
            done <= 1'b1;
          end
        endcase
      end 
    end
  end


endmodule
