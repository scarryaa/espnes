#ifndef PPU_HPP
#define PPU_HPP

#include <cstdint>
#include <iostream>
#include "../include/debug/debug.hpp"
#include <string>
#include "../include/interrupt_type.hpp"
#include <vector>

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
    uint8_t *get_vram();
    uint8_t *get_frame_buffer();
    uint8_t *get_palette();
    void set_cpu(CPU &cpu);
    void reset();
    void render_background_scanline(int scanline);
    int get_cycle();
    int get_scanline();
    int get_frame();
    void draw_palette();
    long get_total_cycles();

private:
    InterruptCallback interruptCallback;
    uint8_t *vram;
    uint8_t *oam;
    uint8_t *palette;
    uint8_t chr_rom[0x2000];

    uint8_t control;
    uint8_t mask;
    uint8_t status;
    uint8_t prev_read;
    uint8_t oam_address;
    uint8_t oam_data;
    uint8_t scroll_x;
    uint8_t scroll_y;
    uint8_t address;
    uint8_t data;
    uint8_t oam_dma;
    int cycles;
    long total_cycles;
    int scanline;
    uint8_t NMI_occurred;
    uint8_t frame;
    uint8_t write_toggle;
    uint16_t vram_address;

    static const int XRES = 256;
    static const int YRES = 240;
    static const int COLOR_DEPTH = 4;
    uint8_t frame_buffer[XRES * YRES * COLOR_DEPTH];
    static const int SCANLINE_CYCLES = 341;
    static const int SCANLINES = 261;
    static const int VBLANK_SCANLINE = 241;
    const uint32_t PaletteLUT_2C04_0001[64] = {
        0xFF585858, 0xFF00237C, 0xFF0D1099, 0xFF300092, 0xFF4F006C, 0xFF600035, 0xFF5C0500, 0xFF461800,
        0xFF271400, 0xFF0B2400, 0xFF003200, 0xFF003D00, 0xFF003840, 0xFF002F66, 0xFF000000, 0xFF000000,
        0xFFA8A8A8, 0xFF0F63B2, 0xFF4051D0, 0xFF7844BC, 0xFFA7369A, 0xFFB3285C, 0xFFB53120, 0xFF994E00,
        0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,
        0xFFFFFFFF, 0xFFA0D6F0, 0xFFB8B8F8, 0xFFD8B8F8, 0xFFF8B8F8, 0xFFF8A4C0, 0xFFF0D0B0, 0xFFFCE0A8,
        0xFFF8E888, 0xFFD8F878, 0xFFB8F8B8, 0xFFB8F8D8, 0xFF00FCFC, 0xFFF8D8F8, 0xFF000000, 0xFF000000,
        0xFFFFFFFF, 0xFFC4E4F0, 0xFFD8D8F8, 0xFFE8D8F8, 0xFFF8D8F8, 0xFFF8CCE8, 0xFFF4E4D8, 0xFFFCE4D0,
        0xFFF8F0C0, 0xFFF0F8C8, 0xFFD8F8D8, 0xFFD8F8E8, 0xFF00FCFC, 0xFFF8F8F8, 0xFF000000, 0xFF000000};

    void draw_pattern_table(int startX, int startY, int table, uint8_t *palette);
    void draw_name_table(int nameTableIndex);
    void draw_pixel(int x, int y, uint32_t color);

    CPU *cpu;
};

#endif
