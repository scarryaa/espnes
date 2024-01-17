#include "../include/cpu.hpp"
#include "../include/cpu_helpers.hpp"
#include "../include/instructions.hpp"

// 0x00
uint8_t Instructions::brk_impl(CPU* cpu, Memory* memory)
{
    // Fetch additional opcode for brk reason
    cpu->fetch_opcode();

    // set interrupt, rest of the work is done in interrupt handler
    cpu->set_interrupt(InterruptType::BRK);

    // returns 0 because the interrupt handler will return 7 cycles
    return 0;
}

// 0x01
uint8_t Instructions::ora_x_ind(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Get value at the address
    uint8_t val = memory->read(addr);

    // OR value with A
    cpu->set_A(cpu->get_A() | val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 6;
}

// 0x05
uint8_t Instructions::ora_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // OR value with A
    cpu->set_A(cpu->get_A() | val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 3;
}

// 0x06
uint8_t Instructions::asl_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Shift value left
    uint8_t result = val << 1;

    // Write result to memory
    memory->write(zpg_addr, result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 5;
}

// 0x08
uint8_t Instructions::php_impl(CPU* cpu, Memory* memory)
{
    // Push P onto stack with B and U flags set
    CPUHelpers::push_to_stack8(cpu, memory, cpu->get_P() | CPU::FLAG_BREAK | CPU::FLAG_UNUSED);

    return 3;
}

// 0x09
uint8_t Instructions::ora_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // OR value with A
    cpu->set_A(cpu->get_A() | val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 2;
}

// 0x0A
uint8_t Instructions::asl_a(CPU* cpu, Memory* memory)
{
    // Get A
    uint8_t val = cpu->get_A();

    // Shift value left
    uint8_t result = val << 1;

    // Write result to A
    cpu->set_A(result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 2;
}

// 0x0D
uint8_t Instructions::ora_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // OR value with A
    cpu->set_A(cpu->get_A() | val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0x0E
uint8_t Instructions::asl_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Shift value left
    uint8_t result = val << 1;

    // Write result to memory
    memory->write(addr, result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

uint8_t Instructions::bpl_rel(CPU* cpu, Memory* memory)
{
    // Get relative address
    int8_t offset = AddressingModes::relative(cpu, memory);
    uint16_t originalPC = cpu->get_PC();
    uint16_t addr = originalPC + offset;

    // Check if negative flag is clear
    if (!cpu->get_N())
    {
        // Branch
        cpu->set_PC(addr);

        // Add cycles if page boundary is crossed
        if ((originalPC & 0xFF00) != (addr & 0xFF00))
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }

    return 2;
}


// 0x11
uint8_t Instructions::ora_ind_y(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Add Y register to address
    addr += cpu->get_Y();

    // Get value at the address
    uint8_t val = memory->read(addr);

    // OR value with A
    cpu->set_A(cpu->get_A() | val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        return 6;
    }

    return 5;
}

// 0x15
uint8_t Instructions::ora_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // OR value with A
    cpu->set_A(cpu->get_A() | val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0x16
uint8_t Instructions::asl_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Shift value left
    uint8_t result = val << 1;

    // Write result to memory
    memory->write(zpg_addr, result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

// 0x18
uint8_t Instructions::clc_impl(CPU* cpu, Memory* memory)
{
    // Clear C flag
    cpu->set_C(false);

    return 2;
}

// 0x19
uint8_t Instructions::ora_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;

    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // OR value with A
    cpu->set_A(cpu->get_A() | val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0x1D
uint8_t Instructions::ora_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;

    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // OR value with A
    cpu->set_A(cpu->get_A() | val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0x1E
uint8_t Instructions::asl_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;

    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Shift value left
    uint8_t result = val << 1;

    // Write result to memory
    memory->write(addr, result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 7;
    }

    return 6;
}

// 0x20
uint8_t Instructions::jsr_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Push address of next instruction onto stack
    CPUHelpers::push_to_stack16(cpu, memory, cpu->get_PC() - 1);

    // Set PC to address
    cpu->set_PC(addr);

    return 6;
}

// 0x21
uint8_t Instructions::and_x_ind(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Get value at the address
    uint8_t val = memory->read(addr);

    // AND value with A
    cpu->set_A(cpu->get_A() & val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 6;
}

// 0x24
uint8_t Instructions::bit_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // AND value with A
    uint8_t result = cpu->get_A() & val;

    // Set flags
    cpu->set_Z(result == 0);
    cpu->set_N(val & 0x80);
    cpu->set_V(val & 0x40);

    return 3;
}

// 0x25
uint8_t Instructions::and_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // AND value with A
    cpu->set_A(cpu->get_A() & val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 3;
}

// 0x26
uint8_t Instructions::rol_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Shift value left
    uint8_t result = (val << 1) | cpu->get_C();

    // Write result to memory
    memory->write(zpg_addr, result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 5;
}

// 0x28
uint8_t Instructions::plp_impl(CPU* cpu, Memory* memory)
{
    // Pull P from stack
    cpu->set_P(CPUHelpers::pop_from_stack8(cpu, memory));

    // Clear B and set U flag
    cpu->set_B(false);
    cpu->set_U(true);

    return 4;
}

// 0x29
uint8_t Instructions::and_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // AND value with A
    cpu->set_A(cpu->get_A() & val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 2;
}

// 0x2A
uint8_t Instructions::rol_a(CPU* cpu, Memory* memory)
{
    // Get A
    uint8_t val = cpu->get_A();

    // Shift value left
    uint8_t result = (val << 1) | cpu->get_C();

    // Write result to A
    cpu->set_A(result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 2;
}

// 0x2C
uint8_t Instructions::bit_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // AND value with A
    uint8_t result = cpu->get_A() & val;

    // Set flags
    cpu->set_Z(result == 0);
    cpu->set_N(val & 0x80);
    cpu->set_V(val & 0x40);

    return 4;
}

// 0x2D
uint8_t Instructions::and_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // AND value with A
    cpu->set_A(cpu->get_A() & val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0x2E
uint8_t Instructions::rol_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Shift value left
    uint8_t result = (val << 1) | cpu->get_C();

    // Write result to memory
    memory->write(addr, result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

// 0x30
uint8_t Instructions::bmi_rel(CPU* cpu, Memory* memory)
{
    // Get relative address
    int8_t offset = AddressingModes::relative(cpu, memory);
    uint16_t addr = cpu->get_PC() + offset;

    // Check if negative flag is set
    if (cpu->get_N())
    {
        // Branch
        cpu->set_PC(addr);

        // Add cycles if page boundary is crossed
        if ((cpu->get_PC() & 0xFF00) != (addr & 0xFF00))
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }

    return 2;
}

// 0x31
uint8_t Instructions::and_ind_y(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Add Y register to address
    addr += cpu->get_Y();

    // Get value at the address
    uint8_t val = memory->read(addr);

    // AND value with A
    cpu->set_A(cpu->get_A() & val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        return 6;
    }

    return 5;
}

// 0x35
uint8_t Instructions::and_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // AND value with A
    cpu->set_A(cpu->get_A() & val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0x36
uint8_t Instructions::rol_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Shift value left
    uint8_t result = (val << 1) | cpu->get_C();

    // Write result to memory
    memory->write(zpg_addr, result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

// 0x38
uint8_t Instructions::sec_impl(CPU* cpu, Memory* memory)
{
    // Set C flag
    cpu->set_C(true);

    return 2;
}

// 0x39
uint8_t Instructions::and_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // AND value with A
    cpu->set_A(cpu->get_A() & val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0x3D
uint8_t Instructions::and_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // AND value with A
    cpu->set_A(cpu->get_A() & val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0x3E
uint8_t Instructions::rol_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Shift value left
    uint8_t result = (val << 1) | cpu->get_C();

    // Write result to memory
    memory->write(addr, result);

    // Set flags
    cpu->set_C(val & 0x80);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 7;
    }

    return 6;
}

// 0x40
uint8_t Instructions::rti_impl(CPU* cpu, Memory* memory)
{
    // Pull P from stack
    cpu->set_P(CPUHelpers::pop_from_stack8(cpu, memory));

    // Pull PC from stack
    cpu->set_PC(CPUHelpers::pop_from_stack16(cpu, memory));

    return 6;
}

// 0x41
uint8_t Instructions::eor_x_ind(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Get value at the address
    uint8_t val = memory->read(addr);

    // XOR value with A
    cpu->set_A(cpu->get_A() ^ val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 6;
}

// 0x45
uint8_t Instructions::eor_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // XOR value with A
    cpu->set_A(cpu->get_A() ^ val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 3;
}

// 0x46
uint8_t Instructions::lsr_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Shift value right
    uint8_t result = val >> 1;

    // Write result to memory
    memory->write(zpg_addr, result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 5;
}

// 0x48
uint8_t Instructions::pha_impl(CPU* cpu, Memory* memory)
{
    // Push A onto stack
    CPUHelpers::push_to_stack8(cpu, memory, cpu->get_A());

    return 3;
}

// 0x49
uint8_t Instructions::eor_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // XOR value with A
    cpu->set_A(cpu->get_A() ^ val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 2;
}

// 0x4A
uint8_t Instructions::lsr_a(CPU* cpu, Memory* memory)
{
    // Get A
    uint8_t val = cpu->get_A();

    // Shift value right
    uint8_t result = val >> 1;

    // Write result to A
    cpu->set_A(result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 2;
}

// 0x4C
uint8_t Instructions::jmp_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Set PC to address
    cpu->set_PC(addr);

    return 3;
}

// 0x4D
uint8_t Instructions::eor_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // XOR value with A
    cpu->set_A(cpu->get_A() ^ val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0x4E
uint8_t Instructions::lsr_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Shift value right
    uint8_t result = val >> 1;

    // Write result to memory
    memory->write(addr, result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

// 0x50
uint8_t Instructions::bvc_rel(CPU* cpu, Memory* memory)
{
    // Get relative address
    int8_t offset = AddressingModes::relative(cpu, memory);
    uint16_t addr = cpu->get_PC() + offset;

    // Check if overflow flag is clear
    if (!cpu->get_V())
    {
        // Branch
        cpu->set_PC(addr);

        // Add cycles if page boundary is crossed
        if ((cpu->get_PC() & 0xFF00) != (addr & 0xFF00))
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }

    return 2;
}

// 0x51
uint8_t Instructions::eor_ind_y(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Add Y register to address
    addr += cpu->get_Y();

    // Get value at the address
    uint8_t val = memory->read(addr);

    // XOR value with A
    cpu->set_A(cpu->get_A() ^ val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        return 6;
    }

    return 5;
}

// 0x55
uint8_t Instructions::eor_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // XOR value with A
    cpu->set_A(cpu->get_A() ^ val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0x56
uint8_t Instructions::lsr_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Shift value right
    uint8_t result = val >> 1;

    // Write result to memory
    memory->write(zpg_addr, result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

// 0x58
uint8_t Instructions::cli_impl(CPU* cpu, Memory* memory)
{
    // Clear I flag
    cpu->set_I(false);

    return 2;
}

// 0x59
uint8_t Instructions::eor_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // XOR value with A
    cpu->set_A(cpu->get_A() ^ val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0x5D
uint8_t Instructions::eor_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // XOR value with A
    cpu->set_A(cpu->get_A() ^ val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0x5E
uint8_t Instructions::lsr_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Shift value right
    uint8_t result = val >> 1;

    // Write result to memory
    memory->write(addr, result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 7;
    }

    return 6;
}

// 0x60
uint8_t Instructions::rts_impl(CPU* cpu, Memory* memory)
{
    // Pull PC from stack
    cpu->set_PC(CPUHelpers::pop_from_stack16(cpu, memory));

    // Increment PC
    cpu->set_PC(cpu->get_PC() + 1);

    return 6;
}

// 0x61
uint8_t Instructions::adc_x_ind(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Add value to A
    uint16_t result = cpu->get_A() + val + cpu->get_C();

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result > 0xFF);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 6;
}

// 0x65
uint8_t Instructions::adc_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Add value to A
    uint16_t result = cpu->get_A() + val + cpu->get_C();

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result > 0xFF);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 3;
}

// 0x66
uint8_t Instructions::ror_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Rotate value right
    uint8_t result = (val >> 1) | (cpu->get_C() << 7);

    // Write result to memory
    memory->write(zpg_addr, result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 5;
}

// 0x68
uint8_t Instructions::pla_impl(CPU* cpu, Memory* memory)
{
    // Pull A from stack
    cpu->set_A(CPUHelpers::pop_from_stack8(cpu, memory));

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0x69
uint8_t Instructions::adc_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // Add value to A
    uint16_t result = cpu->get_A() + val + cpu->get_C();

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result > 0xFF);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 2;
}

// 0x6A
uint8_t Instructions::ror_a(CPU* cpu, Memory* memory)
{
    // Get A
    uint8_t val = cpu->get_A();

    // Rotate value right
    uint8_t result = (val >> 1) | (cpu->get_C() << 7);

    // Write result to A
    cpu->set_A(result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 2;
}

// 0x6C
uint8_t Instructions::jmp_ind(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::indirect(cpu, memory);

    // Set PC to address
    cpu->set_PC(addr);

    return 5;
}

// 0x6D
uint8_t Instructions::adc_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Add value to A
    uint16_t result = cpu->get_A() + val + cpu->get_C();

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result > 0xFF);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 4;
}

// 0x6E
uint8_t Instructions::ror_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Rotate value right
    uint8_t result = (val >> 1) | (cpu->get_C() << 7);

    // Write result to memory
    memory->write(addr, result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

// 0x70
uint8_t Instructions::bvs_rel(CPU* cpu, Memory* memory)
{
    // Get relative address
    int8_t offset = AddressingModes::relative(cpu, memory);
    uint16_t addr = cpu->get_PC() + offset;

    // Check if overflow flag is set
    if (cpu->get_V())
    {
        // Branch
        cpu->set_PC(addr);

        // Add cycles if page boundary is crossed
        if ((cpu->get_PC() & 0xFF00) != (addr & 0xFF00))
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }

    return 2;
}

// 0x71
uint8_t Instructions::adc_ind_y(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Add Y register to address
    addr += cpu->get_Y();

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Add value to A
    uint16_t result = cpu->get_A() + val + cpu->get_C();

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result > 0xFF);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        return 6;
    }

    return 5;
}

// 0x75
uint8_t Instructions::adc_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Add value to A
    uint16_t result = cpu->get_A() + val + cpu->get_C();

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result > 0xFF);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 4;
}

// 0x76
uint8_t Instructions::ror_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Rotate value right
    uint8_t result = (val >> 1) | (cpu->get_C() << 7);

    // Write result to memory
    memory->write(zpg_addr, result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

// 0x78
uint8_t Instructions::sei_impl(CPU* cpu, Memory* memory)
{
    // Set I flag
    cpu->set_I(true);

    return 2;
}

// 0x79
uint8_t Instructions::adc_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Add value to A
    uint16_t result = cpu->get_A() + val + cpu->get_C();

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result > 0xFF);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0x7D
uint8_t Instructions::adc_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Add value to A
    uint16_t result = cpu->get_A() + val + cpu->get_C();

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result > 0xFF);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0x7E
uint8_t Instructions::ror_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Rotate value right
    uint8_t result = (val >> 1) | (cpu->get_C() << 7);

    // Write result to memory
    memory->write(addr, result);

    // Set flags
    cpu->set_C(val & 0x01);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 7;
    }

    return 6;
}

// 0x81
uint8_t Instructions::sta_x_ind(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Store A at the address
    memory->write(addr, cpu->get_A());

    return 6;
}

// 0x84
uint8_t Instructions::sty_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Store Y at the address
    memory->write(zpg_addr, cpu->get_Y());

    return 3;
}

// 0x85
uint8_t Instructions::sta_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Store A at the address
    memory->write(zpg_addr, cpu->get_A());

    return 3;
}

// 0x86
uint8_t Instructions::stx_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Store X at the address
    memory->write(zpg_addr, cpu->get_X());

    return 3;
}

// 0x88
uint8_t Instructions::dey_impl(CPU* cpu, Memory* memory)
{
    // Decrement Y
    cpu->set_Y(cpu->get_Y() - 1);

    // Set flags
    cpu->set_Z(cpu->get_Y() == 0);
    cpu->set_N(cpu->get_Y() & 0x80);

    return 2;
}

// 0x8A
uint8_t Instructions::txa_impl(CPU* cpu, Memory* memory)
{
    // Transfer X to A
    cpu->set_A(cpu->get_X());

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 2;
}

// 0x8C
uint8_t Instructions::sty_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Store Y at the address
    memory->write(addr, cpu->get_Y());

    return 4;
}

// 0x8D
uint8_t Instructions::sta_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Store A at the address
    memory->write(addr, cpu->get_A());

    return 4;
}

