module branch_control
(
  input clk,
  input rst,
  input [3-1:0] thread_id,
  input branch_in,
  input branch_in_en,
  output branch_out,
  input branch_out_en
);

  wire [8-1:0] stack;
  wire [8-1:0] thread_en;
  assign thread_en = 1 << thread_id;
  assign branch_out = stack[thread_id];
  genvar genv;

  generate for(genv=0; genv<8; genv=genv+1) begin : inst_branch_stack

    branch_stack
    branch_stack
    (
      .clk(clk),
      .rst(rst),
      .en(thread_en[genv]),
      .push(branch_in_en),
      .din(branch_in),
      .pop(branch_out_en),
      .dout(stack[genv])
    );

  end
  endgenerate

endmodule