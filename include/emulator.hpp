#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>
#include "window.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "cartridge.hpp"
#include "debug/debug.hpp"
#include "debug/disassembler.hpp"
#include "apu.hpp"
#include "ppu.hpp"
#include <functional>
#include <chrono>
#include <iostream>
#include <vector>

class Emulator
{
public:
    Emulator();
    ~Emulator();

    void set_PC_to_reset_vector();
    void load_rom(const std::string &romPath);
    void run();
    void step();
    void pause();
    void reset();
    bool is_paused();
    uint16_t get_PC();
    CPU *get_CPU();
    Disassembler get_disassembler();

private:
    Window window;
    CPU cpu;
    Cartridge cartridge;
    Memory memory;
    PPU ppu;
    APU apu;
    Disassembler disassembler;

    bool quit;
    bool paused;
    uint16_t reset_vector;
};

#endif