#include "../include/emulator.hpp"

int main(int argv, char** args)
{
    // Clear log file
    std::ofstream log_file;
    log_file.open("log.txt", std::ofstream::out | std::ofstream::trunc);
    log_file.close();

    Emulator emulator;
    emulator.load_rom("roms/nestest.nes");
    emulator.set_PC_to_reset_vector();
    emulator.run();

    return 1;
}