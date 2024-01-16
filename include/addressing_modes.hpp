#ifndef ADDRESSING_MODES_HPP
#define ADDRESSING_MODES_HPP

#include <cstdint>

class CPU;
class Memory;

class AddressingModes
{
public:
    static uint16_t absolute(CPU *cpu, Memory *memory);
    static uint16_t absolute_x(CPU *cpu, Memory *memory, bool *page_crossed);
    static uint16_t absolute_y(CPU *cpu, Memory *memory, bool *page_crossed);
    static uint8_t immediate(CPU *cpu, Memory *memory);
    static uint16_t indirect(CPU *cpu, Memory *memory);
    static uint16_t indirect_x(CPU *cpu, Memory *memory);
    static uint16_t indirect_y(CPU *cpu, Memory *memory, bool *page_crossed);
    static int8_t relative(CPU *cpu, Memory *memory);
    static uint8_t zero_page(CPU *cpu, Memory *memory);
    static uint8_t zero_page_x(CPU *cpu, Memory *memory);
    static uint8_t zero_page_y(CPU *cpu, Memory *memory);
    static uint8_t accumulator(CPU *cpu, Memory *memory);
    static uint8_t implied(CPU *cpu, Memory *memory);

private:
};

#endif
