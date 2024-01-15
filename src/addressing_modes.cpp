#include "../include/addressing_modes.hpp"
#include "../include/cpu.hpp"
#include "../include/memory.hpp"

uint8_t AddressingModes::immediate(CPU *cpu, Memory *memory)
{
    // Get immediate value
    return cpu->fetch_opcode();
}

uint8_t AddressingModes::zero_page(CPU *cpu, Memory *memory)
{
    // Get zero page address
    return cpu->fetch_opcode();
}

uint8_t AddressingModes::zero_page_x(CPU *cpu, Memory *memory)
{
    // Get zero page address
    uint8_t addr = cpu->fetch_opcode();
    // Add X register to address
    addr += cpu->get_X();
    // Wrap around if necessary
    addr &= 0xFF;

    return addr;
}

uint8_t AddressingModes::zero_page_y(CPU *cpu, Memory *memory)
{
    // Get zero page address
    uint8_t addr = cpu->fetch_opcode();
    // Add Y register to address
    addr += cpu->get_Y();
    // Wrap around if necessary
    addr &= 0xFF;

    return addr;
}

uint16_t AddressingModes::absolute(CPU *cpu, Memory *memory)
{
    // Get low byte of address
    uint8_t lo = cpu->fetch_opcode();
    // Get high byte of address
    uint8_t hi = cpu->fetch_opcode();
    // Combine bytes to get address
    uint16_t addr = (hi << 8) | lo;
    return addr;
}

uint16_t AddressingModes::absolute_x(CPU *cpu, Memory *memory, bool *page_crossed)
{
    // Get low byte of address
    uint8_t lo = cpu->fetch_opcode();
    // Get high byte of address
    uint8_t hi = cpu->fetch_opcode();
    // Combine bytes to get address
    uint16_t addr = (hi << 8) | lo;
    // Add X register to address
    addr += cpu->get_X();
    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        *page_crossed = true;
    }
    return addr;
}

uint16_t AddressingModes::absolute_y(CPU *cpu, Memory *memory, bool *page_crossed)
{
    // Get low byte of address
    uint8_t lo = cpu->fetch_opcode();
    // Get high byte of address
    uint8_t hi = cpu->fetch_opcode();
    // Combine bytes to get address
    uint16_t addr = (hi << 8) | lo;
    // Add Y register to address
    addr += cpu->get_Y();
    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        *page_crossed = true;
    }
    return addr;
}

uint16_t AddressingModes::indirect(CPU *cpu, Memory *memory)
{
    // Get low byte of address
    uint8_t lo = cpu->fetch_opcode();
    // Get high byte of address
    uint8_t hi = cpu->fetch_opcode();
    // Combine bytes to get address
    uint16_t addr = (hi << 8) | lo;
    // Get low byte of indirect address
    lo = memory->read(addr);
    // Get high byte of indirect address
    hi = memory->read(addr + 1);
    // Combine bytes to get indirect address
    addr = (hi << 8) | lo;
    return addr;
}

uint16_t AddressingModes::indirect_x(CPU *cpu, Memory *memory)
{
    // Get zero page address
    uint8_t addr = cpu->fetch_opcode();
    // Add X register to address
    addr += cpu->get_X();
    // Wrap around if necessary
    addr &= 0xFF;
    // Get low byte of indirect address
    uint8_t lo = memory->read(addr);
    // Get high byte of indirect address
    uint8_t hi = memory->read(addr + 1);
    // Combine bytes to get indirect address
    addr = (hi << 8) | lo;
    return addr;
}

uint16_t AddressingModes::indirect_y(CPU *cpu, Memory *memory, bool *page_crossed)
{
    // Get zero page address
    uint8_t addr = cpu->fetch_opcode();
    // Get low byte of indirect address
    uint8_t lo = memory->read(addr);
    // Get high byte of indirect address
    uint8_t hi = memory->read(addr + 1);
    // Combine bytes to get indirect address
    addr = (hi << 8) | lo;
    // Add Y register to address
    addr += cpu->get_Y();
    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        *page_crossed = true;
    }
    return addr;
}

uint8_t AddressingModes::relative(CPU *cpu, Memory *memory)
{
    // Get relative address
    uint8_t addr = cpu->fetch_opcode();
    // Check if negative
    if (addr & 0x80)
    {
        // Sign extend
        addr |= 0xFF00;
    }
    return addr;
}