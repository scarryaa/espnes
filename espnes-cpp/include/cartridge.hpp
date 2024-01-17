#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <cstdint>

class Cartridge
{
public:
    Cartridge();
    ~Cartridge();

    void load(uint8_t *rom, uint32_t size);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    void switchBank(uint16_t bank);

private:
    uint8_t *rom;
    uint16_t currentBank;
    static const uint16_t BANK_SIZE = 0x4000; // 16KB
};

#endif
