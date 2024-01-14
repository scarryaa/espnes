#include "../include/emulator.hpp"

Emulator::Emulator()
{
}

Emulator::~Emulator()
{
}

void Emulator::loadROM(const std::string &romPath)
{
    printf("Loading ROM: %s\n", romPath.c_str());
}

void Emulator::run()
{
    while (!quit)
    {
        quit = window.poll_events();

        if (!paused)
        {
            // TODO: Run CPU
        }

        // render graphics
        window.render();
    }
}