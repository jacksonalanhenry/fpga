#include "Vsine_wave_rom.h"
#include "verilated.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <queue>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Vsine_wave_rom* top = new Vsine_wave_rom;

    const int num_cycles = 200;
    const int addr_width = 8;
    const int phase_width = 32;
    const int sample_width = 16;
    const uint32_t phase_inc = (1u << (phase_width - addr_width));  // 1 address per cycle

    std::queue<int16_t> expected_queue;
    uint32_t phase = 0;

    // Initialize inputs
    top->rst = 1;
    top->clk = 0;
    top->in_valid = 0;
    top->out_ready = 1;
    top->eval();

    // Deassert reset after one cycle
    top->rst = 0;
    top->in_valid = 1;

    // Run for enough cycles to see num_cycles outputs (plus latency)
    for (int cycle = 0; cycle < num_cycles; ++cycle) {
        // Toggle clock low -> high
        top->clk = 0;
        top->eval();

        top->clk = 1;

        // On rising edge, if input ready, drive input and enqueue expected output
        if (top->in_ready && cycle < num_cycles) {
            top->phase = phase;

            // Calculate expected value for this phase
            uint32_t addr = phase >> (phase_width - addr_width);
            double radians = (2.0 * M_PI * addr) / (1 << addr_width);
            int16_t expected = static_cast<int16_t>(std::round(std::sin(radians) * ((1 << (sample_width - 1)) - 1)));

            expected_queue.push(expected);

            phase += phase_inc;
            top->in_valid = 1;
        } else {
            top->in_valid = 0;
        }

        // evaluate after driving inputs
        top->eval();

        // When output valid and ready, pop expected and compare
        if (top->out_valid && top->out_ready) {
            assert(!expected_queue.empty() && "Expected queue empty when output valid");
            int16_t expected = expected_queue.front();
            expected_queue.pop();

            int16_t actual = top->sine_sample;

            // Accept small error margin (LUT precision)
            assert(std::abs(expected - actual) <= 1 && "Mismatch in sine ROM output");

            std::cout << "Cycle " << cycle 
                      << " | Expected: " << expected 
                      << " | Actual: " << actual << "\n";
        }
    }

    delete top;
    std::cout << "Test passed.\n";
    return 0;
}

