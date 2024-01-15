#include "../../include/debug/debug.hpp"

void Debug::debug_print(const char *format, ...)
{
    if (IS_DEBUG)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        vprintf("\n", args);
        va_end(args);
    }
}

void Debug::debug_print(std::string format, ...)
{
    if (IS_DEBUG)
    {
        va_list args;
        va_start(args, format);
        vprintf(format.c_str(), args);
        vprintf("\n", args);
        va_end(args);
    }
}

void Debug::hex_to_string(uint8_t *data, int size, std::string &output)
{
    for (int i = 0; i < size; i++)
    {
        char buffer[3];
        sprintf(buffer, "%02X", data[i]);
        output += buffer;
    }
}