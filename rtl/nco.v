module nco #(
    parameter PHASE_WIDTH = 32,
    parameter DATA_WIDTH  = 16
) (
    input logic clk,
    input logic rst,
    input logic [PHASE_WIDTH-1:0] phase_inc,
    output logic signed [DATA_WIDTH-1:0] sine_out
);

  logic [PHASE_WIDTH-1:0] phase;

  //1 cycle latency
  phase_accumulator #(
      .PHASE_WIDTH(PHASE_WIDTH)
  ) pa (
      .clk(clk),
      .rst(rst),
      .phase_inc(phase_inc),
      .phase_out(phase)
  );

  //assign rom_addr = phase[PHASE_WIDTH-1-:ADDR_WIDTH];

  //1 cycle latency
  sine_wave_rom #(
      .PHASE_WIDTH(PHASE_WIDTH)
  ) sin_LUT (
      .clk(clk),
      .phase(phase),
      .sine_sample(sine_out)
  );

endmodule

