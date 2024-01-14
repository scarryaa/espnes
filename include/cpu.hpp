#ifndef CPU_HPP
#define CPU_HPP

#include <functional>
#include <cstdint>
#include "../include/memory.hpp"
#include "../include/instructions.hpp"
#include "../include/interrupt_type.hpp"

class Interrupt;

class CPU
{
public:
    CPU(Memory *memory);
    ~CPU();

    int get_cycles();
    int run();
    void set_interrupt(InterruptType type);
    uint8_t fetch_opcode();

    // Getters
    uint16_t get_PC();
    uint8_t get_SP();
    uint8_t get_A();
    uint8_t get_X();
    uint8_t get_Y();
    uint8_t get_P();

    // Setters
    void set_PC(uint16_t value);
    void set_SP(uint8_t value);
    void set_A(uint8_t value);
    void set_X(uint8_t value);
    void set_Y(uint8_t value);
    void set_P(uint8_t value);

    // Constants
    static const int CLOCK_SPEED = 1789773;         // 1.789773 MHz
    static const int FLAG_CARRY = 0x01;             // Bit 0
    static const int FLAG_ZERO = 0x02;              // Bit 1
    static const int FLAG_INTERRUPT_DISABLE = 0x04; // Bit 2
    static const int FLAG_DECIMAL_MODE = 0x08;      // Bit 3
    static const int FLAG_BREAK = 0x10;             // Bit 4
    static const int FLAG_UNUSED = 0x20;            // Bit 5
    static const int FLAG_OVERFLOW = 0x40;          // Bit 6
    static const int FLAG_NEGATIVE = 0x80;          // Bit 7
    static const int NMI_VECTOR = 0xFFFA;
    static const int RESET_VECTOR = 0xFFFC;
    static const int IRQ_VECTOR = 0xFFFE;

private:
    int cycles;
    long total_cycles;
    Instructions::InstructionFunction ins_table[256];

    // Registers
    uint16_t PC; // Program Counter
    uint8_t SP;  // Stack Pointer
    uint8_t A;   // Accumulator
    uint8_t X;   // Index Register X
    uint8_t Y;   // Index Register Y
    uint8_t P;   // Processor Status

    // Interrupts
    InterruptType interrupt;

    // Memory
    Memory *memory;
};

#endif