#include "../include/emulator.hpp"

Emulator::Emulator() : cpu(&memory), ppu(), apu(), window(), cartridge(), memory(&ppu, &apu, &cartridge), disassembler(&cpu, &memory), quit(false), paused(false)
{
    ppu.set_cpu(cpu);
}

Emulator::~Emulator()
{
}

uint16_t Emulator::get_PC()
{
    return cpu.get_PC();
}

CPU* Emulator::get_CPU()
{
    return &cpu;
}

PPU* Emulator::get_PPU()
{
    return &ppu;
}

bool Emulator::is_paused()
{
    return paused;
}

void Emulator::pause()
{
    paused = !paused;
}

void Emulator::step()
{
    uint8_t cycles = cpu.run();
    ppu.step(cycles * 3);
}

void Emulator::reset()
{
    cpu.reset();
    ppu.reset();
}

Disassembler Emulator::get_disassembler()
{
    return disassembler;
}

void Emulator::add_breakpoint(breakpoint_type_t type, uint16_t value)
{
    breakpoints.insert({ type, value });
}

void Emulator::clear_breakpoint(breakpoint_type_t type, uint16_t value)
{
	breakpoints.erase({ type, value });
}

void Emulator::clear_all_breakpoints()
{
    breakpoints.clear();
}

bool Emulator::is_breakpoint(breakpoint_type_t type, long value)
{
    for (auto breakpoint : breakpoints)
    {
        if (breakpoint.type == type && breakpoint.address == value)
        {
			return true;
		}
	}
	return false;
}

std::set<Breakpoint> Emulator::get_breakpoints()
{
    return breakpoints;
}

std::set<Breakpoint> Emulator::get_breakpoints_of_type(breakpoint_type_t type)
{
	std::set<Breakpoint> breakpoints_of_type;
    for (auto breakpoint : breakpoints)
    {
        if (breakpoint.type == type)
        {
			breakpoints_of_type.insert(breakpoint);
		}
	}
	return breakpoints_of_type;
}

void Emulator::load_rom(const std::string& romPath)
{
    // Open the file
    std::ifstream rom(romPath, std::ios::binary);
    if (!rom)
    {
        throw std::runtime_error("Failed to open ROM file");
    }

    // Read the header
    std::vector<uint8_t> header(16);
    rom.read(reinterpret_cast<char*>(header.data()), header.size());

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
    rom.read(reinterpret_cast<char*>(prg_rom.data()), prg_rom.size());

    // Get the reset vector
    uint16_t reset_vector = prg_rom[prg_rom_size - 4] | (prg_rom[prg_rom_size - 3] << 8);

    // Load reset vector into memory
    Debug::debug_print("Reset vector: 0x%04X", reset_vector);
    this->reset_vector = reset_vector;

    // Read CHR ROM
    std::vector<uint8_t> chr_rom(chr_rom_size);
    rom.read(reinterpret_cast<char*>(chr_rom.data()), chr_rom.size());

    // Load PRG ROM into cartridge memory
    cartridge.load(prg_rom.data(), prg_rom.size());

    // print out cartridge
    // std::string cartridge_data;
    // Debug::hex_to_string(prg_rom.data(), prg_rom.size(), cartridge_data);
    // Debug::debug_print("PRG ROM: %s", cartridge_data.c_str());

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
        start_time = current_time;
        const float max_elapsed = 1000.0f / 30.0f;

        if (elapsed > max_elapsed)
        {
            elapsed = max_elapsed;
        }
        cycles_to_run = (elapsed * CPU::CLOCK_SPEED / 1000);
        quit = window.poll_events();

        window.render(this);

        uint8_t cycles = 0;
        while (cycles_to_run > 0)
        {
            if (paused)
            {
                break;
            }

            //log cpu
            //log_cpu();

            cycles = cpu.run();

            cycles_to_run -= cycles;

            ppu.step(cycles * 3);

            check_for_breakpoints();
        }

        // render graphics
        window.post_render(ppu.get_frame_buffer());
    }
}

void Emulator::open_log_file()
{
	log_file.open("log.txt", std::ios_base::app);
}

void Emulator::close_log_file()
{
	log_file.close();
}

void Emulator::log_cpu()
{
     /*log in format C009  AD 02 20  LDA $2002 = 80                  A:00 X:FF Y:00 P:A4 SP:FF CYC:15*/
    log_file << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << cpu.get_PC() << "  ";
    log_file << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu.get_current_opcode() << "  ";
    log_file << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)memory.read(cpu.get_PC() + 1) << "  ";
    log_file << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)memory.read(cpu.get_PC() + 2) << "  ";
    log_file << "A:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu.get_A() << " ";
    log_file << "X:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu.get_X() << " ";
    log_file << "Y:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu.get_Y() << " ";
    log_file << "P:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu.get_P() << " ";
    log_file << "SP:" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)cpu.get_SP() << " ";
    log_file << "CYC:" << std::dec << std::uppercase << std::setfill('0') << std::setw(3) << ppu.get_total_cycles() << "\n";
    log_file.flush();
}

void Emulator::check_for_breakpoints()
{
    if (is_breakpoint(BREAKPOINT_TYPE_ADDRESS, cpu.get_PC()))
    {
		pause();
    }
    else if (is_breakpoint(BREAKPOINT_TYPE_CYCLE, cpu.get_total_cycles()))
    {
		pause();
    }
    else if (is_breakpoint(BREAKPOINT_TYPE_SCANLINE, ppu.get_scanline()))
    {
		pause();
	}
}