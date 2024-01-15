#include "../include/cpu_helpers.hpp"

void CPUHelpers::push_to_stack8(CPU *cpu, Memory *memory, uint8_t value)
{
    memory->write(cpu->get_SP(), value);
    cpu->set_SP(cpu->get_SP() - 1);
}

void CPUHelpers::push_to_stack16(CPU *cpu, Memory *memory, uint16_t value)
{
    uint8_t low_byte = value & 0x00FF;
    uint8_t high_byte = (value & 0xFF00) >> 8;

    memory->write(cpu->get_SP(), high_byte);
    cpu->set_SP(cpu->get_SP() - 1);
    memory->write(cpu->get_SP(), low_byte);
    cpu->set_SP(cpu->get_SP() - 1);
}

uint8_t CPUHelpers::pop_from_stack8(CPU *cpu, Memory *memory)
{
    cpu->set_SP(cpu->get_SP() + 1);
    return memory->read(cpu->get_SP());
}

uint16_t CPUHelpers::pop_from_stack16(CPU *cpu, Memory *memory)
{
    cpu->set_SP(cpu->get_SP() + 1);
    uint8_t low_byte = memory->read(cpu->get_SP());
    cpu->set_SP(cpu->get_SP() + 1);
    uint8_t high_byte = memory->read(cpu->get_SP());

    return (high_byte << 8) | low_byte;
}