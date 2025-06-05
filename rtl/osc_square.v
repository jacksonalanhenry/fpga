module osc_square (
    input logic clk,
    input logic rst,
    input logic enable,
    input logic [31:0] phase_inc,
    output logic wave_out
);

    logic [31:0] phase;

    always_ff @(posedge clk) begin
        if (rst) begin
            phase <= 0;
            wave_out <= 0;
        end else if (enable) begin
            phase <= phase + phase_inc;
            wave_out <= phase[31];  // MSB is square wave
        end
    end
endmodule

