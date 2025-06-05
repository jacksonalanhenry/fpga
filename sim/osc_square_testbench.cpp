#include "Vosc_square.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <fstream>

int main(int argc, char** argv) {
    const char* vcdfile = "sim/outputs/default_wave.vcd"; // default

    if (argc > 1) {
        vcdfile = argv[1];
    }


    Verilated::commandArgs(argc, argv);
    Vosc_square* top = new Vosc_square;

    VerilatedVcdC* tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open(vcdfile);


    const double clk_rate = 48000.0;
    const double freq = 440.0;
    const uint32_t PHASE_INC = (uint32_t)((freq / clk_rate) * (1ull << 32));

    std::ofstream out("sim/outputs/waveform.txt");

    top->clk = 0;
    top->rst = 1;
    top->eval();
    tfp->dump(0);

    top->rst = 0;
    top->enable = 1;
    top->phase_inc = PHASE_INC;

    for (int i = 0; i < 48000; ++i) {
        top->clk = !top->clk;
        top->eval();
        tfp->dump(i * 10);

        if (top->clk) {
            out << (top->wave_out ? 1.0f : -1.0f) << "\n";
        }
    }

    tfp->close();
    delete top;
    return 0;
}

