#include "../include/window.hpp"

Window::Window()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("espnes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);
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

    return false;
}

void Window::render(uint8_t *frame_buffer)
{
    // Delay to control frame rate (16 ms = 60 fps)
    SDL_Delay(16);

    // Clear screen
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(texture, NULL, frame_buffer, 256 * sizeof(uint8_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
