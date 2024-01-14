#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>

class Window
{
public:
    Window();
    ~Window();

    bool poll_events();
    void render();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif