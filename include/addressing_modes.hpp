#ifndef ADDRESSING_MODES_HPP
#define ADDRESSING_MODES_HPP

#include <cstdint>

class AddressingModes
{
public:
    static uint8_t immediate();
    static uint8_t zero_page();
    static uint8_t zero_page_x();
    static uint8_t zero_page_y();
    static uint8_t absolute();
    static uint8_t absolute_x();
    static uint8_t absolute_y();
    static uint8_t indirect();
    static uint8_t indexed_indirect();
    static uint8_t indirect_indexed();

private:
};

#endif
