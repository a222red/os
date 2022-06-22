#ifndef PORT_HPP
#define PORT_HPP

#include "types.hpp"

/// Output a byte to a port.
static inline void outb(u16 port, u8 data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

/// Output 2 bytes to a port.
static inline void outw(u16 port, u16 data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

/// Output 4 bytes to a port.
static inline void outd(u16 port, u32 data) {
    __asm__("out %%eax, %%dx" : : "a" (data), "d" (port));
}

/// Get a byte of input from a port.
static inline u8 inb(u16 port) {
    u8 data;
    __asm__("in %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

/// Get 2 bytes of input from a port.
static inline u16 inw(u16 port) {
    u16 data;
    __asm__("in %%dx, %%ax" : "=a" (data) : "d" (port));
    return data;
}

/// Get 4 bytes of input from a port.
static inline u32 ind(u16 port) {
    u32 data;
    __asm__("in %%dx, %%eax" : "=a" (data) : "d" (port));
    return data;
}

#endif
