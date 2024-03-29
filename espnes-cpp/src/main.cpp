#include "../include/emulator.hpp"

int main(int argv, char** args)
{
    // Clear log file
    std::ofstream log_file;
    log_file.open("log.txt", std::ofstream::out | std::ofstream::trunc);
    log_file.close();

    Emulator emulator;
    emulator.open_log_file();
    emulator.load_rom("roms/Donkey Kong.nes");
    emulator.set_PC_to_reset_vector();
    emulator.run();
    emulator.close_log_file();

    return 1;
}