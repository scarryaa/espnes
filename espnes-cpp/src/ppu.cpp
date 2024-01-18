#include "../include/ppu.hpp"
#include "../include/cpu.hpp"

PPU::PPU() : cycles(0), scanline(0), frame(0), total_cycles(7), control(0), mask(0), status(0), oam_address(0), oam_data(0), scroll_x(0), scroll_y(0), address(0), data(0), oam_dma(0), NMI_occurred(0)
{
    vram = new uint8_t[0x2000];
    oam = new uint8_t[0x100];
    palette = new uint8_t[0x40];
    cycles = 21;
    scanline = 0;
    frame = 0;
    total_cycles = 7;
    old_frame = 500;
    this->status = 0;
    
    for (int i = 0; i < 0x2000; i++)
    {
		vram[i] = 0;
	}   

    for (int i = 0; i < 0x100; i++)
    {
        frame_buffer[i] = 0;
    }

    for (int i = 0; i < 0x100; i++)
    {
        oam[i] = 0;
    }

    for (int i = 0; i < 0x40; i++)
    {
		palette[i] = 0;
	}

    this->oam_dma = 0;
    this->vram_address = 0;
}

PPU::~PPU()
{
    delete[] vram;
    delete[] oam;
    delete[] palette;
}

long PPU::get_total_cycles()
{
    return this->total_cycles;
}

uint8_t* PPU::get_vram()
{
    return vram;
}

int PPU::get_cycle()
{
    return this->cycles;
}

int PPU::get_scanline()
{
    return this->scanline;
}

int PPU::get_frame()
{
    return this->frame;
}

uint8_t* PPU::get_palette()
{
    return palette;
}

void PPU::reset()
{
    this->cycles = 21;
    this->scanline = 0;
    this->frame = 0;
    this->total_cycles = 7;
    this->control = 0;
    this->mask = 0;
    this->status = 0;
    this->oam_address = 0;
    this->oam_data = 0;
    this->scroll_x = 0;
    this->scroll_y = 0;
    this->address = 0;
    this->data = 0;
    this->oam_dma = 0;
    this->NMI_occurred = 0;
    this->prev_read = 0;
    this->old_frame = 500;

    // Clear frame buffer
    for (int i = 0; i < XRES * YRES * COLOR_DEPTH; i++)
    {
        frame_buffer[i] = 0;
    }

    // Clear VRAM
    for (int i = 0; i < 0x2000; i++)
    {
		vram[i] = 0;
	}

    // Clear OAM
    for (int i = 0; i < 0x100; i++)
    {
		oam[i] = 0;
	}

	// Clear palette
    for (int i = 0; i < 0x40; i++)
    {
		palette[i] = 0;
	}

	// Clear OAMDMA
	this->oam_dma = 0;

    // Clear NMI
	this->NMI_occurred = 0;

    // Clear NMI triggered
    this->nmi_triggered = 0;

    // Clear write toggle
    this->write_toggle = 0;

    // Clear VRAM address
    this->vram_address = 0;

    // Clear OAM address
    this->oam_address = 0;

    // Clear OAM data
    this->oam_data = 0;

    // Clear scroll
    this->scroll_x = 0;
    this->scroll_y = 0;
}

void PPU::set_cpu(CPU& cpu)
{
    this->cpu = &cpu;
}

uint8_t* PPU::get_frame_buffer()
{
    return frame_buffer;
}

uint8_t PPU::read(uint16_t address)
{
    uint8_t register_index = address & 7;
    uint8_t old_NMI = this->NMI_occurred;
    uint8_t current_status = this->status;

    switch (register_index)
    {
    case 0:
        // PPUCTRL
        this->prev_read = this->control;
        return this->control;
    case 1:
        // PPUMASK
        this->prev_read = this->mask;
        return this->mask;
    case 2:
    {
        // PPUSTATUS
        this->write_toggle = 0; // Reading status resets write toggle

        // Clear VBlank flag after reading status
        this->status &= ~0x80;

        // Clear NMI
        this->NMI_occurred = 0;

        // Return status with lower 5 bits set to last value written
        return (current_status & 0xE0) | (prev_read & 0x1F);
    }
    case 3:
        // OAMADDR
        this->prev_read = this->oam_address;
        return this->oam_address;
    case 4:
        // OAMDATA
        this->prev_read = this->oam_data;
        return this->oam_data;
    case 5:
        // PPUSCROLL
        // return this->scroll;
        break;
    case 6:
        // PPUADDR
        this->prev_read = this->address;
        return this->address;
    case 7:
        // PPUDATA
        this->prev_read = this->data;
        return this->data;
    case 8:
        // OAMDMA
        this->prev_read = this->oam_dma;
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
    {
        // PPUDATA
        // Adjust address for mirroring
        unsigned int mirrored_address = this->vram_address % 0x2000;
        this->vram[mirrored_address] = value;
        // Increment and wrap VRAM address after writes
        this->vram_address = (this->vram_address + 1) & 0x3FFF;
        break;
    }
    case 8:
        // OAMDMA
        this->oam_dma = value;
        break;
    }

    this->prev_read = value;
}

void PPU::load(uint8_t* rom, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        chr_rom[i] = rom[i];
    }
}

