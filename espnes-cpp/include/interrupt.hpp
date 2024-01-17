#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

#include <cstdint>
#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include "../include/interrupt_type.hpp"

class Interrupt
{
public:
    static uint8_t handle_interrupt(InterruptType type, CPU *cpu, Memory *memory);

private:
    static void handle_nmi(CPU *cpu, Memory *memory);
    static void handle_brk(CPU *cpu, Memory *memory);
    static void handle_irq(CPU *cpu, Memory *memory);
    static void handle_reset(CPU *cpu, Memory *memory);
};

#endif