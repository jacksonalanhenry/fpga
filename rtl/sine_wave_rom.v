module sine_wave_rom #(
    parameter PHASE_WIDTH  = 32,
    parameter ADDR_WIDTH   = 8,
    parameter SAMPLE_WIDTH = 16
) (
    input logic clk,
    input logic rst,

    input logic in_valid,
    output logic in_ready,
    // Only top ADDR_WIDTH bits used for LUT indexing
    // Lower bits of phase are reserved for future interpolation logic
    /* verilator lint_off UNUSEDSIGNAL */
    input logic [PHASE_WIDTH-1:0] phase,
    /* verilator lint_on UNUSEDSIGNAL */

    output logic out_valid,
    input logic out_ready,
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

  logic [ADDR_WIDTH-1:0] addr_reg;
  logic out_valid_reg;

  always_ff @(posedge clk or posedge rst) begin
    if (rst) begin
      addr_reg      <= 0;
      out_valid_reg <= 0;
    end else begin
      if (in_valid && in_ready) begin
        addr_reg <= addr;
        out_valid_reg <= 1'b1;
      end else if (out_ready) begin
        out_valid_reg <= 1'b0;
      end
    end
  end

  assign sine_sample = sine_rom[addr_reg];
  assign out_valid   = out_valid_reg;
  assign in_ready    = !out_valid_reg || out_ready;

endmodule

