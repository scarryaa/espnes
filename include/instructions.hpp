#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <cstdint>

class Instructions
{
public:
    static uint8_t nop_impl();
    typedef uint8_t (*InstructionFunction)();
    static Instructions::InstructionFunction ins_table[256];

private:
};

#endif
