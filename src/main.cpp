#include "../include/emulator.hpp"

int main(int argv, char **args)
{
    Emulator emulator;
    emulator.loadROM("roms/official.nes");
    emulator.run();

    return 1;
}