#include "../include/interrupt.hpp"
#include "../include/cpu.hpp"

uint8_t Interrupt::handle_interrupt(InterruptType type, CPU *cpu, Memory *memory)
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

void Interrupt::handle_nmi(CPU *cpu, Memory *memory)
{
    // Push PC onto stack
    memory->write(cpu->get_SP(), (cpu->get_PC() >> 8) & 0xFF);
    cpu->set_SP(cpu->get_SP() - 1);
    memory->write(cpu->get_SP(), cpu->get_PC() & 0xFF);
    cpu->set_SP(cpu->get_SP() - 1);

    // Push P onto stack with B flag set
    memory->write(cpu->get_SP(), cpu->get_P() | CPU::FLAG_BREAK);

    // Set PC to NMI vector
    cpu->set_PC(memory->read(CPU::NMI_VECTOR) | (memory->read(CPU::NMI_VECTOR + 1) << 8));
}

void Interrupt::handle_brk(CPU *cpu, Memory *memory)
{
    // Disable interrupts
    cpu->set_P(cpu->get_P() | CPU::FLAG_INTERRUPT_DISABLE);

    // Push PC onto stack
    memory->write(cpu->get_SP(), (cpu->get_PC() >> 8) & 0xFF);
    cpu->set_SP(cpu->get_SP() - 1);
    memory->write(cpu->get_SP(), cpu->get_PC() & 0xFF);
    cpu->set_SP(cpu->get_SP() - 1);

    // Push P onto stack with B and U flags set
    memory->write(cpu->get_SP(), cpu->get_P() | CPU::FLAG_BREAK | CPU::FLAG_UNUSED);

    // Set PC to IRQ vector
    cpu->set_PC(memory->read(CPU::IRQ_VECTOR) | (memory->read(CPU::IRQ_VECTOR + 1) << 8));
}

void Interrupt::handle_irq(CPU *cpu, Memory *memory)
{
    // Check if interrupts are enabled
    if ((cpu->get_P() & CPU::FLAG_INTERRUPT_DISABLE) == 0)
    {
        // Disable interrupts
        cpu->set_P(cpu->get_P() | CPU::FLAG_INTERRUPT_DISABLE);

        // Push PC onto stack
        memory->write(cpu->get_SP(), (cpu->get_PC() >> 8) & 0xFF);
        cpu->set_SP(cpu->get_SP() - 1);
        memory->write(cpu->get_SP(), cpu->get_PC() & 0xFF);
        cpu->set_SP(cpu->get_SP() - 1);

        // Push P onto stack with B unset
        memory->write(cpu->get_SP(), cpu->get_P() & ~CPU::FLAG_BREAK);

        // Set PC to IRQ vector
        cpu->set_PC(memory->read(CPU::IRQ_VECTOR) | (memory->read(CPU::IRQ_VECTOR + 1) << 8));
    }
}

void Interrupt::handle_reset(CPU *cpu, Memory *memory)
{
    // Set PC to RESET vector
    cpu->set_PC(memory->read(CPU::RESET_VECTOR) | (memory->read(CPU::RESET_VECTOR + 1) << 8));
}