#include "../../include/cpu.hpp"
#include "../../include/memory.hpp"
#include "../../include/debug/disassembler.hpp"

Disassembler::Disassembler(CPU *cpu, Memory *memory)
{
    this->cpu = cpu;
    this->memory = memory;
}

Disassembler::~Disassembler()
{
}

Disassembler::Instruction Disassembler::disassemble(uint16_t address)
{
    uint8_t opcode = memory->read(address);
    OpcodeInfo instruction = instructionTable[opcode];

    Instruction result;
    result.address = address;
    result.opcode = opcode;
    result.mnemonic = instruction.mnemonic;
    result.addressingMode = instruction.addressingMode;

    switch (instruction.addressingMode)
    {
    // Combine cases that have the same logic
    case AddressingMode::IMMEDIATE:
    case AddressingMode::ZERO_PAGE:
    case AddressingMode::ZERO_PAGE_X:
    case AddressingMode::ZERO_PAGE_Y:
    case AddressingMode::INDIRECT_X:
    case AddressingMode::INDIRECT_Y:
    case AddressingMode::RELATIVE:
        result.bytes[0] = memory->read(address + 1);
        break;
    case AddressingMode::ABSOLUTE:
    case AddressingMode::ABSOLUTE_X:
    case AddressingMode::ABSOLUTE_Y:
    case AddressingMode::INDIRECT:
        result.bytes[0] = memory->read(address + 1);
        result.bytes[1] = memory->read(address + 2);
        break;
    case AddressingMode::ACCUMULATOR:
    case AddressingMode::IMPLIED:
        // No additional bytes to read
        break;

    default:
        break;
    }

    return result;
}