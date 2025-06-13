module sine_wave_rom #(
    parameter PHASE_WIDTH  = 32,
    parameter ADDR_WIDTH   = 8,
    parameter SAMPLE_WIDTH = 16
) (
    input logic clk,

    // Only top ADDR_WIDTH bits used for LUT indexing
    // Lower bits of phase are reserved for future interpolation logic
    /* verilator lint_off UNUSEDSIGNAL */
    input logic [PHASE_WIDTH-1:0] phase,
    /* verilator lint_on UNUSEDSIGNAL */


    output logic signed [SAMPLE_WIDTH-1:0] sine_sample
);

  // Use MSBs of the phase to index the LUT
  logic [ADDR_WIDTH-1:0] addr;
  assign addr = phase[PHASE_WIDTH-1-:ADDR_WIDTH];

  // ROM to hold sine values
  logic signed [SAMPLE_WIDTH-1:0] sine_rom[0:(1 << ADDR_WIDTH)-1];

  initial begin
    // This right here is the implementation of the LUT
    // right now it is only in simulation, must change for hardware
    $readmemh("sim/lut/sine_rom.hex", sine_rom);
  end

  always_ff @(posedge clk) begin
    sine_sample <= sine_rom[addr];
  end

endmodule

