#include "../include/cartridge.hpp"

Cartridge::Cartridge()
{
    rom = new uint8_t[0x10000];
}

Cartridge::~Cartridge()
{
    delete[] rom;
}

uint8_t Cartridge::read(uint16_t address)
{
    return rom[address];
}

void Cartridge::write(uint16_t address, uint8_t value)
{
    return;
}

void Cartridge::load(uint8_t *rom, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        this->rom[i] = rom[i];
    }
}