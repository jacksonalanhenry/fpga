// generate_lut.cpp
#include <cmath>
#include <fstream>
#include <cstdint>

int main() {
    const int N = 256; //ADDR_WIDTH in sin_wave_lut is 8. 2^8=256 entries in this sin_wave_lut
    const int AMP = (1 << 15) - 1;
    std::ofstream out("sim/lut/sine_rom.hex");
    for (int i = 0; i < N; ++i) {
        double angle = 2 * M_PI * i / N;
        int16_t sample = std::round(std::sin(angle) * AMP);
        out << std::hex << ((uint16_t)sample & 0xFFFF) << "\n";
    }
    return 0;
}

