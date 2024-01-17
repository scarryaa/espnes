#ifndef BREAKPOINT_TYPES_H
#define BREAKPOINT_TYPES_H

#include <set>

// Breakpoint types
typedef enum {
    BREAKPOINT_TYPE_ADDRESS,
    BREAKPOINT_TYPE_CYCLE,
    BREAKPOINT_TYPE_SCANLINE
} breakpoint_type_t;

struct Breakpoint {
    breakpoint_type_t type;
    uint16_t address;

    bool operator<(const Breakpoint& other) const {
        return address < other.address || (address == other.address && type < other.type);
    }
};

#endif