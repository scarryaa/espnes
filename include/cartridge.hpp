#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <cstdint>

class Cartridge
{
public:
    Cartridge();
    ~Cartridge();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    uint8_t *rom;
    uint8_t *ram;
};

#endif
