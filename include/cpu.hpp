#ifndef CPU_HPP
#define CPU_HPP

#include <functional>
#include <cstdint>
#include "../include/memory.hpp"
#include "../include/instructions.hpp"
#include "../include/interrupt_type.hpp"
#include "../include/cpu_helpers.hpp"
#include "../include/debug/disassembler.hpp"

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
    void reset();

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
    void set_Z(bool value);
    void set_N(bool value);
    void set_C(bool value);
    void set_I(bool value);
    void set_D(bool value);
    void set_B(bool value);
    void set_U(bool value);
    void set_V(bool value);

    // Flags
    bool get_C();
    bool get_Z();
    bool get_I();
    bool get_D();
    bool get_B();
    bool get_U();
    bool get_V();
    bool get_N();

    // Constants
    static const int CLOCK_SPEED = 1789773;         // 1.789773 MHz
    static const int FLAG_CARRY = 0x01;             // Bit 0
    static const int FLAG_ZERO = 0x02;              // Bit 1
    static const int FLAG_INTERRUPT_DISABLE = 0x04; // Bit 2
    static const int FLAG_DECIMAL = 0x08;           // Bit 3
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