// 0x8E
uint8_t Instructions::stx_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Store X at the address
    memory->write(addr, cpu->get_X());

    return 4;
}

// 0x90
uint8_t Instructions::bcc_rel(CPU* cpu, Memory* memory)
{
    // Get relative address
    int8_t offset = AddressingModes::relative(cpu, memory);
    uint16_t addr = cpu->get_PC() + offset;

    // Check if carry flag is clear
    if (!cpu->get_C())
    {
        // Branch
        cpu->set_PC(addr);

        // Add cycles if page boundary is crossed
        if ((cpu->get_PC() & 0xFF00) != (addr & 0xFF00))
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }

    return 2;
}

// 0x91
uint8_t Instructions::sta_ind_y(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Add Y register to address
    addr += cpu->get_Y();

    // Store A at the address
    memory->write(addr, cpu->get_A());

    return 6;
}

// 0x94
uint8_t Instructions::sty_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Store Y at the address
    memory->write(zpg_addr, cpu->get_Y());

    return 4;
}

// 0x95
uint8_t Instructions::sta_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Store A at the address
    memory->write(zpg_addr, cpu->get_A());

    return 4;
}

// 0x96
uint8_t Instructions::stx_zpg_y(CPU* cpu, Memory* memory)
{
    // Get zero page address and add Y register to it
    uint8_t zpg_addr = AddressingModes::zero_page_y(cpu, memory);

    // Store X at the address
    memory->write(zpg_addr, cpu->get_X());

    return 4;
}

