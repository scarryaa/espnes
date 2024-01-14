#include "../include/cartridge.hpp"

Cartridge::Cartridge()
{
}

Cartridge::~Cartridge()
{
}

uint8_t Cartridge::read(uint16_t address)
{
    return this->rom[address];
}

void Cartridge::write(uint16_t address, uint8_t value)
{
    return;
}