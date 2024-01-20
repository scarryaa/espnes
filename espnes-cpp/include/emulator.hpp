#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>
#include "window.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "cartridge.hpp"
#include "debug/debug.hpp"
#include "debug/disassembler.hpp"
#include "controller.hpp"
#include "apu.hpp"
#include "ppu.hpp"
#include <functional>
#include <chrono>
#include <iostream>
#include <vector>
#include "../include/breakpoint_types.hpp"

class Emulator
{
public:
    Emulator();
    ~Emulator();

    void set_dma_triggered(bool dma_triggered);
    void set_PC_to_reset_vector();
    void load_rom(const std::string &romPath);
    void run();
    void step();
    void pause();
    void reset();
    bool is_paused();
    void log_cpu();
    void clear_breakpoint(breakpoint_type_t type, uint16_t value);
    void clear_all_breakpoints();
    bool is_breakpoint(breakpoint_type_t type, long value);
    void check_for_breakpoints();
    void open_log_file();
    void close_log_file();
    std::set<Breakpoint> get_breakpoints();
    std::set<Breakpoint> get_breakpoints_of_type(breakpoint_type_t type);

    void add_breakpoint(breakpoint_type_t type, uint16_t value);

    uint16_t get_PC();
    CPU *get_CPU();
    PPU *get_PPU();
    Memory *get_memory();
    Disassembler get_disassembler();

private:
    std::ofstream log_file;
    std::set<Breakpoint> breakpoints;
    Window window;
    CPU cpu;
    Cartridge cartridge;
    Memory memory;
    PPU ppu;
    APU apu;
    Controller controller;
    Disassembler disassembler;

    bool dma_triggered;
    bool quit;
    bool paused;
    uint16_t reset_vector;
};

#endif