// 0x98
uint8_t Instructions::tya_impl(CPU* cpu, Memory* memory)
{
    // Transfer Y to A
    cpu->set_A(cpu->get_Y());

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 2;
}

// 0x99
uint8_t Instructions::sta_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Store A at the address
    memory->write(addr, cpu->get_A());

    return 5;
}

// 0x9A
uint8_t Instructions::txs_impl(CPU* cpu, Memory* memory)
{
    // Transfer X to SP
    cpu->set_SP(cpu->get_X());

    return 2;
}

// 0x9D
uint8_t Instructions::sta_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Store A at the address
    memory->write(addr, cpu->get_A());

    return 5;
}

// 0xA0
uint8_t Instructions::ldy_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // Set Y to value
    cpu->set_Y(val);

    // Set flags
    cpu->set_Z(cpu->get_Y() == 0);
    cpu->set_N(cpu->get_Y() & 0x80);

    return 2;
}

// 0xA1
uint8_t Instructions::lda_x_ind(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set A to value
    cpu->set_A(val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 6;
}

// 0xA2
uint8_t Instructions::ldx_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // Set X to value
    cpu->set_X(val);

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    return 2;
}

// 0xA4
uint8_t Instructions::ldy_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Set Y to value
    cpu->set_Y(val);

    // Set flags
    cpu->set_Z(cpu->get_Y() == 0);
    cpu->set_N(cpu->get_Y() & 0x80);

    return 3;
}

