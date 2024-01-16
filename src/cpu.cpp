#include "../include/cpu.hpp"
#include "../include/interrupt.hpp"

CPU::CPU(Memory *memory) : memory(memory)
{
    total_cycles = 0;
    PC = 0;
    SP = 0xFD;
    A = 0;
    X = 0;
    Y = 0;
    P = 0x24;
    interrupt = InterruptType::NONE;

    // initialize LUT
    ins_table[0x00] = Instructions::brk_impl;
    ins_table[0x01] = Instructions::ora_x_ind;
    ins_table[0x05] = Instructions::ora_zpg;
    ins_table[0x06] = Instructions::asl_zpg;
    ins_table[0x08] = Instructions::php_impl;
    ins_table[0x09] = Instructions::ora_imm;
    ins_table[0x0A] = Instructions::asl_a;
    ins_table[0x0D] = Instructions::ora_abs;
    ins_table[0x0E] = Instructions::asl_abs;
    ins_table[0x10] = Instructions::bpl_rel;
    ins_table[0x11] = Instructions::ora_ind_y;
    ins_table[0x15] = Instructions::ora_zpg_x;
    ins_table[0x16] = Instructions::asl_zpg_x;
    ins_table[0x18] = Instructions::clc_impl;
    ins_table[0x19] = Instructions::ora_abs_y;
    ins_table[0x1D] = Instructions::ora_abs_x;
    ins_table[0x1E] = Instructions::asl_abs_x;
    ins_table[0x20] = Instructions::jsr_abs;
    ins_table[0x21] = Instructions::and_x_ind;
    ins_table[0x24] = Instructions::bit_zpg;
    ins_table[0x25] = Instructions::and_zpg;
    ins_table[0x26] = Instructions::rol_zpg;
    ins_table[0x28] = Instructions::plp_impl;
    ins_table[0x29] = Instructions::and_imm;
    ins_table[0x2A] = Instructions::rol_a;
    ins_table[0x2C] = Instructions::bit_abs;
    ins_table[0x2D] = Instructions::and_abs;
    ins_table[0x2E] = Instructions::rol_abs;
    ins_table[0x30] = Instructions::bmi_rel;
    ins_table[0x31] = Instructions::and_ind_y;
    ins_table[0x35] = Instructions::and_zpg_x;
    ins_table[0x36] = Instructions::rol_zpg_x;
    ins_table[0x38] = Instructions::sec_impl;
    ins_table[0x39] = Instructions::and_abs_y;
    ins_table[0x3D] = Instructions::and_abs_x;
    ins_table[0x3E] = Instructions::rol_abs_x;
    ins_table[0x40] = Instructions::rti_impl;
    ins_table[0x41] = Instructions::eor_x_ind;
    ins_table[0x45] = Instructions::eor_zpg;
    ins_table[0x46] = Instructions::lsr_zpg;
    ins_table[0x48] = Instructions::pha_impl;
    ins_table[0x49] = Instructions::eor_imm;
    ins_table[0x4A] = Instructions::lsr_a;
    ins_table[0x4C] = Instructions::jmp_abs;
    ins_table[0x4D] = Instructions::eor_abs;
    ins_table[0x4E] = Instructions::lsr_abs;
    ins_table[0x50] = Instructions::bvc_rel;
    ins_table[0x51] = Instructions::eor_ind_y;
    ins_table[0x55] = Instructions::eor_zpg_x;
    ins_table[0x56] = Instructions::lsr_zpg_x;
    ins_table[0x58] = Instructions::cli_impl;
    ins_table[0x59] = Instructions::eor_abs_y;
    ins_table[0x5D] = Instructions::eor_abs_x;
    ins_table[0x5E] = Instructions::lsr_abs_x;
    ins_table[0x60] = Instructions::rts_impl;
    ins_table[0x61] = Instructions::adc_x_ind;
    ins_table[0x65] = Instructions::adc_zpg;
    ins_table[0x66] = Instructions::ror_zpg;
    ins_table[0x68] = Instructions::pla_impl;
    ins_table[0x69] = Instructions::adc_imm;
    ins_table[0x6A] = Instructions::ror_a;
    ins_table[0x6C] = Instructions::jmp_ind;
    ins_table[0x6D] = Instructions::adc_abs;
    ins_table[0x6E] = Instructions::ror_abs;
    ins_table[0x70] = Instructions::bvs_rel;
    ins_table[0x71] = Instructions::adc_ind_y;
    ins_table[0x75] = Instructions::adc_zpg_x;
    ins_table[0x76] = Instructions::ror_zpg_x;
    ins_table[0x78] = Instructions::sei_impl;
    ins_table[0x79] = Instructions::adc_abs_y;
    ins_table[0x7D] = Instructions::adc_abs_x;
    ins_table[0x7E] = Instructions::ror_abs_x;
    ins_table[0x81] = Instructions::sta_x_ind;
    ins_table[0x84] = Instructions::sty_zpg;
    ins_table[0x85] = Instructions::sta_zpg;
    ins_table[0x86] = Instructions::stx_zpg;
    ins_table[0x88] = Instructions::dey_impl;
    ins_table[0x8A] = Instructions::txa_impl;
    ins_table[0x8C] = Instructions::sty_abs;
    ins_table[0x8D] = Instructions::sta_abs;
    ins_table[0x8E] = Instructions::stx_abs;
    ins_table[0x90] = Instructions::bcc_rel;
    ins_table[0x91] = Instructions::sta_ind_y;
    ins_table[0x94] = Instructions::sty_zpg_x;
    ins_table[0x95] = Instructions::sta_zpg_x;
    ins_table[0x96] = Instructions::stx_zpg_y;
    ins_table[0x98] = Instructions::tya_impl;
    ins_table[0x99] = Instructions::sta_abs_y;
    ins_table[0x9A] = Instructions::txs_impl;
    ins_table[0x9D] = Instructions::sta_abs_x;
    ins_table[0xA0] = Instructions::ldy_imm;
    ins_table[0xA1] = Instructions::lda_x_ind;
    ins_table[0xA2] = Instructions::ldx_imm;
    ins_table[0xA4] = Instructions::ldy_zpg;
    ins_table[0xA5] = Instructions::lda_zpg;
    ins_table[0xA6] = Instructions::ldx_zpg;
    ins_table[0xA8] = Instructions::tay_impl;
    ins_table[0xA9] = Instructions::lda_imm;
    ins_table[0xAA] = Instructions::tax_impl;
    ins_table[0xAC] = Instructions::ldy_abs;
    ins_table[0xAD] = Instructions::lda_abs;
    ins_table[0xAE] = Instructions::ldx_abs;
    ins_table[0xB0] = Instructions::bcs_rel;
    ins_table[0xB1] = Instructions::lda_ind_y;
    ins_table[0xB4] = Instructions::ldy_zpg_x;
    ins_table[0xB5] = Instructions::lda_zpg_x;
    ins_table[0xB6] = Instructions::ldx_zpg_y;
    ins_table[0xB8] = Instructions::clv_impl;
    ins_table[0xB9] = Instructions::lda_abs_y;
    ins_table[0xBA] = Instructions::tsx_impl;
    ins_table[0xBC] = Instructions::ldy_abs_x;
    ins_table[0xBD] = Instructions::lda_abs_x;
    ins_table[0xBE] = Instructions::ldx_abs_y;
    ins_table[0xC0] = Instructions::cpy_imm;
    ins_table[0xC1] = Instructions::cmp_x_ind;
    ins_table[0xC4] = Instructions::cpy_zpg;
    ins_table[0xC5] = Instructions::cmp_zpg;
    ins_table[0xC6] = Instructions::dec_zpg;
    ins_table[0xC8] = Instructions::iny_impl;
    ins_table[0xC9] = Instructions::cmp_imm;
    ins_table[0xCA] = Instructions::dex_impl;
    ins_table[0xCC] = Instructions::cpy_abs;
    ins_table[0xCD] = Instructions::cmp_abs;
    ins_table[0xCE] = Instructions::dec_abs;
    ins_table[0xD0] = Instructions::bne_rel;
    ins_table[0xD1] = Instructions::cmp_ind_y;
    ins_table[0xD5] = Instructions::cmp_zpg_x;
    ins_table[0xD6] = Instructions::dec_zpg_x;
    ins_table[0xD8] = Instructions::cld_impl;
    ins_table[0xD9] = Instructions::cmp_abs_y;
    ins_table[0xDD] = Instructions::cmp_abs_x;
    ins_table[0xDE] = Instructions::dec_abs_x;
    ins_table[0xE0] = Instructions::cpx_imm;
    ins_table[0xE1] = Instructions::sbc_x_ind;
    ins_table[0xE4] = Instructions::cpx_zpg;
    ins_table[0xE5] = Instructions::sbc_zpg;
    ins_table[0xE6] = Instructions::inc_zpg;
    ins_table[0xE8] = Instructions::inx_impl;
    ins_table[0xE9] = Instructions::sbc_imm;
    ins_table[0xEA] = Instructions::nop_impl;
    ins_table[0xEC] = Instructions::cpx_abs;
    ins_table[0xED] = Instructions::sbc_abs;
    ins_table[0xEE] = Instructions::inc_abs;
    ins_table[0xF0] = Instructions::beq_rel;
    ins_table[0xF1] = Instructions::sbc_ind_y;
    ins_table[0xF5] = Instructions::sbc_zpg_x;
    ins_table[0xF6] = Instructions::inc_zpg_x;
    ins_table[0xF8] = Instructions::sed_impl;
    ins_table[0xF9] = Instructions::sbc_abs_y;
    ins_table[0xFD] = Instructions::sbc_abs_x;
    ins_table[0xFE] = Instructions::inc_abs_x;
}

