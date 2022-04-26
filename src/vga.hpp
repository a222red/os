#ifndef VGA_HPP
#define VGA_HPP

#include "types.hpp"
#include "port.hpp"
#include "str.hpp"

// This has to be a macro bc constexpr rules are dumb
#define VID_MEM (reinterpret_cast<u8*>(0x000b8000))

constexpr u32 MAX_ROWS = 25;
constexpr u32 MAX_COLS = 80;

namespace colors {
    enum color: u8 {
        BLACK = 0x0,
        BLUE = 0x1,
        GREEN = 0x2,
        CYAN = 0x3,
        RED = 0x4,
        PURPLE = 0x5,
        BROWN = 0x6,
        GRAY = 0x7,
        DARK_GRAY = 0x8,
        LIGHT_BLUE = 0x9,
        LIGHT_GREEN = 0xa,
        LIGHT_CYAN = 0xb,
        LIGHT_RED = 0xc,
        LIGHT_PURPLE = 0xd,
        YELLOW = 0xe,
        WHITE = 0xf
    };

    inline u8 text_color(
        u8 fg, u8 bg, bool fg_b = false, bool bg_b = false
    ) {
        return (
            ((bg << 4) | (bg_b << 7)) | ((fg & 0x0F) | (fg_b << 3))
        );
    }
}

extern "C" u32 lf_offset(u32 offset) {
    return ((offset / 160) + 1) * 160;
}

extern "C" void set_cursor(u32 offset);
extern "C" u32 get_cursor();
extern "C" void set_char(char ch, u8 color, u32 offset);
extern "C" void putc(char chr, u8 color=0x0f);
extern "C" void puts(const char* str, u8 color=0x0f);
extern "C" void scroll(u32 n = 1);
extern "C" void clear();

void enable_cursor(u8 cursor_start, u8 cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor() {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void char_back() {
    u32 c = get_cursor();
    VID_MEM[c * 2 - 2] = ' ';
    VID_MEM[c * 2 - 1] = 0x07;
    set_cursor(c - 1);
};

extern "C" void print_num(i32 i) {
    char a[11] { 0 };
    itos(a, i);
    puts(a);
}

#endif
