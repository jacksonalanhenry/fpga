#include "Vnco.h"
#include "verilated.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <queue>
#include <cassert>
#include "verilated_vcd_c.h"



#define NUM_CYCLES 300
#define FREQ_WORD  (uint32_t(1) << 24)  // tune for ~1Hz output if clk is 100MHz
#define LUT_SIZE 256
#define MAX_AMPLITUDE 32767


int main(int argc, char** argv) {
    const char* vcdfile = "sim/outputs/default_wave.vcd"; // default
    int main_time = 0;

    if (argc > 1) {
        vcdfile = argv[1];
    }
    Verilated::commandArgs(argc, argv);
    Vnco* nco = new Vnco;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    nco->trace(tfp, 99);  // 99 is trace depth
    tfp->open(vcdfile);

    nco->clk = 0;
    nco->rst = 0;
    nco->phase_inc = FREQ_WORD;
    nco->eval();
    tfp->dump(main_time++);

    // Set up c++ LUT values for checking
    std::vector<int16_t> sine_rom(LUT_SIZE);
    for (int i = 0; i < LUT_SIZE; ++i) {
        double angle = (2.0 * M_PI * i) / LUT_SIZE;
        sine_rom[i] = int16_t(std::round(std::sin(angle) * MAX_AMPLITUDE));
    }

    // Reset
    // Assert reset for 2 full cycles
    nco->rst = 1;
    for (int i = 0; i < 4; ++i) {
        nco->clk = i % 2;
        nco->eval();
        tfp->dump(main_time++);
    }
    
    // Deassert reset
    nco->rst = 0;
    for (int i = 0; i < 2; ++i) {
        nco->clk = i % 2;
        nco->eval();
        tfp->dump(main_time++);
    }

    // Track expected output
    std::queue<uint32_t> phase_queue;
    const int LUT_LATENCY = 1; // adjust if sine ROM has pipeline

    for (int clk_tick = 0; clk_tick < NUM_CYCLES; ++clk_tick) {
        // Clock low
        nco->clk = 0;
        nco->eval();
        tfp->dump(main_time++);

        // Clock high
        nco->clk = 1;
        nco->eval();
        tfp->dump(main_time++);

        // Track phase and check expected output
        static uint32_t phase = 0;
        phase += FREQ_WORD;
        uint8_t index = phase >> (32 - 8);
        phase_queue.push(index);

        if (phase_queue.size() >= LUT_LATENCY) {
            uint8_t expected_index = phase_queue.front();
            int16_t expected = sine_rom[expected_index];
            int16_t actual = static_cast<int16_t>(nco->sine_out);

            std::cout << "Clock Tick " << clk_tick
                      << " | Addr: " << int(expected_index)
                      << " | Expected: " << expected
                      << " | Actual: " << actual
                      << std::endl;

            phase_queue.pop();
        }
    }



    delete nco;
    tfp->close();

    std::cout << "Test passed.\n";
    return 0;
}

