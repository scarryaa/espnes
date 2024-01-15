#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>
#include "../include/imgui/imgui.h"
#include "../include/imgui/imconfig.h"
#include "../include/imgui/imgui_internal.h"
#include "../include/imgui/imgui_impl_sdl2.h"
#include "../include/imgui/imgui_impl_sdlrenderer2.h"
#include <iostream>
class Window
{
public:
    Window();
    ~Window();

    bool poll_events();
    void render(uint8_t *frame_buffer);
    void post_render(uint8_t *frame_buffer);
    void render_menu_bar();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};

#endif