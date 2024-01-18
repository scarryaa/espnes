#include <string>
#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include "../include/debug/disassembler.hpp"

Disassembler::Disassembler(CPU* cpu, Memory* memory)
{
    this->cpu = cpu;
    this->memory = memory;
}

Disassembler::~Disassembler()
{
}

void Disassembler::get_opcode_info_str(char* buffer, size_t bufferSize) {
    auto ins = disassemble(cpu->get_PC());

    switch (ins.addressingMode) {
    case AddressingMode::IMMEDIATE:
    case AddressingMode::ZERO_PAGE:
    case AddressingMode::ZERO_PAGE_X:
    case AddressingMode::ZERO_PAGE_Y:
    case AddressingMode::INDIRECT_X:
    case AddressingMode::INDIRECT_Y:
    case AddressingMode::RELATIVE:
        snprintf(buffer, bufferSize, "%04X  %02X %02X     %s", ins.address, ins.opcode, ins.operand1, ins.mnemonic);
        break;
    case AddressingMode::ABSOLUTE:
    case AddressingMode::ABSOLUTE_X:
    case AddressingMode::ABSOLUTE_Y:
    case AddressingMode::INDIRECT:
        snprintf(buffer, bufferSize, "%04X  %02X %02X %02X  %s", ins.address, ins.opcode, ins.operand1, ins.operand2, ins.mnemonic);
        break;
    case AddressingMode::ACCUMULATOR:
    case AddressingMode::IMPLIED:
    default:
        snprintf(buffer, bufferSize, "%04X  %02X        %s", ins.address, ins.opcode, ins.mnemonic);
        break;
    }
}

Disassembler::Instruction Disassembler::disassemble(uint16_t address)
{
    uint8_t opcode = memory->read(address);
    OpcodeInfo instruction = instructionTable[opcode];

    Instruction result;
    result.address = address;
    result.opcode = opcode;
    result.operand1 = memory->read(address + 1);
    result.operand2 = memory->read(address + 2);
    result.mnemonic = instruction.mnemonic;
    result.addressingMode = instruction.addressingMode;
    result.length = instruction.bytes;

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
        result.bytes[1] = ' ';
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
    default:
        result.bytes[0] = ' ';
        result.bytes[1] = ' ';
        // No additional bytes to read
        break;
    }

    return result;
}