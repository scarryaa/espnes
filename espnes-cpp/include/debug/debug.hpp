#ifndef DEBUG_HPP
#define DEBUG_HPP

#ifdef DEBUG
#define IS_DEBUG true
#else
#define IS_DEBUG false
#endif

#include <cstdarg>
#include <stdio.h>
#include <string>

class Debug
{
public:
    static void debug_print(const char *format, ...);
    static void debug_print(std::string format, ...);
    static void hex_to_string(uint8_t *data, int size, std::string &output);
};

#endif