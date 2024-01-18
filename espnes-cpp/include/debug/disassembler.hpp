#ifndef DISASSEEMBLER_HPP
#define DISASSEEMBLER_HPP

#include <cstdint>

class CPU;
class Memory;

class Disassembler
{
public:
    Disassembler(CPU *cpu, Memory *memory);
    ~Disassembler();
    void get_opcode_info_str(char* buffer, size_t bufferSize);

    enum AddressingMode
    {
        ABSOLUTE,
        ABSOLUTE_X,
        ABSOLUTE_Y,
        ACCUMULATOR,
        IMMEDIATE,
        IMPLIED,
        INDIRECT,
        INDIRECT_X,
        INDIRECT_Y,
        RELATIVE,
        ZERO_PAGE,
        ZERO_PAGE_X,
        ZERO_PAGE_Y
    };

    typedef struct
    {
        uint16_t address;
        uint8_t opcode;
        uint8_t operand1;
        uint8_t operand2;
        uint8_t bytes[2];
        uint16_t length;
        const char *mnemonic;
        AddressingMode addressingMode;
    } Instruction;

    struct OpcodeInfo
    {
        const char *mnemonic;
        AddressingMode addressingMode;
        int bytes;
        int cycles;
    };

    Instruction disassemble(uint16_t address);

private:
    static const int LOG_SIZE = 1000;
    Instruction instructionLog[LOG_SIZE];
    int logIndex = 0;
    CPU *cpu;
    Memory *memory;

