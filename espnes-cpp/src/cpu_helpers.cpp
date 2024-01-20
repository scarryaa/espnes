#include "../include/cpu_helpers.hpp"

void CPUHelpers::push_to_stack8(CPU* cpu, Memory* memory, uint8_t value)
{
    memory->write(0x0100 + cpu->get_SP(), value);
    cpu->set_SP(cpu->get_SP() - 1);
}

void CPUHelpers::push_to_stack16(CPU* cpu, Memory* memory, uint16_t value)
{
    uint8_t high_byte = (value >> 8) & 0xFF;
    uint8_t low_byte = value & 0xFF;

    // Push high byte first, then low byte
    memory->write(0x0100 + cpu->get_SP(), high_byte);
    cpu->set_SP(cpu->get_SP() - 1);
    memory->write(0x0100 + cpu->get_SP(), low_byte);
    cpu->set_SP(cpu->get_SP() - 1);
}

uint8_t CPUHelpers::pop_from_stack8(CPU* cpu, Memory* memory)
{
    cpu->set_SP(cpu->get_SP() + 1);
    return memory->read(0x0100 + cpu->get_SP());
}

uint16_t CPUHelpers::pop_from_stack16(CPU* cpu, Memory* memory)
{
    cpu->set_SP(cpu->get_SP() + 1);
    uint8_t low_byte = memory->read(0x0100 + cpu->get_SP());

    cpu->set_SP(cpu->get_SP() + 1);
    uint8_t high_byte = memory->read(0x0100 + cpu->get_SP());

    return (high_byte << 8) | low_byte;
}


void CPUHelpers::check_for_illegal_opcode(uint8_t opcode)
{
    if (opcode == 0x02 || opcode == 0x12 || opcode == 0x22 || opcode == 0x32 || opcode == 0x42 || opcode == 0x52 || opcode == 0x62 || opcode == 0x72 || opcode == 0x92 || opcode == 0xB2 || opcode == 0xD2 || opcode == 0xF2 || opcode == 0x1A || opcode == 0x3A || opcode == 0x5A || opcode == 0x7A || opcode == 0xDA || opcode == 0xFA || opcode == 0x80 || opcode == 0x82 || opcode == 0x89 || opcode == 0xC2 || opcode == 0xE2 || opcode == 0x04 || opcode == 0x44 || opcode == 0x64 || opcode == 0x14 || opcode == 0x34 || opcode == 0x54 || opcode == 0x74 || opcode == 0xD4 || opcode == 0xF4 || opcode == 0x0C || opcode == 0x1C || opcode == 0x3C || opcode == 0x5C || opcode == 0x7C || opcode == 0xDC || opcode == 0xFC || opcode == 0xA3 || opcode == 0xA7 || opcode == 0xAF || opcode == 0xB3 || opcode == 0xB7 || opcode == 0xBF || opcode == 0x93 || opcode == 0x9F || opcode == 0xC3 || opcode == 0xC7 || opcode == 0xCF || opcode == 0xD3 || opcode == 0xD7 || opcode == 0xDB || opcode == 0xDF || opcode == 0xE3 || opcode == 0xE7 || opcode == 0xEF || opcode == 0xF3 || opcode == 0xF7 || opcode == 0xFB || opcode == 0xFF)
    {
        Debug::debug_print("Illegal opcode: %02X", opcode);
    }
}

uint16_t CPUHelpers::adc(uint8_t op1, uint8_t op2, uint8_t carry)
{
    uint16_t result = op1 + op2 + (carry);
    return result;
}

void CPUHelpers::log_cpu_status(CPU* cpu, Memory* memory, uint8_t opcode)
{
    // Write to file in the format uppercased hex values with leading zeroes
    // PC:0000 SP:00 A:00 X:00 Y:00 P:FFFFFFFF Opcode:00
    std::ofstream log_file;
    log_file.open("log.txt", std::ios_base::app);
    log_file << "PC:" << std::setfill('0') << std::setw(4) << std::uppercase << std::hex << cpu->get_PC() << " ";
    log_file << "SP:" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (int)cpu->get_SP() << " ";
    log_file << "A:" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (int)cpu->get_A() << " ";
    log_file << "X:" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (int)cpu->get_X() << " ";
    log_file << "Y:" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (int)cpu->get_Y() << " ";
    log_file << "P:" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (int)cpu->get_P() << " ";
    log_file << "Opcode:" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (int)opcode << "\n";
    log_file.close();
}