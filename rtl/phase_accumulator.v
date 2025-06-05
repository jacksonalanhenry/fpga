module phase_accumulator #(
    parameter PHASE_WIDTH = 32
)(
    input  logic clk,
    input  logic rst,
    input  logic [PHASE_WIDTH-1:0] phase_inc,
    output logic [PHASE_WIDTH-1:0] phase_out
);

    always_ff @(posedge clk or posedge rst) begin
        if (rst)
            phase_out <= 0;
        else
            phase_out <= phase_out + phase_inc;
    end

endmodule

