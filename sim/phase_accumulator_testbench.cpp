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
    top->phase_inc = phase_inc;
    top->eval();

    // Deassert reset
    top->rst = 0;

    for (int i = 0; i < num_cycles * 2; ++i) {
        top->clk = !top->clk;
        top->eval();

        if (top->clk) {
            expected_phase += phase_inc;

            assert(top->phase_out == expected_phase && "Phase accumulator mismatch");
            std::cout << "Cycle " << i / 2 << ": phase_out = " << top->phase_out << "\n";
        }
    }

    delete top;
    std::cout << "Test passed!\n";
    return 0;
}

