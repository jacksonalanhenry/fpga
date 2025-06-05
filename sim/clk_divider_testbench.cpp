#include "Vclk_divider.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <fstream>


int main(int argc, char** argv) {
    const char* vcdfile = "sim/outputs/default_wave.vcd"; // default

    if (argc > 1) {
        vcdfile = argv[1];
    }

    Verilated::commandArgs(argc, argv);
    Vclk_divider* top = new Vclk_divider;

    VerilatedVcdC* tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open(vcdfile);


    const double clk_rate = 48000.0;
    const double freq = 440.0;
    // const uint32_t PHASE_INC = (uint32_t)((freq / clk_rate) * (1ull << 32));

    std::ofstream out("sim/outputs/waveform.txt");
    std::ofstream debug_file("sim/outputs/debug_file.txt");

    top->clk = 0;
    top->rst = 1;
    top->freq_value = freq;
    top->eval();
    tfp->dump(0);

    top->rst = 0;

    for (int i = 0; i < 48000; ++i) {
        for (int phase = 0; phase < 2; ++phase) {
            top->clk = phase;
            top->eval();
            tfp->dump(i * 20 + phase * 10); // 10 time units per half-cycle
        }

        out << (top->tick ? 1.0f : -1.0f) << "\n";
        debug_file << i << " : " << (top->tick ? 1.0f : -1.0f) << "\n";
    }


    tfp->close();
    delete top;
    return 0;
}

