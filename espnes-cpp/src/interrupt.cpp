#include "../include/interrupt.hpp"
#include "../include/cpu.hpp"

uint8_t Interrupt::handle_interrupt(InterruptType type, CPU* cpu, Memory* memory)
{
    switch (type)
    {
    case InterruptType::NMI:
        handle_nmi(cpu, memory);
        break;
    case InterruptType::BRK:
        handle_brk(cpu, memory);
        break;
    case InterruptType::IRQ:
        handle_irq(cpu, memory);
        break;
    case InterruptType::RESET:
        handle_reset(cpu, memory);
        break;
    }

    // Interrupts take ~7 cycles to complete
    return 7;
}

void Interrupt::handle_nmi(CPU* cpu, Memory* memory)
{
    uint16_t pc = cpu->get_PC();
    uint8_t p = cpu->get_P() | 0x20;

    // Push high byte of PC
    memory->write(0x0100 + cpu->get_SP(), (pc >> 8) & 0xFF);
    cpu->set_SP(cpu->get_SP() - 1);

    // Push low byte of PC
    memory->write(0x0100 + cpu->get_SP(), pc & 0xFF);
    cpu->set_SP(cpu->get_SP() - 1);

    // Push processor status (P) with a constant
    memory->write(0x0100 + cpu->get_SP(), p);
    cpu->set_SP(cpu->get_SP() - 1);

    // Set PC to NMI vector
    cpu->set_PC(memory->read(CPU::NMI_VECTOR) | (memory->read(CPU::NMI_VECTOR + 1) << 8));
}


void Interrupt::handle_brk(CPU* cpu, Memory* memory)
{
    uint16_t pc = cpu->get_PC() + 1;

    // Disable interrupts
    cpu->set_P(cpu->get_P() | CPU::FLAG_INTERRUPT_DISABLE);

    // Push PC onto stack
    memory->write(0x0100 + cpu->get_SP(), (pc >> 8) & 0xFF);
    cpu->set_SP(cpu->get_SP() - 1);
    memory->write(0x0100 + cpu->get_SP(), pc & 0xFF);
    cpu->set_SP(cpu->get_SP() - 1);

    // Push P onto stack with B and U flags set
    memory->write(0x0100 + cpu->get_SP(), cpu->get_P() | CPU::FLAG_BREAK | CPU::FLAG_UNUSED);
    cpu->set_SP(cpu->get_SP() - 1);

    // Set PC to IRQ
    cpu->set_PC(memory->read(CPU::IRQ_VECTOR) | (memory->read(CPU::IRQ_VECTOR + 1) << 8));
}


void Interrupt::handle_irq(CPU* cpu, Memory* memory)
{
    // Check if interrupts are enabled
    if ((cpu->get_P() & CPU::FLAG_INTERRUPT_DISABLE) == 0)
    {
        // Disable interrupts
        cpu->set_P(cpu->get_P() | CPU::FLAG_INTERRUPT_DISABLE);

        // Push PC onto stack
        memory->write(0x0100 + cpu->get_SP(), (cpu->get_PC() >> 8) & 0xFF);
        cpu->set_SP(cpu->get_SP() - 1);
        memory->write(0x0100 + cpu->get_SP(), cpu->get_PC() & 0xFF);
        cpu->set_SP(cpu->get_SP() - 1);

        // Push P onto stack with B unset
        memory->write(0x0100 + cpu->get_SP(), cpu->get_P() & ~CPU::FLAG_BREAK);
        cpu->set_SP(cpu->get_SP() - 1);

        // Set PC to IRQ vector
        cpu->set_PC(memory->read(CPU::IRQ_VECTOR) | (memory->read(CPU::IRQ_VECTOR + 1) << 8));
    }
}

void Interrupt::handle_reset(CPU* cpu, Memory* memory)
{
    // Set PC to RESET vector
    cpu->set_PC(memory->read(CPU::RESET_VECTOR) | (memory->read(CPU::RESET_VECTOR + 1) << 8));
}