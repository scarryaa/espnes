#include "../include/ppu.hpp"

PPU::PPU()
{
    vram = new uint8_t[0x2000];
    oam = new uint8_t[0x100];
    palette = new uint8_t[0x20];
}

PPU::~PPU()
{
    delete[] vram;
    delete[] oam;
    delete[] palette;
}

uint8_t PPU::read(uint16_t address)
{
    return 0;
}

void PPU::write(uint16_t address, uint8_t value)
{
}

void PPU::load(uint8_t *rom, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        vram[i] = rom[i];
    }
}