// 0xA5
uint8_t Instructions::lda_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Set A to value
    cpu->set_A(val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 3;
}

// 0xA6
uint8_t Instructions::ldx_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Set X to value
    cpu->set_X(val);

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    return 3;
}

// 0xA8
uint8_t Instructions::tay_impl(CPU* cpu, Memory* memory)
{
    // Transfer A to Y
    cpu->set_Y(cpu->get_A());

    // Set flags
    cpu->set_Z(cpu->get_Y() == 0);
    cpu->set_N(cpu->get_Y() & 0x80);

    return 2;
}

// 0xA9
uint8_t Instructions::lda_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // Set A to value
    cpu->set_A(val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 2;
}

// 0xAA
uint8_t Instructions::tax_impl(CPU* cpu, Memory* memory)
{
    // Transfer A to X
    cpu->set_X(cpu->get_A());

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    return 2;
}

// 0xAC
uint8_t Instructions::ldy_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set Y to value
    cpu->set_Y(val);

    // Set flags
    cpu->set_Z(cpu->get_Y() == 0);
    cpu->set_N(cpu->get_Y() & 0x80);

    return 4;
}

// 0xAD
uint8_t Instructions::lda_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set A to value
    cpu->set_A(val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0xAE
uint8_t Instructions::ldx_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set X to value
    cpu->set_X(val);

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    return 4;
}

