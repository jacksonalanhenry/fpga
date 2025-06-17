module phase_accumulator #(
    parameter PHASE_WIDTH = 32
) (
    input logic clk,
    input logic rst,

    input logic in_valid,
    output logic in_ready,
    input logic [PHASE_WIDTH-1:0] phase_inc,

    output logic out_valid,
    input logic out_ready,
    output logic [PHASE_WIDTH-1:0] phase_out
);

  logic [PHASE_WIDTH-1:0] acc_reg;
  logic valid_reg;

  always_ff @(posedge clk or posedge rst) begin
    if (rst) begin
      acc_reg   <= 0;
      valid_reg <= 0;
    end else begin
      if (in_valid && in_ready) begin
        acc_reg   <= acc_reg + phase_inc;
        valid_reg <= 1'b1;
      end else if (out_ready) begin
        valid_reg <= 1'b0;
      end
    end
  end

  assign phase_out = acc_reg;
  assign out_valid = valid_reg;
  assign in_ready  = !valid_reg || out_ready;


endmodule