void PPU::set_vblank_flag()
{
	this->status |= 0x80;
}

void PPU::step(int cycles)
{
    this->cycles += cycles;
    this->total_cycles += cycles / 3;

    if (this->cycles >= SCANLINE_CYCLES)
    {
        this->cycles -= SCANLINE_CYCLES;
        this->scanline++;
    }

    // Pre-render scanline
    if (this->scanline == SCANLINES)
    {
        nmi_triggered = false;
        // Clear VBlank flag
        this->status &= ~0x80;

        // Clear NMI
        this->NMI_occurred = 0;

        this->scanline = 0;
        this->frame++;
    }

    // VBlank
    if (this->scanline == VBLANK_SCANLINE && nmi_triggered == false && this->old_frame != this->frame)
    {
        // Set VBlank flag
        this->status |= 0x80;

        old_frame = this->frame;

        // Trigger NMI
        this->NMI_occurred = 1;

        // Trigger NMI if NMI_occured is true and NMI is enabled
        if (this->NMI_occurred && (this->control & 0x80) != 0 && !this->nmi_triggered)
        {
            // Trigger NMI
            this->cpu->set_interrupt(InterruptType::NMI);
            this->nmi_triggered = true;
            this->NMI_occurred = 0;
        }
    }

    for (int i = 0; i < 32; i++)
    {
        palette[i] = vram[0x3F00 + i];
    }

     render_background_scanline(this->scanline);
    // draw_pattern_table(0, 8, 0, this->palette);
    // draw_pattern_table(128, 8, 1, this->palette);
    // draw_name_table(0, 8, 0, this->palette);
    // draw_name_table(0, 8, 1, this->palette);
}

void PPU::draw_palette()
{
    for (int i = 0; i < 32; i++)
    {
        uint16_t color = palette[i];
        draw_pixel(i % 16, i / 16, color);
    }
}

void PPU::render_background_scanline(int scanline)
{
    for (int x = 0; x < XRES; x++)
    {
        uint16_t tile = 32 * scanline / 8 + x / 8;
        uint16_t tile_addr = 0x2000 + 0x400 * 0 + 16 * tile;
        uint8_t* tile_data = &vram[tile_addr];

        uint8_t lo = tile_data[scanline % 8];
        uint8_t hi = tile_data[scanline % 8 + 8];

        
        uint8_t color_index = ((hi >> (7 - x % 8)) & 0x1) << 1 | ((lo >> (7 - x % 8)) & 0x1);
        uint16_t color = PaletteLUT_2C04_0001[palette[color_index]];
        draw_pixel(x, scanline, color);
    }
}

void PPU::draw_pattern_table(int startX, int startY, int table, uint8_t* palette)
{
    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            uint16_t tile = 16 * y + x;
            uint16_t tile_addr = 0x1000 * table + 16 * tile;
            uint8_t* tile_data = &chr_rom[tile_addr];

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

void PPU::draw_name_table(int nameTableIndex)
{
    uint16_t baseAddr = nameTableIndex == 0 ? 0x2000 : 0x2400;

    for (int row = 0; row < 30; ++row) { // 30 tiles per column
        for (int col = 0; col < 32; ++col) { // 32 tiles per row
            uint16_t tileIndex = vram[baseAddr + row * 32 + col]; // Get tile index
            uint16_t tileAddr = 0x1000 * (control & 0x10) + 16 * tileIndex; // Get tile address

            // Get tile data
            uint8_t* tileData = &chr_rom[tileAddr];

            // Calculate attribute table address for the tile
            uint16_t attrTableAddr = baseAddr + 0x3C0 + (row / 4) * 8 + (col / 4);
            uint8_t attrByte = vram[attrTableAddr];

            // Determine which 2-bit palette number to use from the attribute byte
            int paletteShift = ((row % 4) / 2 * 2 + (col % 4) / 2 * 4);
            uint8_t paletteIndex = (attrByte >> paletteShift) & 0x03;

            // Draw tile
            for (int y = 0; y < 8; ++y) {
                uint8_t lo = tileData[y];
                uint8_t hi = tileData[y + 8];

                for (int x = 0; x < 8; ++x) {
                    uint8_t colorIndex = ((hi >> (7 - x)) & 0x1) << 1 | ((lo >> (7 - x)) & 0x1);
                    uint8_t paletteAddr = 0x3F00 + paletteIndex * 4 + colorIndex;
                    uint16_t color = PaletteLUT_2C04_0001[palette[paletteAddr]];
                    draw_pixel(col * 8 + x, row * 8 + y, color);
                }
            }
        }
    }
}

inline void PPU::draw_pixel(int x, int y, uint32_t color)
{
    if (x < 0 || x >= XRES || y < 0 || y >= YRES)
    {
        return;
    }

    int index = (y * XRES + x) * 4;
    this->frame_buffer[index] = color & 0xFF;             // Blue
    this->frame_buffer[index + 1] = (color >> 8) & 0xFF;  // Green
    this->frame_buffer[index + 2] = (color >> 16) & 0xFF; // Red
    this->frame_buffer[index + 3] = (color >> 24) & 0xFF; // Alpha
}