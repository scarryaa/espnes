#include "../include/ppu.hpp"
#include "../include/cpu.hpp"

PPU::PPU() : cycles(0), scanline(0), frame(0), total_cycles(0), control(0), mask(0), status(0), oam_address(0), oam_data(0), scroll_x(0), scroll_y(0), address(0), data(0), oam_dma(0), NMI_occurred(0)
{
    vram = new uint8_t[0x2000];
    oam = new uint8_t[0x100];
    palette = new uint8_t[0x20];
}

PPU::~PPU()
{
    delete[] vram;
    delete[] oam;
    delete[] palette;
}

void PPU::set_cpu(CPU &cpu)
{
    this->cpu = &cpu;
}

uint8_t *PPU::get_frame_buffer()
{
    return frame_buffer;
}

uint8_t PPU::read(uint16_t address)
{
    uint8_t register_index = address & 7;
    uint8_t old_NMI = this->NMI_occurred;
    uint8_t old_status = this->status;

    switch (register_index)
    {
    case 0:
        // PPUCTRL
        return this->control;
    case 1:
        // PPUMASK
        return this->mask;
    case 2:
        // PPUSTATUS
        this->write_toggle = 0; // Reading status resets write toggle

        // Clear VBlank flag after reading status
        this->status &= ~0x80;

        // Clear NMI
        this->NMI_occurred = 0;

        return old_status;
    case 3:
        // OAMADDR
        return this->oam_address;
    case 4:
        // OAMDATA
        return this->oam_data;
    case 5:
        // PPUSCROLL
        // return this->scroll;
        break;
    case 6:
        // PPUADDR
        return this->address;
    case 7:
        // PPUDATA
        return this->data;
    case 8:
        // OAMDMA
        return this->oam_dma;
    }

    Debug::debug_print("Unknown PPU read: " + std::to_string(address));
    return 0;
}

void PPU::write(uint16_t address, uint8_t value)
{
    uint8_t register_index = address & 7;
    switch (register_index)
    {
    case 0:
        // PPUCTRL

        // Ignore writes if cycles 0 - 29658 (reset)
        if (this->total_cycles < 29659)
        {
            return;
        }
        this->control = value;

        // Set VRAM address increment
        this->vram_address++;
        break;
    case 1:
        // PPUMASK
        this->mask = value;
        break;
    case 2:
        // PPUSTATUS
        this->status = value;
        break;
        // OAMADDR
    case 3:
        this->oam_address = value;
        break;
    case 4:
        // OAMDATA
        this->oam_data = value;

        // Increment OAM address after writes
        this->oam_address++;
        break;
    case 5:
        // PPUSCROLL
        if (this->write_toggle == 0)
        {
            // First write
            this->scroll_x = value;
            this->write_toggle = 1;
        }
        else
        {
            // Second write
            this->scroll_y = value;
            this->write_toggle = 0;
        }
        break;
    case 6:
        // PPUADDR
        if (this->write_toggle == 0)
        {
            // First write
            this->address = value;
            this->write_toggle = 1;
        }
        else
        {
            // Second write
            this->address = (this->address << 8) | value;
            this->write_toggle = 0;
        }
        break;
    case 7:
        // PPUDATA
        this->vram[this->vram_address] = value;
        this->vram_address++; // Increment VRAM address after writes
        break;
    case 8:
        // OAMDMA
        this->oam_dma = value;
        break;
    }
}

void PPU::load(uint8_t *rom, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        vram[i] = rom[i];
    }
}

void PPU::step(int cycles)
{
    this->cycles += cycles;
    this->total_cycles += cycles / 3;

    // VBlank
    if (this->scanline == VBLANK_SCANLINE && this->cycles == 1)
    {
        // Set VBlank flag
        this->status |= 0x80;

        // Trigger NMI
        this->NMI_occurred = 1;

        // Trigger NMI if NMI_occured is true and NMI is enabled
        if (this->NMI_occurred && (this->control & 0x80))
        {
            // Trigger NMI
            this->cpu->set_interrupt(InterruptType::NMI);

            this->NMI_occurred = 0;
        }
    }

    // Pre-render scanline
    else if (this->scanline == SCANLINES && this->cycles == 1)
    {
        // Clear VBlank flag
        this->status &= ~0x80;

        // Clear NMI
        this->NMI_occurred = 0;
    }

    if (this->cycles == SCANLINE_CYCLES)
    {
        this->cycles = 0;
        this->scanline++;
    }

    if (this->scanline == SCANLINES + 1)
    {
        this->scanline = 0;
        this->frame++;
    }

    draw_pattern_table(0, 0, 0, this->palette);
    draw_pattern_table(128, 0, 1, this->palette);
}

void PPU::draw_pattern_table(int startX, int startY, int table, uint8_t *palette)
{
    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            uint16_t tile = 16 * y + x;
            uint16_t tile_addr = 0x1000 * table + 16 * tile;
            uint8_t *tile_data = &chr_rom[tile_addr];

            for (int row = 0; row < 8; row++)
            {
                uint8_t lo = tile_data[row];
                uint8_t hi = tile_data[row + 8];

                for (int col = 0; col < 8; col++)
                {
                    uint8_t color_index = ((hi >> (7 - col)) & 0x1) << 1 | ((lo >> (7 - col)) & 0x1);
                    uint16_t color = palette[color_index];
                    draw_pixel(startX + x * 8 + col, startY + y * 8 + row, color);
                }
            }
        }
    }
}

void PPU::draw_name_table(int startX, int startY, int table, const uint8_t *palette)
{
    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 30; y++)
        {
            uint16_t tile = 30 * y + x;
            uint16_t tile_addr = 0x2000 + 0x400 * table + 16 * tile;
            uint8_t *tile_data = &vram[tile_addr];

            for (int row = 0; row < 8; row++)
            {
                uint8_t lo = tile_data[row];
                uint8_t hi = tile_data[row + 8];

                for (int col = 0; col < 8; col++)
                {
                    uint8_t color_index = ((hi >> (7 - col)) & 0x1) << 1 | ((lo >> (7 - col)) & 0x1);
                    uint16_t color = palette[color_index];
                    draw_pixel(x * 8 + col, y * 8 + row, color);
                }
            }
        }
    }
}

void PPU::draw_pixel(int x, int y, uint16_t color)
{
    int index = (y * XRES + x) * COLOR_DEPTH;
    frame_buffer[index] = (color >> 16) & 0xFF;
    frame_buffer[index + 1] = (color >> 8) & 0xFF;
    frame_buffer[index + 2] = color & 0xFF;
}