// 0xB0
uint8_t Instructions::bcs_rel(CPU* cpu, Memory* memory)
{
    // Get relative address
    int8_t offset = AddressingModes::relative(cpu, memory);
    uint16_t addr = cpu->get_PC() + offset;

    // Check if carry flag is set
    if (cpu->get_C())
    {
        // Branch
        cpu->set_PC(addr);

        // Add cycles if page boundary is crossed
        if ((cpu->get_PC() & 0xFF00) != (addr & 0xFF00))
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }

    return 2;
}

// 0xB1
uint8_t Instructions::lda_ind_y(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Add Y register to address
    addr += cpu->get_Y();

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set A to value
    cpu->set_A(val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        return 6;
    }

    return 5;
}

// 0xB4
uint8_t Instructions::ldy_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Set Y to value
    cpu->set_Y(val);

    // Set flags
    cpu->set_Z(cpu->get_Y() == 0);
    cpu->set_N(cpu->get_Y() & 0x80);

    return 4;
}

// 0xB5
uint8_t Instructions::lda_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Set A to value
    cpu->set_A(val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    return 4;
}

// 0xB6
uint8_t Instructions::ldx_zpg_y(CPU* cpu, Memory* memory)
{
    // Get zero page address and add Y register to it
    uint8_t zpg_addr = AddressingModes::zero_page_y(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Set X to value
    cpu->set_X(val);

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    return 4;
}

// 0xB8
uint8_t Instructions::clv_impl(CPU* cpu, Memory* memory)
{
    // Clear V flag
    cpu->set_V(false);

    return 2;
}

// 0xB9
uint8_t Instructions::lda_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set A to value
    cpu->set_A(val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0xBA
uint8_t Instructions::tsx_impl(CPU* cpu, Memory* memory)
{
    // Transfer SP to X
    cpu->set_X(cpu->get_SP());

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    return 2;
}

// 0xBC
uint8_t Instructions::ldy_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set Y to value
    cpu->set_Y(val);

    // Set flags
    cpu->set_Z(cpu->get_Y() == 0);
    cpu->set_N(cpu->get_Y() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0xBD
uint8_t Instructions::lda_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set A to value
    cpu->set_A(val);

    // Set flags
    cpu->set_Z(cpu->get_A() == 0);
    cpu->set_N(cpu->get_A() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0xBE
uint8_t Instructions::ldx_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Set X to value
    cpu->set_X(val);

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0xC0
uint8_t Instructions::cpy_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // Compare Y to value
    uint8_t result = cpu->get_Y() - val;

    // Set flags
    cpu->set_C(cpu->get_Y() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 2;
}

// 0xC1
uint8_t Instructions::cmp_x_ind(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Compare A to value
    uint8_t result = cpu->get_A() - val;

    // Set flags
    cpu->set_C(cpu->get_A() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 6;
}

// 0xC4
uint8_t Instructions::cpy_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Compare Y to value
    uint8_t result = cpu->get_Y() - val;

    // Set flags
    cpu->set_C(cpu->get_Y() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 3;
}

// 0xC5
uint8_t Instructions::cmp_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Compare A to value
    uint8_t result = cpu->get_A() - val;

    // Set flags
    cpu->set_C(cpu->get_A() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 3;
}

// 0xC6
uint8_t Instructions::dec_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Decrement value
    val--;

    // Write value to memory
    memory->write(zpg_addr, val);

    // Set flags
    cpu->set_Z(val == 0);
    cpu->set_N(val & 0x80);

    return 5;
}

// 0xC8
uint8_t Instructions::iny_impl(CPU* cpu, Memory* memory)
{
    // Increment Y
    cpu->set_Y(cpu->get_Y() + 1);

    // Set flags
    cpu->set_Z(cpu->get_Y() == 0);
    cpu->set_N(cpu->get_Y() & 0x80);

    return 2;
}

// 0xC9
uint8_t Instructions::cmp_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // Compare A to value
    uint8_t result = cpu->get_A() - val;

    // Set flags
    cpu->set_C(cpu->get_A() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 2;
}

// 0xCA
uint8_t Instructions::dex_impl(CPU* cpu, Memory* memory)
{
    // Decrement X
    cpu->set_X(cpu->get_X() - 1);

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    return 2;
}

// 0xCC
uint8_t Instructions::cpy_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Compare Y to value
    uint8_t result = cpu->get_Y() - val;

    // Set flags
    cpu->set_C(cpu->get_Y() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 4;
}

// 0xCD
uint8_t Instructions::cmp_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Compare A to value
    uint8_t result = cpu->get_A() - val;

    // Set flags
    cpu->set_C(cpu->get_A() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 4;
}

// 0xCE
uint8_t Instructions::dec_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Decrement value
    val--;

    // Write value to memory
    memory->write(addr, val);

    // Set flags
    cpu->set_Z(val == 0);
    cpu->set_N(val & 0x80);

    return 6;
}

// 0xD0
uint8_t Instructions::bne_rel(CPU* cpu, Memory* memory)
{
    // Get relative address
    int8_t offset = AddressingModes::relative(cpu, memory);

    uint16_t addr = cpu->get_PC() + offset;

    // Check if zero flag is clear
    if (!cpu->get_Z())
    {
        // Branch
        cpu->set_PC(addr);

        // Add cycles if page boundary is crossed
        if ((cpu->get_PC() & 0xFF00) != (addr & 0xFF00))
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }

    return 2;
}

// 0xD1
uint8_t Instructions::cmp_ind_y(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Add Y register to address
    addr += cpu->get_Y();

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Compare A to value
    uint8_t result = cpu->get_A() - val;

    // Set flags
    cpu->set_C(cpu->get_A() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        return 6;
    }

    return 5;
}

// 0xD5
uint8_t Instructions::cmp_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Compare A to value
    uint8_t result = cpu->get_A() - val;

    // Set flags
    cpu->set_C(cpu->get_A() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 4;
}

// 0xD6
uint8_t Instructions::dec_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Decrement value
    val--;

    // Write value to memory
    memory->write(zpg_addr, val);

    // Set flags
    cpu->set_Z(val == 0);
    cpu->set_N(val & 0x80);

    return 6;
}

// 0xD8
uint8_t Instructions::cld_impl(CPU* cpu, Memory* memory)
{
    // Clear D flag
    cpu->set_D(false);

    return 2;
}

// 0xD9
uint8_t Instructions::cmp_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Compare A to value
    uint8_t result = cpu->get_A() - val;

    // Set flags
    cpu->set_C(cpu->get_A() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0xDD
uint8_t Instructions::cmp_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Compare A to value
    uint8_t result = cpu->get_A() - val;

    // Set flags
    cpu->set_C(cpu->get_A() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0xDE
uint8_t Instructions::dec_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Decrement value
    val--;

    // Write value to memory
    memory->write(addr, val);

    // Set flags
    cpu->set_Z(val == 0);
    cpu->set_N(val & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 7;
    }

    return 6;
}

// 0xE0
uint8_t Instructions::cpx_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // Compare X to value
    uint8_t result = cpu->get_X() - val;

    // Set flags
    cpu->set_C(cpu->get_X() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 2;
}

// 0xE1
uint8_t Instructions::sbc_x_ind(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Subtract value from A
    uint8_t result = cpu->get_A() - val - (cpu->get_C() ? 0 : 1);

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result <= cpu->get_A());
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 6;
}

// 0xE4
uint8_t Instructions::cpx_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Compare X to value
    uint8_t result = cpu->get_X() - val;

    // Set flags
    cpu->set_C(cpu->get_X() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 3;
}

// 0xE5
uint8_t Instructions::sbc_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Subtract value from A
    uint8_t result = cpu->get_A() - val - (cpu->get_C() ? 0 : 1);

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result <= cpu->get_A());
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 3;
}

