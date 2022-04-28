#ifndef PORT_HPP
#define PORT_HPP

#include "types.hpp"

/// Output a byte to a port.
static inline void outb(u16 port, u8 data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

/// Get a byte of input from a port.
static inline u8 inb(u16 port) {
    u8 data;
    __asm__("in %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

#endif
