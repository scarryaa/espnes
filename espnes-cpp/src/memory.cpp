#include "../include/memory.hpp"

Memory::Memory(PPU* ppu, APU* apu, Cartridge* cartridge) : ppu(ppu), apu(apu), cartridge(cartridge)
{
    memory = new uint8_t[0x10000];
    ram = new uint8_t[0x800];
    stack = new uint8_t[0x100];
}

Memory::~Memory()
{
    delete[] memory;
    delete[] ram;
    delete[] stack;
}

uint8_t Memory::read(uint16_t address)
{
    // Read from stack
    if (address >= 0x100 && address <= 0x1FF)
    {
        return stack[address - 0x100];
    }
    // Read from RAM
    else if (address >= 0x0000 && address < 0x2000)
    {
        return ram[address % 0x0800];
    }
    // Read from PPU
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        return ppu->read(address);
    }
    // Read from APU
    else if (address >= 0x4000 && address <= 0x4017)
    {
        return apu->read(address);
    }
    // Unknown
    else if (address >= 0x4018 && address <= 0x401F)
    {
        return 0;
    }
    // Read from cartridge
    else if (address >= 0x4020 && address <= 0xFFFF)
    {
        return cartridge->read(address);
    }
    else
    {
        Debug::debug_print("Unknown memory read: " + std::to_string(address));
    }

    return 0;
}

void Memory::write(uint16_t address, uint8_t value)
{
    // Write to stack
    if (address >= 0x100 && address <= 0x1FF)
    {
        stack[address - 0x100] = value;
    }
    // Write to RAM
    else if (address >= 0x0000 && address < 0x2000)
    {
        ram[address % 0x0800] = value;
    }
    // Write to PPU
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        ppu->write(address, value);
    }
    // Write to APU
    else if (address >= 0x4000 && address <= 0x4017)
    {
        apu->write(address, value);
    }
    // Unknown
    else if (address >= 0x4018 && address <= 0x401F)
    {
        return;
    }
    // Write to cartridge
    // else if (address >= 0x4020 && address <= 0xFFFF)
    // {
    //     cartridge->write(address, value);
    // }
    else
    {
        Debug::debug_print("Unknown memory write: " + std::to_string(address));
    }
}

void Memory::load(uint8_t* rom, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        memory[i] = rom[i];
    }
}