    OpcodeInfo instructionTable[256] = {
        {"BRK", AddressingMode::IMPLIED, 1, 7},     // 0x00
        {"ORA", AddressingMode::INDIRECT_X, 2, 6},  // 0x01
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x02
        {"SLO", AddressingMode::INDIRECT_X, 2, 8},  // 0x03
        {"NOP", AddressingMode::ZERO_PAGE, 2, 3},   // 0x04
        {"ORA", AddressingMode::ZERO_PAGE, 2, 3},   // 0x05
        {"ASL", AddressingMode::ZERO_PAGE, 2, 5},   // 0x06
        {"SLO", AddressingMode::ZERO_PAGE, 2, 5},   // 0x07
        {"PHP", AddressingMode::IMPLIED, 1, 3},     // 0x08
        {"ORA", AddressingMode::IMMEDIATE, 2, 2},   // 0x09
        {"ASL", AddressingMode::ACCUMULATOR, 1, 2}, // 0x0A
        {"ANC", AddressingMode::IMMEDIATE, 2, 2},   // 0x0B
        {"NOP", AddressingMode::ABSOLUTE, 3, 4},    // 0x0C
        {"ORA", AddressingMode::ABSOLUTE, 3, 4},    // 0x0D
        {"ASL", AddressingMode::ABSOLUTE, 3, 6},    // 0x0E
        {"SLO", AddressingMode::ABSOLUTE, 3, 6},    // 0x0F
        {"BPL", AddressingMode::RELATIVE, 2, 2},    // 0x10
        {"ORA", AddressingMode::INDIRECT_Y, 2, 5},  // 0x11
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x12
        {"SLO", AddressingMode::INDIRECT_Y, 2, 8},  // 0x13
        {"NOP", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x14
        {"ORA", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x15
        {"ASL", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0x16
        {"SLO", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0x17
        {"CLC", AddressingMode::IMPLIED, 1, 2},     // 0x18
        {"ORA", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0x19
        {"NOP", AddressingMode::IMPLIED, 1, 2},     // 0x1A
        {"SLO", AddressingMode::ABSOLUTE_Y, 3, 7},  // 0x1B
        {"NOP", AddressingMode::ABSOLUTE_X, 3, 4},  // 0x1C
        {"ORA", AddressingMode::ABSOLUTE_X, 3, 4},  // 0x1D
        {"ASL", AddressingMode::ABSOLUTE_X, 3, 7},  // 0x1E
        {"SLO", AddressingMode::ABSOLUTE_X, 3, 7},  // 0x1F
        {"JSR", AddressingMode::ABSOLUTE, 3, 6},    // 0x20
        {"AND", AddressingMode::INDIRECT_X, 2, 6},  // 0x21
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x22
        {"RLA", AddressingMode::INDIRECT_X, 2, 8},  // 0x23
        {"BIT", AddressingMode::ZERO_PAGE, 2, 3},   // 0x24
        {"AND", AddressingMode::ZERO_PAGE, 2, 3},   // 0x25
        {"ROL", AddressingMode::ZERO_PAGE, 2, 5},   // 0x26
        {"RLA", AddressingMode::ZERO_PAGE, 2, 5},   // 0x27
        {"PLP", AddressingMode::IMPLIED, 1, 4},     // 0x28
        {"AND", AddressingMode::IMMEDIATE, 2, 2},   // 0x29
        {"ROL", AddressingMode::ACCUMULATOR, 1, 2}, // 0x2A
        {"ANC", AddressingMode::IMMEDIATE, 2, 2},   // 0x2B
        {"BIT", AddressingMode::ABSOLUTE, 3, 4},    // 0x2C
        {"AND", AddressingMode::ABSOLUTE, 3, 4},    // 0x2D
        {"ROL", AddressingMode::ABSOLUTE, 3, 6},    // 0x2E
        {"RLA", AddressingMode::ABSOLUTE, 3, 6},    // 0x2F
        {"BMI", AddressingMode::RELATIVE, 2, 2},    // 0x30
        {"AND", AddressingMode::INDIRECT_Y, 2, 5},  // 0x31
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x32
        {"RLA", AddressingMode::INDIRECT_Y, 2, 8},  // 0x33
        {"NOP", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x34
        {"AND", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x35
        {"ROL", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0x36
        {"RLA", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0x37
        {"SEC", AddressingMode::IMPLIED, 1, 2},     // 0x38
        {"AND", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0x39
        {"NOP", AddressingMode::IMPLIED, 1, 2},     // 0x3A
        {"RLA", AddressingMode::ABSOLUTE_Y, 3, 7},  // 0x3B
        {"NOP", AddressingMode::ABSOLUTE_X, 3, 4},  // 0x3C
        {"AND", AddressingMode::ABSOLUTE_X, 3, 4},  // 0x3D
        {"ROL", AddressingMode::ABSOLUTE_X, 3, 7},  // 0x3E
        {"RLA", AddressingMode::ABSOLUTE_X, 3, 7},  // 0x3F
        {"RTI", AddressingMode::IMPLIED, 1, 6},     // 0x40
        {"EOR", AddressingMode::INDIRECT_X, 2, 6},  // 0x41
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x42
        {"SRE", AddressingMode::INDIRECT_X, 2, 8},  // 0x43
        {"NOP", AddressingMode::ZERO_PAGE, 2, 3},   // 0x44
        {"EOR", AddressingMode::ZERO_PAGE, 2, 3},   // 0x45
        {"LSR", AddressingMode::ZERO_PAGE, 2, 5},   // 0x46
        {"SRE", AddressingMode::ZERO_PAGE, 2, 5},   // 0x47
        {"PHA", AddressingMode::IMPLIED, 1, 3},     // 0x48
        {"EOR", AddressingMode::IMMEDIATE, 2, 2},   // 0x49
        {"LSR", AddressingMode::ACCUMULATOR, 1, 2}, // 0x4A
        {"ALR", AddressingMode::IMMEDIATE, 2, 2},   // 0x4B
        {"JMP", AddressingMode::ABSOLUTE, 3, 3},    // 0x4C
        {"EOR", AddressingMode::ABSOLUTE, 3, 4},    // 0x4D
        {"LSR", AddressingMode::ABSOLUTE, 3, 6},    // 0x4E
        {"SRE", AddressingMode::ABSOLUTE, 3, 6},    // 0x4F
        {"BVC", AddressingMode::RELATIVE, 2, 2},    // 0x50
        {"EOR", AddressingMode::INDIRECT_Y, 2, 5},  // 0x51
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x52
        {"SRE", AddressingMode::INDIRECT_Y, 2, 8},  // 0x53
        {"NOP", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x54
        {"EOR", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x55
        {"LSR", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0x56
        {"SRE", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0x57
        {"CLI", AddressingMode::IMPLIED, 1, 2},     // 0x58
        {"EOR", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0x59
        {"NOP", AddressingMode::IMPLIED, 1, 2},     // 0x5A
        {"SRE", AddressingMode::ABSOLUTE_Y, 3, 7},  // 0x5B
        {"NOP", AddressingMode::ABSOLUTE_X, 3, 4},  // 0x5C
        {"EOR", AddressingMode::ABSOLUTE_X, 3, 4},  // 0x5D
        {"LSR", AddressingMode::ABSOLUTE_X, 3, 7},  // 0x5E
        {"SRE", AddressingMode::ABSOLUTE_X, 3, 7},  // 0x5F
        {"RTS", AddressingMode::IMPLIED, 1, 6},     // 0x60
        {"ADC", AddressingMode::INDIRECT_X, 2, 6},  // 0x61
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x62
        {"RRA", AddressingMode::INDIRECT_X, 2, 8},  // 0x63
        {"NOP", AddressingMode::ZERO_PAGE, 2, 3},   // 0x64
        {"ADC", AddressingMode::ZERO_PAGE, 2, 3},   // 0x65
        {"ROR", AddressingMode::ZERO_PAGE, 2, 5},   // 0x66
        {"RRA", AddressingMode::ZERO_PAGE, 2, 5},   // 0x67
        {"PLA", AddressingMode::IMPLIED, 1, 4},     // 0x68
        {"ADC", AddressingMode::IMMEDIATE, 2, 2},   // 0x69
        {"ROR", AddressingMode::ACCUMULATOR, 1, 2}, // 0x6A
        {"ARR", AddressingMode::IMMEDIATE, 2, 2},   // 0x6B
        {"JMP", AddressingMode::INDIRECT, 3, 5},    // 0x6C
        {"ADC", AddressingMode::ABSOLUTE, 3, 4},    // 0x6D
        {"ROR", AddressingMode::ABSOLUTE, 3, 6},    // 0x6E
        {"RRA", AddressingMode::ABSOLUTE, 3, 6},    // 0x6F
        {"BVS", AddressingMode::RELATIVE, 2, 2},    // 0x70
        {"ADC", AddressingMode::INDIRECT_Y, 2, 5},  // 0x71
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x72
        {"RRA", AddressingMode::INDIRECT_Y, 2, 8},  // 0x73
        {"NOP", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x74
        {"ADC", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x75
        {"ROR", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0x76
        {"RRA", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0x77
        {"SEI", AddressingMode::IMPLIED, 1, 2},     // 0x78
        {"ADC", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0x79
        {"NOP", AddressingMode::IMPLIED, 1, 2},     // 0x7A
        {"RRA", AddressingMode::ABSOLUTE_Y, 3, 7},  // 0x7B
        {"NOP", AddressingMode::ABSOLUTE_X, 3, 4},  // 0x7C
        {"ADC", AddressingMode::ABSOLUTE_X, 3, 4},  // 0x7D
        {"ROR", AddressingMode::ABSOLUTE_X, 3, 7},  // 0x7E
        {"RRA", AddressingMode::ABSOLUTE_X, 3, 7},  // 0x7F
        {"NOP", AddressingMode::IMMEDIATE, 2, 2},   // 0x80
        {"STA", AddressingMode::INDIRECT_X, 2, 6},  // 0x81
        {"NOP", AddressingMode::IMMEDIATE, 2, 2},   // 0x82
        {"SAX", AddressingMode::INDIRECT_X, 2, 6},  // 0x83
        {"STY", AddressingMode::ZERO_PAGE, 2, 3},   // 0x84
        {"STA", AddressingMode::ZERO_PAGE, 2, 3},   // 0x85
        {"STX", AddressingMode::ZERO_PAGE, 2, 3},   // 0x86
        {"SAX", AddressingMode::ZERO_PAGE, 2, 3},   // 0x87
        {"DEY", AddressingMode::IMPLIED, 1, 2},     // 0x88
        {"NOP", AddressingMode::IMMEDIATE, 2, 2},   // 0x89
        {"TXA", AddressingMode::IMPLIED, 1, 2},     // 0x8A
        {"XAA", AddressingMode::IMMEDIATE, 2, 2},   // 0x8B
        {"STY", AddressingMode::ABSOLUTE, 3, 4},    // 0x8C
        {"STA", AddressingMode::ABSOLUTE, 3, 4},    // 0x8D
        {"STX", AddressingMode::ABSOLUTE, 3, 4},    // 0x8E
        {"SAX", AddressingMode::ABSOLUTE, 3, 4},    // 0x8F
        {"BCC", AddressingMode::RELATIVE, 2, 2},    // 0x90
        {"STA", AddressingMode::INDIRECT_Y, 2, 6},  // 0x91
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0x92
        {"AHX", AddressingMode::INDIRECT_Y, 2, 6},  // 0x93
        {"STY", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x94
        {"STA", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0x95
        {"STX", AddressingMode::ZERO_PAGE_Y, 2, 4}, // 0x96
        {"SAX", AddressingMode::ZERO_PAGE_Y, 2, 4}, // 0x97
        {"TYA", AddressingMode::IMPLIED, 1, 2},     // 0x98
        {"STA", AddressingMode::ABSOLUTE_Y, 3, 5},  // 0x99
        {"TXS", AddressingMode::IMPLIED, 1, 2},     // 0x9A
        {"TAS", AddressingMode::ABSOLUTE_Y, 3, 5},  // 0x9B
        {"SHY", AddressingMode::ABSOLUTE_X, 3, 5},  // 0x9C
        {"STA", AddressingMode::ABSOLUTE_X, 3, 5},  // 0x9D
        {"SHX", AddressingMode::ABSOLUTE_Y, 3, 5},  // 0x9E
        {"AHX", AddressingMode::ABSOLUTE_Y, 3, 5},  // 0x9F
        {"LDY", AddressingMode::IMMEDIATE, 2, 2},   // 0xA0
        {"LDA", AddressingMode::INDIRECT_X, 2, 6},  // 0xA1
        {"LDX", AddressingMode::IMMEDIATE, 2, 2},   // 0xA2
        {"LAX", AddressingMode::INDIRECT_X, 2, 6},  // 0xA3
        {"LDY", AddressingMode::ZERO_PAGE, 2, 3},   // 0xA4
        {"LDA", AddressingMode::ZERO_PAGE, 2, 3},   // 0xA5
        {"LDX", AddressingMode::ZERO_PAGE, 2, 3},   // 0xA6
        {"LAX", AddressingMode::ZERO_PAGE, 2, 3},   // 0xA7
        {"TAY", AddressingMode::IMPLIED, 1, 2},     // 0xA8
        {"LDA", AddressingMode::IMMEDIATE, 2, 2},   // 0xA9
        {"TAX", AddressingMode::IMPLIED, 1, 2},     // 0xAA
        {"LAX", AddressingMode::IMMEDIATE, 2, 2},   // 0xAB
        {"LDY", AddressingMode::ABSOLUTE, 3, 4},    // 0xAC
        {"LDA", AddressingMode::ABSOLUTE, 3, 4},    // 0xAD
        {"LDX", AddressingMode::ABSOLUTE, 3, 4},    // 0xAE
        {"LAX", AddressingMode::ABSOLUTE, 3, 4},    // 0xAF
        {"BCS", AddressingMode::RELATIVE, 2, 2},    // 0xB0
        {"LDA", AddressingMode::INDIRECT_Y, 2, 5},  // 0xB1
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0xB2
        {"LAX", AddressingMode::INDIRECT_Y, 2, 5},  // 0xB3
        {"LDY", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0xB4
        {"LDA", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0xB5
        {"LDX", AddressingMode::ZERO_PAGE_Y, 2, 4}, // 0xB6
        {"LAX", AddressingMode::ZERO_PAGE_Y, 2, 4}, // 0xB7
        {"CLV", AddressingMode::IMPLIED, 1, 2},     // 0xB8
        {"LDA", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0xB9
        {"TSX", AddressingMode::IMPLIED, 1, 2},     // 0xBA
        {"LAS", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0xBB
        {"LDY", AddressingMode::ABSOLUTE_X, 3, 4},  // 0xBC
        {"LDA", AddressingMode::ABSOLUTE_X, 3, 4},  // 0xBD
        {"LDX", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0xBE
        {"LAX", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0xBF
        {"CPY", AddressingMode::IMMEDIATE, 2, 2},   // 0xC0
        {"CMP", AddressingMode::INDIRECT_X, 2, 6},  // 0xC1
        {"NOP", AddressingMode::IMMEDIATE, 2, 2},   // 0xC2
        {"DCP", AddressingMode::INDIRECT_X, 2, 8},  // 0xC3
        {"CPY", AddressingMode::ZERO_PAGE, 2, 3},   // 0xC4
        {"CMP", AddressingMode::ZERO_PAGE, 2, 3},   // 0xC5
        {"DEC", AddressingMode::ZERO_PAGE, 2, 5},   // 0xC6
        {"DCP", AddressingMode::ZERO_PAGE, 2, 5},   // 0xC7
        {"INY", AddressingMode::IMPLIED, 1, 2},     // 0xC8
        {"CMP", AddressingMode::IMMEDIATE, 2, 2},   // 0xC9
        {"DEX", AddressingMode::IMPLIED, 1, 2},     // 0xCA
        {"AXS", AddressingMode::IMMEDIATE, 2, 2},   // 0xCB
        {"CPY", AddressingMode::ABSOLUTE, 3, 4},    // 0xCC
        {"CMP", AddressingMode::ABSOLUTE, 3, 4},    // 0xCD
        {"DEC", AddressingMode::ABSOLUTE, 3, 6},    // 0xCE
        {"DCP", AddressingMode::ABSOLUTE, 3, 6},    // 0xCF
        {"BNE", AddressingMode::RELATIVE, 2, 2},    // 0xD0
        {"CMP", AddressingMode::INDIRECT_Y, 2, 5},  // 0xD1
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0xD2
        {"DCP", AddressingMode::INDIRECT_Y, 2, 8},  // 0xD3
        {"NOP", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0xD4
        {"CMP", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0xD5
        {"DEC", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0xD6
        {"DCP", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0xD7
        {"CLD", AddressingMode::IMPLIED, 1, 2},     // 0xD8
        {"CMP", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0xD9
        {"NOP", AddressingMode::IMPLIED, 1, 2},     // 0xDA
        {"DCP", AddressingMode::ABSOLUTE_Y, 3, 7},  // 0xDB
        {"NOP", AddressingMode::ABSOLUTE_X, 3, 4},  // 0xDC
        {"CMP", AddressingMode::ABSOLUTE_X, 3, 4},  // 0xDD
        {"DEC", AddressingMode::ABSOLUTE_X, 3, 7},  // 0xDE
        {"DCP", AddressingMode::ABSOLUTE_X, 3, 7},  // 0xDF
        {"CPX", AddressingMode::IMMEDIATE, 2, 2},   // 0xE0
        {"SBC", AddressingMode::INDIRECT_X, 2, 6},  // 0xE1
        {"NOP", AddressingMode::IMMEDIATE, 2, 2},   // 0xE2
        {"ISC", AddressingMode::INDIRECT_X, 2, 8},  // 0xE3
        {"CPX", AddressingMode::ZERO_PAGE, 2, 3},   // 0xE4
        {"SBC", AddressingMode::ZERO_PAGE, 2, 3},   // 0xE5
        {"INC", AddressingMode::ZERO_PAGE, 2, 5},   // 0xE6
        {"ISC", AddressingMode::ZERO_PAGE, 2, 5},   // 0xE7
        {"INX", AddressingMode::IMPLIED, 1, 2},     // 0xE8
        {"SBC", AddressingMode::IMMEDIATE, 2, 2},   // 0xE9
        {"NOP", AddressingMode::IMPLIED, 1, 2},     // 0xEA
        {"SBC", AddressingMode::IMMEDIATE, 2, 2},   // 0xEB
        {"CPX", AddressingMode::ABSOLUTE, 3, 4},    // 0xEC
        {"SBC", AddressingMode::ABSOLUTE, 3, 4},    // 0xED
        {"INC", AddressingMode::ABSOLUTE, 3, 6},    // 0xEE
        {"ISC", AddressingMode::ABSOLUTE, 3, 6},    // 0xEF
        {"BEQ", AddressingMode::RELATIVE, 2, 2},    // 0xF0
        {"SBC", AddressingMode::INDIRECT_Y, 2, 5},  // 0xF1
        {"KIL", AddressingMode::IMPLIED, 0, 0},     // 0xF2
        {"ISC", AddressingMode::INDIRECT_Y, 2, 8},  // 0xF3
        {"NOP", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0xF4
        {"SBC", AddressingMode::ZERO_PAGE_X, 2, 4}, // 0xF5
        {"INC", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0xF6
        {"ISC", AddressingMode::ZERO_PAGE_X, 2, 6}, // 0xF7
        {"SED", AddressingMode::IMPLIED, 1, 2},     // 0xF8
        {"SBC", AddressingMode::ABSOLUTE_Y, 3, 4},  // 0xF9
        {"NOP", AddressingMode::IMPLIED, 1, 2},     // 0xFA
        {"ISC", AddressingMode::ABSOLUTE_Y, 3, 7},  // 0xFB
        {"NOP", AddressingMode::ABSOLUTE_X, 3, 4},  // 0xFC
        {"SBC", AddressingMode::ABSOLUTE_X, 3, 4},  // 0xFD
        {"INC", AddressingMode::ABSOLUTE_X, 3, 7},  // 0xFE
        {"ISC", AddressingMode::ABSOLUTE_X, 3, 7}   // 0xFF
    };
};

#endif
