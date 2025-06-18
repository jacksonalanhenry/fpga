#include "Vnco.h"
#include "verilated.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <vector>
#include "verilated_vcd_c.h"



#define NUM_CYCLES 10000
#define PHASE_WIDTH 32
#define FREQ 440
#define SAMPLE_RATE 44100

uint32_t calculate_phase_inc(double frequency, double sample_rate, int phase_bits = 32) {
    double rel_freq = frequency / sample_rate; // or pass rel_freq directly
    return static_cast<uint32_t>(rel_freq * (1ULL << phase_bits));
}

//calculate expected output helper func
double estimate_phase_increment(const std::vector<int16_t>& samples, double sample_rate, uint32_t phase_bits = 32) {
    // Find two successive rising zero crossings
    int first = -1, second = -1;

    for (size_t i = 1; i < samples.size(); ++i) {
        if (samples[i - 1] < 0 && samples[i] >= 0) {
            if (first == -1)
                first = i;
            else {
                second = i;
                break;
            }
        }
    }

    if (first == -1 || second == -1)
        throw std::runtime_error("Not enough zero crossings found.");

    int period_samples = second - first;
    double freq = sample_rate / period_samples;
    double phase_inc = (freq * std::pow(2.0, phase_bits)) / sample_rate;

    return phase_inc;
}


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

    // Track expected output
    std::vector<int16_t> sine_wave_buffer;

    uint32_t phase_inc = calculate_phase_inc(FREQ, SAMPLE_RATE, PHASE_WIDTH);

    //some initial values
    nco->clk = 0;
    nco->rst = 0;
    nco->phase_inc = phase_inc;
    nco->eval();
    tfp->dump(main_time++);

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

    //start the main clock loop
    for (int clk_tick = 0; clk_tick < NUM_CYCLES; ++clk_tick) {
        // Clock low
        nco->clk = 0;
        nco->out_ready = 1;
        nco->eval();
        tfp->dump(main_time++);

        // Clock high
        nco->clk = 1;
        nco->out_ready = 1;
        nco->eval();
        tfp->dump(main_time++);


        // On rising edge, if input ready, drive input
        if (nco->in_ready) {
            nco->phase_inc = phase_inc;
            nco->in_valid = 1;
        } else {
            nco->in_valid = 0;
        }

        //evaluate after driving inputs
        nco->eval();

        // When output valid, save the outputs to compare later
        if (nco->out_valid) {
            int16_t nco_sine_sample = nco->sine_out;
            sine_wave_buffer.push_back(nco_sine_sample);
        }

    } //end main clock loop


    //Now that we have a sine_wave_buffer, we need to find the frequency (phase_inc) and check if that is what is expected
    double est_phase_inc = estimate_phase_increment(sine_wave_buffer, SAMPLE_RATE, PHASE_WIDTH);
    std::cout << "est phase inc: " << est_phase_inc << std::endl;
    std::cout << "Phase Inc: " << phase_inc << std::endl;
    double rel_error = std::abs((double)phase_inc - est_phase_inc) / phase_inc;
    std::cout << "Relative error: " << rel_error << std::endl;
    // Accept small error margin (LUT precision)
    assert(rel_error < 0.1 && "Estimated phase_inc is too far off!");


    delete nco;
    tfp->close();

    std::cout << "Test passed.\n";
    return 0;
}


