#include "../include/emulator.hpp"

Emulator::Emulator() : ppu(), apu(), window(), cartridge(), memory(&ppu, &apu, &cartridge), cpu(&memory), quit(false), paused(false)
{
}

Emulator::~Emulator()
{
}

void Emulator::load_rom(const std::string &romPath)
{
    // TODO move this to a separate class
    FILE *rom = fopen(romPath.c_str(), "rb");
    if (rom == NULL)
    {
        std::cout << "Error: Unable to open ROM file" << std::endl;
        exit(1);
    }

    unsigned char header[16];
    fread(header, sizeof(unsigned char), 16, rom);

    // Check if ROM is valid (NES<EOF>)
    if (header[0] != 0x4E || header[1] != 0x45 || header[2] != 0x53 || header[3] != 0x1A)
    {
        std::cout << "Error: Invalid ROM file" << std::endl;
        exit(1);
    }

    // Get ROM info
    int prg_rom_size = header[4] * 16384;
    int chr_rom_size = header[5] * 8192;
    int mapper = ((header[6] & 0xF0) >> 4) | (header[7] & 0xF0);

    // Check if ROM is supported
    if (mapper != 0)
    {
        std::cout << "Error: Unsupported mapper" << std::endl;
        exit(1);
    }

    // Read PRG ROM
    unsigned char *prg_rom = (unsigned char *)malloc(prg_rom_size);
    fread(prg_rom, sizeof(unsigned char), prg_rom_size, rom);

    // Read CHR ROM
    unsigned char *chr_rom = (unsigned char *)malloc(chr_rom_size);
    fread(chr_rom, sizeof(unsigned char), chr_rom_size, rom);

    // Load PRG ROM into CPU memory
    memory.load(prg_rom, prg_rom_size);

    // Load PGR ROM into cartridge memory
    cartridge.load(prg_rom, prg_rom_size);

    // Load CHR ROM into PPU memory
    ppu.load(chr_rom, chr_rom_size);
}

void Emulator::set_PC_to_reset_vector()
{
    cpu.set_PC(memory.read(CPU::RESET_VECTOR) | (memory.read(CPU::RESET_VECTOR + 1) << 8));
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
            while (cycles_to_run > 0)
            {
                // run for 1 cycle
                int cycles = cpu.run();
                cycles_to_run -= cycles;
            }
        }

        // render graphics
        window.render();
    }
}