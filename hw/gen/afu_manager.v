
module afu_manager
(
  input clk,
  input rst,
  input start,
  input [64-1:0] rst_afus,
  input [64-1:0] start_afus,
  input [7-1:0] rst_buffer_in_index,
  input [7-1:0] rst_buffer_out_index,
  input [64-1:0] workspace_addr_base,
  input [16-1:0] conf_size,
  input [16-1:0] dsm_size,
  input update_workspace,
  output reg req_rd_en,
  output reg [64-1:0] req_rd_addr,
  output reg [16-1:0] req_rd_mdata,
  input req_rd_available,
  input resp_rd_valid,
  input [512-1:0] resp_rd_data,
  input [16-1:0] resp_rd_mdata,
  input req_wr_available,
  output reg req_wr_en,
  output reg [64-1:0] req_wr_addr,
  output reg [16-1:0] req_wr_mdata,
  output reg [512-1:0] req_wr_data,
  input resp_wr_valid,
  input [16-1:0] resp_wr_mdata,
  output reg [576-1:0] info
);

  //Parâmetros locais:
  localparam FIFO_IN_FULL = 32;
  localparam TAG_WR_DSM = 16'hfffe;
  localparam TAG_RD_CONF = 16'hffff;
  //CONFs e DSM addr calc:
  wire [64-1:0] configurations_addr_base;
  wire [64-1:0] dsm_addr_base;
  assign configurations_addr_base = workspace_addr_base[63:6];
  assign dsm_addr_base = workspace_addr_base[63:6] + 4;

  //Flag to reset buffers:
  wire reset_buffers_in_flag;
  wire reset_buffers_out_flag;
  assign reset_buffers_in_flag = (rst_buffer_in_index != 7'b0)? 1'b1 : 1'b0;
  assign reset_buffers_out_flag = (rst_buffer_out_index != 7'b0)? 1'b1 : 1'b0;

  //Registrador para pipeline do dado a ser enfileirado nas filas de entrada
  reg [512-1:0] resp_rd_data_tmp;
  reg [2-1:0] reset_buffers_in;
  reg [2-1:0] reset_buffers_out;

  //Gerenciamento dos Done signals
  wire [2-1:0] done_read_buffers;
  wire [2-1:0] done_write_buffers;
  wire [2-1:0] done_uut_afu;
  wire [2-1:0] done_uut_afu_nopend;
  wire [512-1:0] done_reg_vet;
  reg [512-1:0] done_reg_vet_last;

  //controle da fsm de leitura de dados e configurações
  reg [2-1:0] fsm_rd;

  //controle da fsm de escrita de dados
  reg [2-1:0] fsm_wr;

  //registradores onde ficarão armazenados os endereços
  //as quantidades de dados para cada fila de entrada e para cada fila de saída
  //control for conf
  reg [512-1:0] configurations [0:4-1];
  reg [512-1:0] dsm [0:3-1];
  reg [3-1:0] counter_received_conf;
  reg [3-1:0] addr_offset_conf;

  //control for data_in
  reg [64-1:0] counter_received_data_in [0:8-1];
  reg [64-1:0] addr_offset_data_in [0:8-1];
  wire [64-1:0] qtde_data_in [0:8-1];
  wire [64-1:0] addr_in [0:8-1];

  // control for data_out
  reg [64-1:0] counter_sent_data_out [0:8-1];
  reg [64-1:0] addr_offset_data_out [0:8-1];
  wire [64-1:0] qtde_data_out [0:8-1];
  wire [64-1:0] addr_out [0:8-1];

  //criação das fifos, regs para as filas de entrada e saída
  wire [2-1:0] pending_writes_afus;
  reg [8-1:0] pending_writes_fifos_out [0:2-1];
  wire [2-1:0] fifos_in_read_request;
  wire [2-1:0] fifos_out_read_request;
  reg [2-1:0] we_fifo_in;
  reg [2-1:0] re_fifo_out;
  wire [512-1:0] dout_fifo_out [0:2-1];
  wire [2-1:0] almostempty_fifo_out;
  wire [5-1:0] count_fifo_out [0:2-1];
  genvar i;
  genvar j;

  //controle de DONE

  generate for(i=0; i<2; i=i+1) begin : gen_0
    assign done_read_buffers[i] = counter_received_data_in[i] >= qtde_data_in[i];
  end
  endgenerate


  generate for(i=0; i<2; i=i+1) begin : gen_1
    assign done_write_buffers[i] = counter_sent_data_out[i] >= qtde_data_out[i];
  end
  endgenerate


  generate for(i=0; i<2; i=i+1) begin : gen_pend
    assign done_uut_afu_nopend[i] = pending_writes_afus[i] && done_uut_afu[i];
  end
  endgenerate

  assign done_reg_vet[0] = &done_read_buffers && &done_write_buffers && &done_uut_afu;
  assign done_reg_vet[2:1] = done_uut_afu_nopend;
  assign done_reg_vet[4:3] = done_read_buffers;
  assign done_reg_vet[6:5] = done_write_buffers;
  assign done_reg_vet[511:7] = 505'd0;

  //Assigns para o acesso direto aos endereços de cada fila de entrada de dados
  //e para as quantidades de dados para leitura de cada fila

  generate for(i=0; i<1; i=i+1) begin : gen_2
    for(j=0; j<8; j=j+1) begin : gen_3
      assign addr_in[(i*8) + j] = configurations[i*2][(j * 64) + 64 -1 :j * 64];
      assign qtde_data_in[(i*8) + j] = configurations[(i*2)+1][(j * 64) + 64 -1:j * 64];
    end
  end
  endgenerate

  //Assigns para o acesso direto aos endereços de cada fila de saída de dados
  //e para as quantidades de dados de escrita de cada fila

  generate for(i=0; i<1; i=i+1) begin : gen_4
    for(j=0; j<8; j=j+1) begin : gen_5
      assign addr_out[(i*8) + j] = configurations[(i*2)+2][(j * 64) + 64 -1 :j * 64];
      assign qtde_data_out[(i*8) + j] = configurations[(i*2)+3][(j * 64) + 64 -1:j * 64];
    end
  end
  endgenerate

  //Criação das filas de entrada
  wire [2-1:0] re_fifo_in;
  wire [2-1:0] almostempty_fifo_in;
  wire [2-1:0] empty_fifo_in;
  wire [2-1:0] almostfull_fifo_in;
  wire [2-1:0] full_fifo_in;
  wire [5-1:0] count_fifo_in [0:2-1];
  wire [512-1:0] dout_fifo_in [0:2-1];

  generate for(i=0; i<2; i=i+1) begin : gen_6
    assign fifos_in_read_request[i] = addr_offset_data_in[i] - counter_received_data_in[i] + count_fifo_in[i] < FIFO_IN_FULL;
  end
  endgenerate


  generate for(i=0; i<2; i=i+1) begin : gen_7

    fifo
    #(
      .FIFO_WIDTH(512),
      .FIFO_DEPTH_BITS(5)
    )
    fifo_in
    (
      .clk(clk),
      .rst(rst | reset_buffers_in[i]),
      .we(we_fifo_in[i]),
      .din(resp_rd_data_tmp),
      .re(re_fifo_in[i]),
      .dout(dout_fifo_in[i]),
      .count(count_fifo_in[i]),
      .empty(empty_fifo_in[i]),
      .almostempty(almostempty_fifo_in[i]),
      .full(full_fifo_in[i]),
      .almostfull(almostfull_fifo_in[i])
    );

  end
  endgenerate

  //Criação das filas de saída
  wire [2-1:0] we_fifo_out;
  wire [2-1:0] empty_fifo_out;
  wire [2-1:0] almostfull_fifo_out;
  wire [2-1:0] full_fifo_out;
  wire [512-1:0] din_fifo_out [0:2-1];

  generate for(i=0; i<2; i=i+1) begin : gen_8
    assign fifos_out_read_request[i] = ~empty_fifo_out[i];
  end
  endgenerate


  generate for(i=0; i<2; i=i+1) begin : gen_9

    fifo
    #(
      .FIFO_WIDTH(512),
      .FIFO_DEPTH_BITS(5)
    )
    fifo_out
    (
      .clk(clk),
      .rst(rst | reset_buffers_out[i]),
      .we(we_fifo_out[i]),
      .din(din_fifo_out[i]),
      .re(re_fifo_out[i]),
      .dout(dout_fifo_out[i]),
      .count(count_fifo_out[i]),
      .empty(empty_fifo_out[i]),
      .almostempty(almostempty_fifo_out[i]),
      .full(full_fifo_out[i]),
      .almostfull(almostfull_fifo_out[i])
    );

  end
  endgenerate

  //Arbitro de leitura
  wire [2-1:0] grant_in;
  wire grant_in_valid;
  wire grant_in_index;

  arbiter
  #(
    .PORTS(2),
    .TYPE("ROUND_ROBIN"),
    .BLOCK("NONE"),
    .LSB_PRIORITY("LOW")
  )
  arbiter_in
  (
    .clk(clk),
    .rst(rst),
    .request(fifos_in_read_request),
    .acknowledge(2'd0),
    .grant(grant_in),
    .grant_valid(grant_in_valid),
    .grant_encoded(grant_in_index)
  );

  //Arbitro de escrita
  wire [2-1:0] grant_out;
  wire grant_out_valid;
  wire grant_out_index;
  reg grant_out_index_tmp;

  arbiter
  #(
    .PORTS(2),
    .TYPE("ROUND_ROBIN"),
    .BLOCK("NONE"),
    .LSB_PRIORITY("LOW")
  )
  arbiter_out
  (
    .clk(clk),
    .rst(rst),
    .request(fifos_out_read_request),
    .acknowledge(2'd0),
    .grant(grant_out),
    .grant_valid(grant_out_valid),
    .grant_encoded(grant_out_index)
  );

  assign pending_writes_afus[0] = ~req_wr_en && (pending_writes_fifos_out[0] == 32'd0 && empty_fifo_out[0]);
  assign pending_writes_afus[1] = ~req_wr_en && (pending_writes_fifos_out[1] == 32'd0 && empty_fifo_out[1]);
  integer rst_counter_index;

  always @(posedge clk or posedge rst) begin
    if(rst) begin
      for(rst_counter_index=0; rst_counter_index<2; rst_counter_index=rst_counter_index+1) begin
        pending_writes_fifos_out[rst_counter_index] <= 8'd0;
      end
    end else begin
      case({ req_wr_en, resp_wr_valid })
        2'b1: begin
          pending_writes_fifos_out[resp_wr_mdata] <= pending_writes_fifos_out[resp_wr_mdata] - 8'd1;
        end
        2'b10: begin
          pending_writes_fifos_out[req_wr_mdata] <= pending_writes_fifos_out[req_wr_mdata] + 8'd1;
        end
        2'b11: begin
          if(resp_wr_mdata != req_wr_mdata) begin
            pending_writes_fifos_out[resp_wr_mdata] <= pending_writes_fifos_out[resp_wr_mdata] - 8'd1;
            pending_writes_fifos_out[req_wr_mdata] <= pending_writes_fifos_out[req_wr_mdata] + 8'd1;
          end 
        end
        default: begin
        end
      endcase
    end
  end

  //Máquina responsável por fazer a leitura das configurações de ponteiros para os
  //registradores respectivos e dos dados para as filas
  //Parâmetros locais:
  localparam FSM_RD_REQ_READ_CONF = 0;
  localparam FSM_RD_REQ_READ_DATA = 1;

  always @(posedge clk or posedge rst or posedge update_workspace or posedge reset_buffers_in_flag) begin
    if(rst) begin
      addr_offset_conf <= 3'd0;
      req_rd_en <= 1'b0;
      req_rd_addr <= 64'b0;
      req_rd_mdata <= 16'b0;
      fsm_rd <= FSM_RD_REQ_READ_CONF;
      for(rst_counter_index=0; rst_counter_index<8; rst_counter_index=rst_counter_index+1) begin
        addr_offset_data_in[rst_counter_index] <= 64'd0;
      end
    end else begin
      if(update_workspace) begin
        addr_offset_conf <= 3'd0;
        req_rd_en <= 1'b0;
        req_rd_addr <= 64'b0;
        req_rd_mdata <= 16'b0;
        fsm_rd <= FSM_RD_REQ_READ_CONF;
      end else if(reset_buffers_in_flag) begin
        addr_offset_data_in[rst_buffer_in_index] <= 64'd0;
      end else begin
        if(start) begin
          req_rd_en <= 1'b0;
          case(fsm_rd)
            FSM_RD_REQ_READ_CONF: begin
              if(addr_offset_conf < conf_size) begin
                if(req_rd_available) begin
                  addr_offset_conf <= addr_offset_conf + 3'd1;
                  req_rd_addr <= configurations_addr_base + addr_offset_conf;
                  req_rd_mdata <= TAG_RD_CONF;
                  req_rd_en <= 1'b1;
                end 
              end else if(counter_received_conf >= conf_size) begin
                fsm_rd <= FSM_RD_REQ_READ_DATA;
              end 
            end
            FSM_RD_REQ_READ_DATA: begin
              if(req_rd_available && grant_in_valid && (addr_offset_data_in[grant_in_index] < qtde_data_in[grant_in_index])) begin
                addr_offset_data_in[grant_in_index] <= addr_offset_data_in[grant_in_index] + 64'd1;
                req_rd_addr <= addr_in[grant_in_index][63:6] + addr_offset_data_in[grant_in_index];
                req_rd_en <= 1'b1;
                req_rd_mdata <= grant_in_index;
              end 
            end
          endcase
        end 
      end
    end
  end

  //Máquina responsável por fazer a leitura das fifos e gravar de volta na memória
  //Parâmetros locais:
  localparam FSM_WR_REQ_IDLE = 0;
  localparam FSM_WR_REQ_WRITE_DSM = 1;
  localparam FSM_WR_REQ_WRITE_DATA = 2;

  reg [16-1:0] counter_write_dsm;
  reg dsm_wr_ready;

  wire update_dsm;
  wire fifo_out_almost_ready;
  wire fifo_out_ready;
  assign update_dsm = |((done_reg_vet ^ done_reg_vet_last) & done_reg_vet);
  assign fifo_out_almost_ready = fifos_out_read_request && !almostempty_fifo_out[grant_in_index];
  assign fifo_out_ready = fifos_out_read_request && (count_fifo_out[grant_in_index] > 5'd0);

  always @(posedge clk or posedge rst or posedge reset_buffers_out_flag) begin
    if(rst) begin
      counter_write_dsm <= 16'd0;
      req_wr_en <= 1'd0;
      req_wr_addr <= 64'd0;
      req_wr_mdata <= 16'd0;
      req_wr_data <= 512'd0;
      re_fifo_out <= 2'd0;
      grant_out_index_tmp <= 'd0;
      fsm_wr <= FSM_WR_REQ_IDLE;
      for(rst_counter_index=0; rst_counter_index<8; rst_counter_index=rst_counter_index+1) begin
        addr_offset_data_out[rst_counter_index] <= 64'd0;
      end
    end else begin
      if(reset_buffers_out_flag) begin
        addr_offset_data_out[rst_buffer_out_index - 1] <= 64'd0;
      end else begin
        if(start) begin
          req_wr_en <= 1'b0;
          re_fifo_out <= 2'd0;
          case(fsm_wr)
            FSM_WR_REQ_IDLE: begin
              if(req_wr_available) begin
                if(dsm_wr_ready) begin
                  counter_write_dsm <= 16'd0;
                  fsm_wr <= FSM_WR_REQ_WRITE_DSM;
                end else if(fifo_out_ready) begin
                  grant_out_index_tmp <= grant_out_index;
                  re_fifo_out[grant_out_index] <= 1'b1;
                  fsm_wr <= FSM_WR_REQ_WRITE_DATA;
                end 
              end 
            end
            FSM_WR_REQ_WRITE_DSM: begin
              if(counter_write_dsm >= dsm_size) begin
                fsm_wr <= FSM_WR_REQ_IDLE;
              end else if(req_wr_available) begin
                req_wr_addr <= dsm_addr_base + counter_write_dsm;
                req_wr_mdata <= TAG_WR_DSM;
                req_wr_data <= dsm[counter_write_dsm];
                req_wr_en <= 1'b1;
                counter_write_dsm <= counter_write_dsm + 16'd1;
              end 
            end
            FSM_WR_REQ_WRITE_DATA: begin
              addr_offset_data_out[grant_out_index_tmp] <= addr_offset_data_out[grant_out_index_tmp] + 64'd1;
              req_wr_addr <= addr_out[grant_out_index_tmp][63:6] + addr_offset_data_out[grant_out_index_tmp];
              req_wr_mdata <= grant_out_index_tmp;
              req_wr_data <= dout_fifo_out[grant_out_index_tmp];
              req_wr_en <= 1'b1;
              if(fifo_out_almost_ready && req_wr_available && !dsm_wr_ready) begin
                grant_out_index_tmp <= grant_out_index;
                re_fifo_out[2'b0] <= 1'b1;
                fsm_wr <= FSM_WR_REQ_WRITE_DATA;
              end else begin
                fsm_wr <= FSM_WR_REQ_IDLE;
              end
            end
          endcase
        end 
      end
    end
  end

  //Machine for receiving cache data and:
  //1 - save in configuration regs and
  //2 - queuing in the queue.

  always @(posedge clk or posedge rst or posedge update_workspace or posedge reset_buffers_in_flag) begin
    if(rst) begin
      counter_received_conf <= 1'b0;
      we_fifo_in <= 2'd0;
      resp_rd_data_tmp <= 512'd0;
      for(rst_counter_index=0; rst_counter_index<8; rst_counter_index=rst_counter_index+1) begin
        counter_received_data_in[rst_counter_index] <= 64'd0;
      end
      for(rst_counter_index=0; rst_counter_index<4; rst_counter_index=rst_counter_index+1) begin
        configurations[rst_counter_index] <= 512'd0;
      end
    end else begin
      if(update_workspace) begin
        counter_received_conf <= 1'b0;
      end else if(reset_buffers_in_flag) begin
        counter_received_data_in[rst_buffer_in_index] <= 64'd0;
      end else begin
        if(start) begin
          we_fifo_in <= 2'd0;
          if(resp_rd_valid) begin
            if(resp_rd_mdata == TAG_RD_CONF) begin
              if(counter_received_conf < conf_size) begin
                configurations[counter_received_conf] <= resp_rd_data;
                counter_received_conf <= counter_received_conf + 3'd1;
              end 
            end else begin
              resp_rd_data_tmp <= resp_rd_data;
              counter_received_data_in[resp_rd_mdata] <= counter_received_data_in[resp_rd_mdata] + 64'd1;
              we_fifo_in[resp_rd_mdata] <= 1'd1;
            end
          end 
        end 
      end
    end
  end

  //Machine responsible for receiving written data responses.

  always @(posedge clk or posedge rst or posedge reset_buffers_out_flag) begin
    if(rst) begin
      for(rst_counter_index=0; rst_counter_index<8; rst_counter_index=rst_counter_index+1) begin
        counter_sent_data_out[rst_counter_index] <= 64'd0;
      end
    end else begin
      if(reset_buffers_out_flag) begin
        counter_sent_data_out[rst_buffer_out_index] <= 64'd0;
      end else begin
        if(start) begin
          if(resp_wr_valid) begin
            if(resp_wr_mdata != TAG_WR_DSM) begin
              counter_sent_data_out[resp_wr_mdata] <= counter_sent_data_out[resp_wr_mdata] + 64'd1;
            end 
          end 
        end 
      end
    end
  end

  //Recebimento de reset dos buffers de entrada e saida

  always @(posedge clk or posedge rst) begin
    if(rst) begin
      reset_buffers_in <= 2'd0;
      reset_buffers_out <= 2'd0;
    end else begin
      if(start) begin
        if(reset_buffers_in_flag) begin
          reset_buffers_in[rst_buffer_in_index] <= 1'b1;
        end 
        if(reset_buffers_out_flag) begin
          reset_buffers_out[rst_buffer_out_index] <= 1'b1;
        end 
      end 
    end
  end

  localparam CHECK_DSM = 0;
  localparam WAIT_DSM_WRITE = 1;
  integer index;
  reg [1-1:0] fsm_dsm;
  reg [16-1:0] counter_written_dsm;

  always @(posedge clk or posedge rst) begin
    if(rst) begin
      dsm_wr_ready <= 1'b0;
      counter_written_dsm <= 16'd0;
      done_reg_vet_last <= done_reg_vet;
      for(rst_counter_index=0; rst_counter_index<3; rst_counter_index=rst_counter_index+1) begin
        dsm[rst_counter_index] <= 512'd0;
      end
      fsm_dsm <= CHECK_DSM;
    end else begin
      if(start) begin
        case(fsm_dsm)
          CHECK_DSM: begin
            if(update_dsm) begin
              done_reg_vet_last <= done_reg_vet;
              dsm[2] <= done_reg_vet;
              for(index=0; index<1; index=index+1) begin
                dsm[index] <= { counter_received_data_in[index * 8 + 7], counter_received_data_in[index * 8 + 6], counter_received_data_in[index * 8 + 5], counter_received_data_in[index * 8 + 4], counter_received_data_in[index * 8 + 3], counter_received_data_in[index * 8 + 2], counter_received_data_in[index * 8 + 1], counter_received_data_in[index * 8 + 0] };
                dsm[index + 1] <= { counter_sent_data_out[index * 8 + 7], counter_sent_data_out[index * 8 + 6], counter_sent_data_out[index * 8 + 5], counter_sent_data_out[index * 8 + 4], counter_sent_data_out[index * 8 + 3], counter_sent_data_out[index * 8 + 2], counter_sent_data_out[index * 8 + 1], counter_sent_data_out[index * 8 + 0] };
              end
              counter_written_dsm <= 16'd0;
              dsm_wr_ready <= 1'b1;
              fsm_dsm <= WAIT_DSM_WRITE;
            end 
          end
          WAIT_DSM_WRITE: begin
            if(fsm_wr == FSM_WR_REQ_WRITE_DSM) begin
              dsm_wr_ready <= 1'b0;
            end 
            if(resp_wr_valid) begin
              if(resp_wr_mdata == TAG_WR_DSM) begin
                counter_written_dsm <= counter_written_dsm + 16'd1;
              end 
            end 
            if(counter_written_dsm >= dsm_size) begin
              fsm_dsm <= CHECK_DSM;
            end 
          end
        endcase
      end 
    end
  end


  uut_afu_0_1_1
  uut_afu_0_1_1
  (
    .clk(clk),
    .rst(rst | rst_afus[0]),
    .start(start_afus[0]),
    .available_read0(~empty_fifo_in[0]),
    .almost_empty_read0(almostempty_fifo_in[0]),
    .req_rd_data0(re_fifo_in[0]),
    .rd_data0(dout_fifo_in[0]),
    .available_write0(~almostfull_fifo_out[0]),
    .req_wr_data0(we_fifo_out[0]),
    .wr_data0(din_fifo_out[0]),
    .done(done_uut_afu[0])
  );


  uut_afu_1_1_1
  uut_afu_1_1_1
  (
    .clk(clk),
    .rst(rst | rst_afus[1]),
    .start(start_afus[1]),
    .available_read0(~empty_fifo_in[1]),
    .almost_empty_read0(almostempty_fifo_in[1]),
    .req_rd_data0(re_fifo_in[1]),
    .rd_data0(dout_fifo_in[1]),
    .available_write0(~almostfull_fifo_out[1]),
    .req_wr_data0(we_fifo_out[1]),
    .wr_data0(din_fifo_out[1]),
    .done(done_uut_afu[1])
  );


  always @(*) begin
    if(rst) begin
      info <= 576'd0;
    end else begin
      info[0] <= (counter_received_conf >= 4)? 1'b1 : 1'b0;
      info[63:1] <= 63'd0;
      info[79:64] <= {8'd1, 8'd1};
      info[95:80] <= {8'd1, 8'd1};

    end
  end


endmodule
