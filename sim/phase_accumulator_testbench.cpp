#include "Vphase_accumulator.h"
#include "verilated.h"
#include <cassert>
#include <iostream>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Vphase_accumulator* top = new Vphase_accumulator;

    const int num_cycles = 20;
    uint32_t phase_inc = 12345678;
    uint32_t expected_phase = 0;

    top->rst = 1;
    top->clk = 0;
    top->in_valid = 0;
    top->out_ready = 0;
    top->phase_inc = phase_inc;
    top->eval();

    // Deassert reset
    top->rst = 0;
    top->in_valid = 1;
    top->out_ready = 1;

    for (int i = 0; i < num_cycles * 2; ++i) {
        top->clk = !top->clk;
        top->eval();

        if (top->clk && top->out_valid) {
            expected_phase += phase_inc;


            std::cout << "Cycle " << i / 2 
                << ": expected = " << expected_phase 
                << ", phase_out = " << top->phase_out << "\n";
            assert(top->phase_out == expected_phase && "Phase accumulator mismatch");
        }
    }

    delete top;
    std::cout << "Test passed!\n";
    return 0;
}

