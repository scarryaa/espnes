#include "../include/cpu.hpp"
#include "../include/interrupt.hpp"

CPU::CPU(Memory *memory) : memory(memory)
{
    cycles = 0;
    total_cycles = 0;
    PC = 0x8000;
    SP = 0xFD;
    A = 0;
    X = 0;
    Y = 0;
    P = 0x24;

    // initialize LUT
    for (int i = 0; i < 256; i++)
    {
        ins_table[i] = Instructions::nop_impl;
    }
}

CPU::~CPU()
{
}

int CPU::get_cycles()
{
    return cycles;
}

int CPU::run()
{
    uint8_t irq_cycles = 0;

    // Check for interrupts
    if (interrupt != InterruptType::NONE)
    {
        irq_cycles = Interrupt::handle_interrupt(interrupt, this, memory);
        this->interrupt = InterruptType::NONE;
    }

    // Fetch opcode
    uint8_t opcode = fetch_opcode();

    // Decode opcode
    Instructions::InstructionFunction ins = ins_table[opcode];
    if (ins != nullptr)
    {
        uint8_t ins_cycles = ins();
        long cycles = ins_cycles + irq_cycles;
        total_cycles += cycles;
        return cycles;
    }

    return 0;
}

uint8_t CPU::fetch_opcode()
{
    return 0;
}

void CPU::set_interrupt(InterruptType type)
{
    this->interrupt = type;
}

uint16_t CPU::get_PC()
{
    return PC;
}

uint8_t CPU::get_SP()
{
    return SP;
}

uint8_t CPU::get_A()
{
    return A;
}

uint8_t CPU::get_X()
{
    return X;
}

uint8_t CPU::get_Y()
{
    return Y;
}

uint8_t CPU::get_P()
{
    return P;
}

void CPU::set_PC(uint16_t value)
{
    PC = value;
}

void CPU::set_SP(uint8_t value)
{
    SP = value;
}

void CPU::set_A(uint8_t value)
{
    A = value;
}

void CPU::set_X(uint8_t value)
{
    X = value;
}

void CPU::set_Y(uint8_t value)
{
    Y = value;
}

void CPU::set_P(uint8_t value)
{
    P = value;
}