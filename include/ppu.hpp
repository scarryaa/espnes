#ifndef PPU_HPP
#define PPU_HPP

#include <cstdint>
#include <iostream>
#include "../include/debug/debug.hpp"
#include <string>
#include "../include/interrupt_type.hpp"

class CPU;

class PPU
{
public:
    PPU();
    ~PPU();

    typedef void (*InterruptCallback)();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    void load(uint8_t *rom, uint32_t size);
    void step(int cycles);
    uint8_t *get_frame_buffer();
    void set_interrupt_callback(InterruptCallback callback);
    void set_cpu(CPU &cpu);

private:
    InterruptCallback interruptCallback;
    uint8_t *vram;
    uint8_t *oam;
    uint8_t *palette;
    uint8_t chr_rom[0x2000];

    uint8_t control;
    uint8_t mask;
    uint8_t status;
    uint8_t oam_address;
    uint8_t oam_data;
    uint8_t scroll_x;
    uint8_t scroll_y;
    uint8_t address;
    uint8_t data;
    uint8_t oam_dma;
    int cycles;
    long total_cycles;
    uint8_t scanline;
    uint8_t NMI_occurred;
    uint8_t frame;
    uint8_t write_toggle;
    uint16_t vram_address;

    static const int XRES = 256;
    static const int YRES = 240;
    static const int COLOR_DEPTH = 2;
    uint8_t frame_buffer[XRES * YRES * COLOR_DEPTH];
    static const int SCANLINE_CYCLES = 341;
    static const int SCANLINES = 261;
    static const int VBLANK_SCANLINE = 241;
    const unsigned char PaletteLUT_2C04_0001[64] = {
        0x35, 0x23, 0x16, 0x22, 0x1C, 0x09, 0x1D, 0x15, 0x20, 0x00, 0x27, 0x05, 0x04, 0x28, 0x08, 0x20,
        0x21, 0x3E, 0x1F, 0x29, 0x3C, 0x32, 0x36, 0x12, 0x3F, 0x2B, 0x2E, 0x1E, 0x3D, 0x2D, 0x24, 0x01,
        0x0E, 0x31, 0x33, 0x2A, 0x2C, 0x0C, 0x1B, 0x14, 0x2E, 0x07, 0x34, 0x06, 0x13, 0x02, 0x26, 0x2E,
        0x2E, 0x19, 0x10, 0x0A, 0x39, 0x03, 0x37, 0x17, 0x0F, 0x11, 0x0B, 0x0D, 0x38, 0x25, 0x18, 0x3A};

    void trigger_NMI();
    void draw_pattern_table(int startX, int startY, int table, uint8_t *palette);
    void draw_name_table(int startX, int startY, int table, const uint8_t *palette);
    void draw_pixel(int x, int y, uint16_t color);

    CPU *cpu;
};

#endif
