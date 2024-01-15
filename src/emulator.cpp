#include "../include/emulator.hpp"

Emulator::Emulator() : cpu(&memory), ppu(), apu(), window(), cartridge(), memory(&ppu, &apu, &cartridge), quit(false), paused(false)
{
    ppu.set_cpu(cpu);
}

Emulator::~Emulator()
{
}

void Emulator::load_rom(const std::string &romPath)
{
    // Open the file
    std::ifstream rom(romPath, std::ios::binary);
    if (!rom)
    {
        throw std::runtime_error("Failed to open ROM file");
    }

    // Read the header
    std::vector<uint8_t> header(16);
    rom.read(reinterpret_cast<char *>(header.data()), header.size());

    // Check if ROM is valid (NES<EOF>)
    if (header[0] != 0x4E || header[1] != 0x45 || header[2] != 0x53 || header[3] != 0x1A)
    {
        throw std::runtime_error("Invalid ROM");
    }

    // Get ROM info
    int prg_rom_size = header[4] * 16384;
    int chr_rom_size = header[5] * 8192;
    int mapper = ((header[6] >> 4) | (header[7] & 0xF0));

    // Check if ROM is supported
    if (mapper != 0)
    {
        throw std::runtime_error("Unsupported mapper");
    }

    // Read PRG ROM
    std::vector<uint8_t> prg_rom(prg_rom_size);
    rom.read(reinterpret_cast<char *>(prg_rom.data()), prg_rom.size());

    // Get the reset vector
    uint16_t reset_vector = prg_rom[prg_rom_size - 4] | (prg_rom[prg_rom_size - 3] << 8);

    // Load reset vector into memory
    Debug::debug_print("Reset vector: 0x%04X", reset_vector);
    this->reset_vector = reset_vector;

    // Read CHR ROM
    std::vector<uint8_t> chr_rom(chr_rom_size);
    rom.read(reinterpret_cast<char *>(chr_rom.data()), chr_rom.size());

    // Load PRG ROM into cartridge memory
    cartridge.load(prg_rom.data(), prg_rom.size());

    // Load CHR ROM into PPU memory
    ppu.load(chr_rom.data(), chr_rom.size());
}

void Emulator::set_PC_to_reset_vector()
{
    cpu.set_PC(reset_vector);
}

void Emulator::run()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    int cycles_to_run = 0;

    while (!quit)
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        cycles_to_run = (elapsed * CPU::CLOCK_SPEED / 1000);
        quit = window.poll_events();

        if (!paused)
        {
            if (cycles_to_run > 0)
            {
                // run for 1 cycle
                int cycles = cpu.run();
                cycles_to_run -= cycles;

                // run ppu
                ppu.step(cycles * 3);
            }
        }

        // render graphics
        window.render(ppu.get_frame_buffer());
    }
}