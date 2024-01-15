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
    uint16_t relativeAddress = address - 0x8000; // ROM starts at 0x8000
    uint16_t bankAddress = (currentBank * BANK_SIZE) + (relativeAddress % BANK_SIZE);
    return rom[bankAddress];
}

void Cartridge::switchBank(uint16_t bank)
{
    currentBank = bank;
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