`timescale 1ps/1ps
`include "basicCounter.v"

module tb_basicCounter;
reg A;
wire B;

basicCounter mybasicCounter_uut(
    .A(A),
    .B(B)
    );


initial begin
    $dumpfile("tb_basicCounter.vcd");
    $dumpvars(0, tb_basicCounter);

    A = 0;
    #20;

    A = 1;
    #20;

    A = 0;
    #20;

    $display("Test Completed!!");


end


endmodule
