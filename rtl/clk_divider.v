//this is basically freq control
//it takes in a freqvalue (a 'note') and outputs a tick at the rate of that
//note
module clk_divider (
    input logic clk,
    input logic rst,
    input logic [63:0] freq_value,
    output logic tick
);

  logic [63:0] counter;

  always_ff @(posedge clk) begin
    if (rst) begin
      tick <= 0;
      counter <= 0;
    end else begin
      if (counter == freq_value) begin
        tick <= 1;
        counter <= 0;
      end else begin
        tick <= 0;
        counter <= counter + 1;
      end
    end
  end

endmodule
