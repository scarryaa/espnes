#include <string>
#include "window.hpp"

class Emulator
{
public:
    Emulator();
    ~Emulator();

    void loadROM(const std::string &romPath);
    void run();

private:
    Window window;
    bool quit;
    bool paused;
};
