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
    uint8_t register_index = address & 8;
    switch (register_index)
    {
    case 0:
        // PPUCTRL
        return this->control;
        break;
    case 1:
        // PPUMASK
        return this->mask;
        break;
    case 2:
        // PPUSTATUS
        return this->status;
        break;
        // OAMADDR
    case 3:
        return this->oam_address;
        break;
    case 4:
        // OAMDATA
        return this->oam_data;
        break;
    case 5:
        // PPUSCROLL
        return this->scroll;
        break;
    case 6:
        // PPUADDR
        return this->address;
        break;
    case 7:
        // PPUDATA
        return this->data;
        break;
    case 8:
        // OAMDMA
        return this->oam_dma;
        break;
    }

    std::cout << "Error: Invalid PPU register" << std::endl;
    return 0;
}

void PPU::write(uint16_t address, uint8_t value)
{
    uint8_t register_index = address & 8;
    switch (register_index)
    {
    case 0:
        // PPUCTRL
        this->control = value;
        break;
    case 1:
        // PPUMASK
        this->mask = value;
        break;
    case 2:
        // PPUSTATUS
        this->status = value;
        break;
        // OAMADDR
    case 3:
        this->oam_address = value;
        break;
    case 4:
        // OAMDATA
        this->oam_data = value;
        break;
    case 5:
        // PPUSCROLL
        this->scroll = value;
        break;
    case 6:
        // PPUADDR
        this->address = value;
        break;
    case 7:
        // PPUDATA
        this->data = value;
        break;
    case 8:
        // OAMDMA
        this->oam_dma = value;
        break;
    }
}

void PPU::load(uint8_t *rom, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        vram[i] = rom[i];
    }
}