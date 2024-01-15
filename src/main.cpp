#include "../include/emulator.hpp"

int main(int argv, char **args)
{
    Emulator emulator;
    emulator.load_rom("../roms/Donkey Kong.nes");
    emulator.set_PC_to_reset_vector();
    emulator.run();

    return 1;
}