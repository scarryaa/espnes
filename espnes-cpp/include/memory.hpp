#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <iostream>
#include "ppu.hpp"
#include "apu.hpp"
#include "controller.hpp"
#include "cartridge.hpp"
#include "debug/debug.hpp"
#include <string>

class Memory
{
public:
    Memory(PPU *ppu, APU *apu, Cartridge *cartridge, Controller *controller);
    ~Memory();

    uint8_t read(uint16_t address, bool resetStatus = true);
    void write(uint16_t address, uint8_t value);
    void load(uint8_t *rom, uint32_t size);

private:
    uint8_t *memory;
    uint8_t *ram;
    uint8_t *stack;

    PPU *ppu;
    APU *apu;
    Cartridge *cartridge;
    Controller *controller;
};

#endif
