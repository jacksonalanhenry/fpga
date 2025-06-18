module nco #(
    parameter PHASE_WIDTH = 32,
    parameter DATA_WIDTH  = 16
) (
    input logic clk,
    input logic rst,

    input logic in_valid,
    output logic in_ready,
    input logic [PHASE_WIDTH-1:0] phase_inc,

    output logic out_valid,
    input logic out_ready,
    output logic signed [DATA_WIDTH-1:0] sine_out
);

  logic [PHASE_WIDTH-1:0] phase;

  logic pa_valid, pa_in_ready;
  logic sine_rom_valid, sine_in_ready;

  //back pressure
  assign in_ready = pa_in_ready;

  phase_accumulator #(
      .PHASE_WIDTH(PHASE_WIDTH)
  ) pa (
      .clk(clk),
      .rst(rst),

      .in_valid (in_valid),
      .in_ready (pa_in_ready),
      .phase_inc(phase_inc),

      .out_valid(pa_valid),
      .out_ready(sine_in_ready),
      .phase_out(phase)
  );

  sine_wave_rom #(
      .PHASE_WIDTH(PHASE_WIDTH)
  ) sin_LUT (
      .clk(clk),
      .rst(rst),

      .in_valid(pa_valid),
      .in_ready(sine_in_ready),
      .phase(phase),

      .out_valid  (sine_rom_valid),
      .out_ready  (out_ready),
      .sine_sample(sine_out)
  );

  assign out_valid = sine_rom_valid;

endmodule

