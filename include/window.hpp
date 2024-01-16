#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>
#include "../include/imgui/imgui.h"
#include "../include/imgui/imconfig.h"
#include "../include/imgui/imgui_internal.h"
#include "../include/imgui/imgui_impl_sdl2.h"
#include "../include/imgui/imgui_impl_sdlrenderer2.h"
#include "../include/debug/disassembler.hpp"
#include <iostream>

class Emulator;

class Window
{
public:
    Window();
    ~Window();

    void render_memory_view(Emulator *emulator);
    bool poll_events();
    void render(Emulator *emulator);
    void post_render(uint8_t *frame_buffer);
    void render_menu_bar(Emulator &emulator);
    void render_disassembly(Emulator *emulator);
    void render_PPU(Emulator *emulator);
    void render_CPU(Emulator *emulator);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    bool show_disassembly;
};

#endif