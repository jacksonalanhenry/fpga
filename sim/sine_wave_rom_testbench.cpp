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
    const int top_latency = 1; //implemented with LUT, therefore 1 cycle latency

    const int num_cycles = 256;
    const int addr_width = 8;
    const int phase_width = 32;
    const int sample_width = 16;
    const uint32_t phase_inc = (1u << (phase_width - addr_width));  // 1 address per cycle
                                                                    //
    std::queue<int> expected_queue;

    uint32_t phase = 0;

    top->clk = 0;
    top->eval();


    for (int i = 0; i < num_cycles * 2; ++i) {
        top->clk = !top->clk;
        top->eval();

        if (top->clk) { //posedge-clk
            top->phase = phase;

            // Calculate expected output
            uint32_t addr = phase >> (phase_width - addr_width);
            double radians = (2.0 * M_PI * addr) / (1 << addr_width);
            int16_t expected = static_cast<int16_t>(std::round(std::sin(radians) * ((1 << (sample_width - 1)) - 1)));
            // Put the expected output on the queue
            expected_queue.push(expected);

            if (i >= top_latency) {
                int expected = expected_queue.front(); //record the expected from the front
                expected_queue.pop(); //pop expected off the front
                int16_t actual = top->sine_sample;

                // Accept small error margin (LUT precision)
                assert(std::abs(expected - actual) <= 1 && "Mismatch in sine ROM output");

                //print some extra debug output
                std::cout << "Cycle " << (i / 2)
                      << " | Addr: " << addr
                      << " | Expected: " << expected
                      << " | Actual: " << actual << "\n";
            }




            phase += phase_inc;
        }
    }

    delete top;
    std::cout << "Test passed.\n";
    return 0;
}