CPU::~CPU()
{
}

void CPU::reset()
{
    PC = memory->read(RESET_VECTOR) | (memory->read(RESET_VECTOR + 1) << 8);
    SP = 0xFD;
    A = 0;
    X = 0;
    Y = 0;
    P = 0x24;
    interrupt = InterruptType::NONE;
    total_cycles = 7;
}

int CPU::get_cycles()
{
    return total_cycles;
}

int CPU::run()
{
    uint8_t irq_cycles = 0;

    // Check for interrupts
    if (interrupt != InterruptType::NONE)
    {
        irq_cycles = Interrupt::handle_interrupt(interrupt, this, memory);
        this->interrupt = InterruptType::NONE;
    }

    // Log opcode
    // CPUHelpers::log_cpu_status(this, memory, memory->read(PC));

    // Fetch opcode
    uint8_t opcode = fetch_opcode();

    // Decode opcode
    Instructions::InstructionFunction ins = ins_table[opcode];

    // Check for illegal opcodes
    CPUHelpers::check_for_illegal_opcode(opcode);

    if (ins != nullptr)
    {
        uint8_t ins_cycles = ins(this, memory);
        long cycles = ins_cycles + irq_cycles;
        total_cycles += cycles;
        return cycles;
    }

    return 0;
}

uint8_t CPU::get_current_opcode()
{
    return memory->read(PC);
}

