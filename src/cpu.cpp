#include "../include/cpu.hpp"

CPU::CPU()
{
    cycles = 0;
    total_cycles = 0;
    // PC = 0x8000;
    // SP = 0xFD;
    // A = 0;
    // X = 0;
    // Y = 0;
    // P = 0x24;
}

CPU::~CPU()
{
}

int CPU::get_cycles()
{
    return cycles;
}

int CPU::run()
{
    cycles = 7;
    return cycles;
}