#include "../include/memory.hpp"
#include <emulator.hpp>

Memory::Memory(PPU * ppu, APU* apu, Cartridge* cartridge, Controller *controller) : ppu(ppu), apu(apu), cartridge(cartridge), controller(controller)
{
    memory = new uint8_t[0x10000];
    ram = new uint8_t[0x800];
    stack = new uint8_t[0x100];

    for (int i = 0; i < 0x10000; i++)
    {
		memory[i] = 0;
	}

    for (int i = 0; i < 0x800; i++)
    {
		ram[i] = 0;
	}

    for (int i = 0; i < 0x100; i++)
    {
		stack[i] = 0;
	}
}

Memory::~Memory()
{
    delete[] memory;
    delete[] ram;
    delete[] stack;
}

void Memory::set_emulator(Emulator* emulator)
{
	this->emulator = emulator;
}

uint8_t Memory::read(uint16_t address, bool resetStatus)
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
        return ppu->read(address, resetStatus);
    }
    // Read from APU
    else if (address >= 0x4000 && address <= 0x4017)
    {
        // Controller 1
        if (address == 0x4016)
        {
            return controller->read_controller_1();
		}
        // Controller 2
        else if (address == 0x4017)
        {
            return controller->read_controller_2();
        }
        else
        {
			return apu->read(address);
		}
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
    if (address == 0xFA && value == 0x36)
    {
        printf("0xD0\n");
    }

    // Check for write breakpoints
    if (emulator->is_breakpoint(BREAKPOINT_TYPE_WRITE, address))
    {
		emulator->pause();
	}

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
        // OAM DMA
        if (address == 0x4014)
        {
            uint16_t dma_address = value * 0x100;

            for (int i = 0; i < 256; i++)
            {
                uint8_t byte = read(dma_address + i);
                ppu->write_oam_data(i, byte);
            }

            emulator->set_dma_triggered(true);
		}
        // Controller 1
        if (address == 0x4016)
        {
			controller->write_controller_1(value);
		}

        // Controller 2
        else if (address == 0x4017)
        {
			return;
		}
		else
		{
            apu->write(address, value);
        }
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