uint8_t CPU::fetch_opcode()
{
    return memory->read(PC++);
}

void CPU::set_interrupt(InterruptType type)
{
    this->interrupt = type;
}

uint16_t CPU::get_PC()
{
    return PC;
}

uint8_t CPU::get_SP()
{
    return SP;
}

uint8_t CPU::get_A()
{
    return A;
}

uint8_t CPU::get_X()
{
    return X;
}

uint8_t CPU::get_Y()
{
    return Y;
}

uint8_t CPU::get_P()
{
    return P;
}

void CPU::set_PC(uint16_t value)
{
    PC = value;
}

void CPU::set_SP(uint8_t value)
{
    SP = value;
}

void CPU::set_A(uint8_t value)
{
    A = value;
}

void CPU::set_X(uint8_t value)
{
    X = value;
}

void CPU::set_Y(uint8_t value)
{
    Y = value;
}

void CPU::set_P(uint8_t value)
{
    P = value;
}

void CPU::set_Z(bool value)
{
    if (value)
    {
        P |= FLAG_ZERO;
    }
    else
    {
        P &= ~FLAG_ZERO;
    }
}

void CPU::set_N(bool value)
{
    if (value)
    {
        P |= FLAG_NEGATIVE;
    }
    else
    {
        P &= ~FLAG_NEGATIVE;
    }
}

void CPU::set_C(bool value)
{
    if (value)
    {
        P |= FLAG_CARRY;
    }
    else
    {
        P &= ~FLAG_CARRY;
    }
}

void CPU::set_I(bool value)
{
    if (value)
    {
        P |= FLAG_INTERRUPT_DISABLE;
    }
    else
    {
        P &= ~FLAG_INTERRUPT_DISABLE;
    }
}

void CPU::set_D(bool value)
{
    if (value)
    {
        P |= FLAG_DECIMAL;
    }
    else
    {
        P &= ~FLAG_DECIMAL;
    }
}

void CPU::set_B(bool value)
{
    if (value)
    {
        P |= FLAG_BREAK;
    }
    else
    {
        P &= ~FLAG_BREAK;
    }
}

void CPU::set_U(bool value)
{
    if (value)
    {
        P |= FLAG_UNUSED;
    }
    else
    {
        P &= ~FLAG_UNUSED;
    }
}

void CPU::set_V(bool value)
{
    if (value)
    {
        P |= FLAG_OVERFLOW;
    }
    else
    {
        P &= ~FLAG_OVERFLOW;
    }
}

bool CPU::get_C()
{
    return (P & FLAG_CARRY) != 0;
}

bool CPU::get_Z()
{
    return (P & FLAG_ZERO) != 0;
}

bool CPU::get_I()
{
    return (P & FLAG_INTERRUPT_DISABLE) != 0;
}

bool CPU::get_D()
{
    return (P & FLAG_DECIMAL) != 0;
}

bool CPU::get_B()
{
    return (P & FLAG_BREAK) != 0;
}

bool CPU::get_U()
{
    return (P & FLAG_UNUSED) != 0;
}

bool CPU::get_V()
{
    return (P & FLAG_OVERFLOW) != 0;
}

bool CPU::get_N()
{
    return (P & FLAG_NEGATIVE) != 0;
}