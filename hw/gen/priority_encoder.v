
module priority_encoder #
(
  parameter WIDTH = 4,
  parameter LSB_PRIORITY = "LOW"
)
(
  input [WIDTH-1:0] input_unencoded,
  output [1-1:0] output_valid,
  output [$clog2(WIDTH)-1:0] output_encoded,
  output [WIDTH-1:0] output_unencoded
);

  localparam W1 = 2**$clog2(WIDTH);
  localparam W2 = W1/2;

  generate if(WIDTH == 2) begin : if_width
    assign output_valid = input_unencoded[0] | input_unencoded[1];
    if(LSB_PRIORITY == "LOW") begin : if_low
      assign output_encoded = input_unencoded[1];
    end else begin : else_low
      assign output_encoded = ~input_unencoded[0];
    end
  end else begin : else_width
    wire [$clog2(W2)-1:0] out1;
    wire [$clog2(W2)-1:0] out2;
    wire valid1;
    wire valid2;
    wire [WIDTH-1:0] out_un;
    priority_encoder #(
                .WIDTH(W2),
                .LSB_PRIORITY(LSB_PRIORITY)
            )
            priority_encoder_inst1 (
                .input_unencoded(input_unencoded[W2-1:0]),
                .output_valid(valid1),
                .output_encoded(out1),
                .output_unencoded(out_un[W2-1:0])
            );
            priority_encoder #(
                .WIDTH(W2),
                .LSB_PRIORITY(LSB_PRIORITY)
            )
            priority_encoder_inst2 (
                .input_unencoded({{W1-WIDTH{1'b0}}, input_unencoded[WIDTH-1:W2]}),
                .output_valid(valid2),
                .output_encoded(out2),
                .output_unencoded(out_un[WIDTH-1:W2])
            );
           assign output_valid = valid1 | valid2;
            if (LSB_PRIORITY == "LOW") begin
                assign output_encoded = valid2 ? {1'b1, out2} : {1'b0, out1};
            end else begin
                assign output_encoded = valid1 ? {1'b0, out1} : {1'b1, out2};
            end
  end
  endgenerate

  assign output_unencoded = 1 << output_encoded;

endmodule
