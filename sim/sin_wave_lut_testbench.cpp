#include "Vsin_wave_lut.h"
#include "verilated.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <cassert>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Vsin_wave_lut* top = new Vsin_wave_lut;

    const int num_cycles = 256;
    const int addr_width = 8;
    const int phase_width = 32;
    const int sample_width = 16;
    const uint32_t phase_inc = (1u << (phase_width - addr_width));  // 1 address per cycle

    uint32_t phase = 0;

    top->clk = 0;
    top->eval();

    for (int i = 0; i < num_cycles * 2; ++i) {
        top->clk = !top->clk;
        top->eval();

        if (top->clk) {
            top->phase = phase;

            // Calculate expected output
            uint32_t addr = phase >> (phase_width - addr_width);
            double radians = (2.0 * M_PI * addr) / (1 << addr_width);
            int16_t expected = static_cast<int16_t>(std::round(std::sin(radians) * ((1 << (sample_width - 1)) - 1)));

            int16_t actual = top->sine_sample;

            // Accept small error margin (LUT precision)
            assert(std::abs(expected - actual) <= 1 && "Mismatch in sine LUT output");

            std::cout << "Cycle " << (i / 2)
                      << " | Addr: " << addr
                      << " | Expected: " << expected
                      << " | Actual: " << actual << "\n";

            phase += phase_inc;
        }
    }

    delete top;
    std::cout << "Test passed.\n";
    return 0;
}

