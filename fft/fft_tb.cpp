//
#include <stdio.h>
#include <stdlib.h>
#include "Vfft_wrapper.h"

#include "verilated.h"
#include "verilated_vcd_c.h"


#include <iostream>
#include <fstream>
#include <cstdint>

int main(int argc, char **argv) {
	// Call commandArgs first!
	Verilated::commandArgs(argc, argv);

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;



	// Instantiate our design
	Vfft_wrapper *dut = new Vfft_wrapper;

    dut->trace(m_trace, 5);
    m_trace->open("waveform.vcd");

	std::ifstream inputFile("../../dsp/dsss_sig.bin", std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

	int16_t value;
    vluint64_t sim_time = 0;

    while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(value))) {


		dut->i_val = value;



        dut->eval();
        m_trace->dump(sim_time);
        sim_time++;

        printf("sw = %d, ", dut->i_val);
        printf("led = %d\n", dut->o_val);



	}

    m_trace->close();
    inputFile.close();
    exit(EXIT_SUCCESS);
}
