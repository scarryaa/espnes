#ifndef CPU_HELPERS_HPP
#define CPU_HELPERS_HPP

#include <cstdint>
#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include "../include/addressing_modes.hpp"
#include <iomanip>
#include <fstream>

class CPUHelpers
{
public:
    static void push_to_stack8(CPU *cpu, Memory *memory, uint8_t value);
    static void push_to_stack16(CPU *cpu, Memory *memory, uint16_t value);
    static uint8_t pop_from_stack8(CPU *cpu, Memory *memory);
    static uint16_t pop_from_stack16(CPU *cpu, Memory *memory);
    static void check_for_illegal_opcode(uint8_t opcode);
    static void log_cpu_status(CPU *cpu, Memory *memory, uint8_t opcode);
};

#endif
