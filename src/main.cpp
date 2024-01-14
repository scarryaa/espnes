#include "../include/emulator.hpp"

int main(int argv, char **args)
{
    Emulator emulator;
    emulator.load_rom("../roms/official.nes");
    emulator.run();

    return 1;
}