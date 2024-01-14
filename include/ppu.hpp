#ifndef PPU_HPP
#define PPU_HPP

#include <cstdint>
#include <iostream>

class PPU
{
public:
    PPU();
    ~PPU();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    void load(uint8_t *rom, uint32_t size);

private:
    uint8_t *vram;
    uint8_t *oam;
    uint8_t *palette;

    uint8_t control;
    uint8_t mask;
    uint8_t status;
    uint8_t oam_address;
    uint8_t oam_data;
    uint8_t scroll;
    uint8_t address;
    uint8_t data;
    uint8_t oam_dma;
};

#endif
