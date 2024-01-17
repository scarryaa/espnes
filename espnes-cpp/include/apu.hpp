#ifndef APU_HPP
#define APU_HPP

#include <cstdint>

class APU
{
public:
    APU();
    ~APU();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
};

#endif // APU_HPP