// 0xE6
uint8_t Instructions::inc_zpg(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Increment value
    val++;

    // Write value to memory
    memory->write(zpg_addr, val);

    // Set flags
    cpu->set_Z(val == 0);
    cpu->set_N(val & 0x80);

    return 5;
}

// 0xE8
uint8_t Instructions::inx_impl(CPU* cpu, Memory* memory)
{
    // Increment X
    cpu->set_X(cpu->get_X() + 1);

    // Set flags
    cpu->set_Z(cpu->get_X() == 0);
    cpu->set_N(cpu->get_X() & 0x80);

    return 2;
}

// 0xE9
uint8_t Instructions::sbc_imm(CPU* cpu, Memory* memory)
{
    // Get immediate value
    uint8_t val = AddressingModes::immediate(cpu, memory);

    // Subtract value from A
    uint8_t result = cpu->get_A() - val - (cpu->get_C() ? 0 : 1);

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result <= cpu->get_A());
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 2;
}

// 0xEA
uint8_t Instructions::nop_impl(CPU* cpu, Memory* memory)
{
    // Do nothing

    return 2;
}

// 0xEC
uint8_t Instructions::cpx_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Compare X to value
    uint8_t result = cpu->get_X() - val;

    // Set flags
    cpu->set_C(cpu->get_X() >= val);
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);

    return 4;
}

