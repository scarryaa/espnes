#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>
#include "window.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "cartridge.hpp"
#include "apu.hpp"
#include "ppu.hpp"
#include <chrono>
#include <iostream>

class Emulator
{
public:
    Emulator();
    ~Emulator();

    void load_rom(const std::string &romPath);
    void run();

private:
    Window window;
    CPU cpu;
    Cartridge cartridge;
    Memory memory;
    PPU ppu;
    APU apu;

    bool quit;
    bool paused;
};

#endif