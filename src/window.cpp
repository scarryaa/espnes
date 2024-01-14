#include "../include/window.hpp"

Window::Window()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Window::poll_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return true;
        }
    }
}

void Window::render()
{
    // Delay to control frame rate (16 ms = 60 fps)
    SDL_Delay(16);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