// 0xED
uint8_t Instructions::sbc_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Subtract value from A
    uint8_t result = cpu->get_A() - val - (cpu->get_C() ? 0 : 1);

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result <= cpu->get_A());
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 4;
}

// 0xEE
uint8_t Instructions::inc_abs(CPU* cpu, Memory* memory)
{
    // Get absolute address
    uint16_t addr = AddressingModes::absolute(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Increment value
    val++;

    // Write value to memory
    memory->write(addr, val);

    // Set flags
    cpu->set_Z(val == 0);
    cpu->set_N(val & 0x80);

    return 6;
}

// 0xF0
uint8_t Instructions::beq_rel(CPU* cpu, Memory* memory)
{
    // Get relative address (offset)
    int8_t offset = AddressingModes::relative(cpu, memory);

    // Base cycles (no branch taken)
    uint8_t cycles = 2;

    // Check if zero flag is set
    if (cpu->get_Z())
    {
        uint16_t oldPC = cpu->get_PC();
        cpu->set_PC(oldPC + offset);

        // Add cycle for taking branch
        cycles++;

        // Add additional cycle if page boundary is crossed
        if ((oldPC & 0xFF00) != (cpu->get_PC() & 0xFF00))
        {
            cycles++;
        }
    }

    return cycles;
}

// 0xF1
uint8_t Instructions::sbc_ind_y(CPU* cpu, Memory* memory)
{
    // Get zero page address
    uint8_t zpg_addr = AddressingModes::zero_page(cpu, memory);

    // Get the 16-bit address stored at the zero page location
    uint8_t lo = memory->read(zpg_addr & 0xFF);
    uint8_t hi = memory->read((zpg_addr + 1) & 0xFF);
    uint16_t addr = (hi << 8) | lo;

    // Add Y register to address
    addr += cpu->get_Y();

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Subtract value from A
    uint8_t result = cpu->get_A() - val - (cpu->get_C() ? 0 : 1);

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result <= cpu->get_A());
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    // Check if page boundary was crossed
    if ((addr & 0xFF00) != (hi << 8))
    {
        return 6;
    }

    return 5;
}

