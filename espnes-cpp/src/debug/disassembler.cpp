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
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X     %s #$%02X                          ", ins.address, ins.opcode, ins.operand1, ins.mnemonic, ins.operand1);
            break;
        case AddressingMode::ZERO_PAGE:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X     %s $%02X                           ", ins.address, ins.opcode, ins.operand1, ins.mnemonic, ins.operand1);
            break;
        case AddressingMode::ZERO_PAGE_X:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X     %s $%02X,X @ %02X = %02X               ", ins.address, ins.opcode, ins.operand1, ins.mnemonic, ins.operand1, (ins.operand1 + cpu->get_X()) & 0xFF, memory->read((ins.operand1 + cpu->get_X()) & 0xFF));
            break;
        case AddressingMode::ZERO_PAGE_Y:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X     %s $%02X,Y @ %02X = %02X               ", ins.address, ins.opcode, ins.operand1, ins.mnemonic, ins.operand1, (ins.operand1 + cpu->get_Y()) & 0xFF, memory->read((ins.operand1 + cpu->get_Y()) & 0xFF));
            break;
        case AddressingMode::INDIRECT_X:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X     %s ($%02X,X) @ %02X = %02X             ", ins.address, ins.opcode, ins.operand1, ins.mnemonic, ins.operand1, (ins.operand1 + cpu->get_X()) & 0xFF, memory->read((ins.operand1 + cpu->get_X()) & 0xFF));
            break;
        case AddressingMode::INDIRECT_Y:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X     %s ($%02X),Y = %02X @ %02X = %02X        ", ins.address, ins.opcode, ins.operand1, ins.mnemonic, ins.operand1, memory->read(ins.operand1) + cpu->get_Y(), memory->read(ins.operand1) + cpu->get_Y(), memory->read(memory->read(ins.operand1) + cpu->get_Y()));
            break;
        case AddressingMode::RELATIVE:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X     %s $%02X                           ", ins.address, ins.opcode, ins.operand1, ins.mnemonic, ins.operand1);
            break;
        case AddressingMode::ABSOLUTE:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X %02X  %s $%02X%02X  = %02X                   ", ins.address, ins.opcode, ins.operand1, ins.operand2, ins.mnemonic, ins.operand2, ins.operand1, memory->read((ins.operand2 << 8) | ins.operand1));
            break;
        case AddressingMode::ABSOLUTE_X:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X %02X  %s $%02X%02X,X @ %02X%02X = %02X           ", ins.address, ins.opcode, ins.operand1, ins.operand2, ins.mnemonic, ins.operand2, ins.operand1, ins.operand2, ins.operand1, memory->read((ins.operand2 << 8) | ins.operand1));
            break;
        case AddressingMode::ABSOLUTE_Y:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X %02X  %s $%02X%02X,Y @ %02X%02X = %02X           ", ins.address, ins.opcode, ins.operand1, ins.operand2, ins.mnemonic, ins.operand2, ins.operand1, ins.operand2, ins.operand1, memory->read((ins.operand2 << 8) | ins.operand1));
            break;
        case AddressingMode::INDIRECT:
            sprintf_s(buffer, bufferSize, "%04X  %02X %02X %02X  %s ($%02X%02X) = %02X                  ", ins.address, ins.opcode, ins.operand1, ins.operand2, ins.mnemonic, ins.operand2, ins.operand1, memory->read((ins.operand2 << 8) | ins.operand1));
            break;
        case AddressingMode::ACCUMULATOR:
            sprintf_s(buffer, bufferSize, "%04X  %02X        %s A                             ", ins.address, ins.opcode, ins.mnemonic);
            break;
        case AddressingMode::IMPLIED:
        default:
            sprintf_s(buffer, bufferSize, "%04X  %02X        %s                               ", ins.address, ins.opcode, ins.mnemonic);
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