// 0xF5
uint8_t Instructions::sbc_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Subtract value from A
    uint8_t result = cpu->get_A() - val - (cpu->get_C() ? 0 : 1);

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result <= cpu->get_A());
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    return 4;
}

// 0xF6
uint8_t Instructions::inc_zpg_x(CPU* cpu, Memory* memory)
{
    // Get zero page address and add X register to it
    uint8_t zpg_addr = AddressingModes::zero_page_x(cpu, memory);

    // Get value at the address
    uint8_t val = memory->read(zpg_addr);

    // Increment value
    val++;

    // Write value to memory
    memory->write(zpg_addr, val);

    // Set flags
    cpu->set_Z(val == 0);
    cpu->set_N(val & 0x80);

    return 6;
}

// 0xF8
uint8_t Instructions::sed_impl(CPU* cpu, Memory* memory)
{
    // Set D flag
    cpu->set_D(true);

    return 2;
}

// 0xF9
uint8_t Instructions::sbc_abs_y(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add Y register to it
    uint16_t addr = AddressingModes::absolute_y(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Subtract value from A
    uint8_t result = cpu->get_A() - val - (cpu->get_C() ? 0 : 1);

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result <= cpu->get_A());
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0xFD
uint8_t Instructions::sbc_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Subtract value from A
    uint8_t result = cpu->get_A() - val - (cpu->get_C() ? 0 : 1);

    // Set A to result
    cpu->set_A(result);

    // Set flags
    cpu->set_C(result <= cpu->get_A());
    cpu->set_Z(result == 0);
    cpu->set_N(result & 0x80);
    cpu->set_V((cpu->get_A() ^ val) & (cpu->get_A() ^ result) & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 5;
    }

    return 4;
}

// 0xFE
uint8_t Instructions::inc_abs_x(CPU* cpu, Memory* memory)
{
    bool page_crossed = false;
    // Get absolute address and add X register to it
    uint16_t addr = AddressingModes::absolute_x(cpu, memory, &page_crossed);

    // Get value at the address
    uint8_t val = memory->read(addr);

    // Increment value
    val++;

    // Write value to memory
    memory->write(addr, val);

    // Set flags
    cpu->set_Z(val == 0);
    cpu->set_N(val & 0x80);

    // Check if page boundary was crossed
    if (page_crossed)
    {
        return 7;
    }

    return 6;
}