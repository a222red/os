#ifndef KEYBOARD_DRIVER_HPP
#define KEYBOARD_DRIVER_HPP

#include "types.hpp"
#include "port.hpp"
#include "isr.hpp"
#include "event.hpp"

#define IRQ1 33

enum Key: u8 {
    K_ERROR, K_ESC, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
    K_0, K_DASH, K_EQ, K_BACKSPACE, K_TAB, K_Q, K_W, K_E, K_R, K_T,
    K_Y, K_U, K_I, K_O, K_P, K_LBRAK, K_RBRAK, K_ENTER, K_CTRL, K_A,
    K_S, K_D, K_F, K_G, K_H, K_J, K_K, K_L, K_SEMICOLON, K_QUOTE,
    K_BACKTICK, K_LSHIFT, K_BSLASH, K_Z, K_X, K_C, K_V, K_B, K_N,
    K_M, K_COMMA, K_PERIOD, K_SLASH, K_RSHIFT, K_KEYPAD_STAR, K_ALT,
    K_SPACE
};

bool keys_down[58] { false };

const char sc_ascii[] {
    0x00, 0x00, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', 0x01, 0x00,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']', '\n', 0x00, 'a', 's', 'd', 'f', 'g',
    'h', 'j', 'k', 'l', ';', '\'', '`', 0x00, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm',
    ',', '.', '/', 0x00, 0x00, 0x00, ' '
};

const char sc_ascii_upper[] {
    0x00, 0x00, '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', 0x01, 0x00,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}', '\n', 0x00, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ':', '"', '~', 0x00, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M',
    '<', '>', '?', 0x00, 0x00, 0x00, ' '
};

volatile bool shift_held = false;

static void keyboard_callback(Registers* regs) {
    u8 scancode = inb(0x60);
    
    if (scancode <= 0x39) {
        keys_down[scancode] = true;
        push_event({ KEY_DOWN, { .scancode=scancode } });
    }
    else {
        keys_down[scancode - 0x80] = false;
        push_event({
            KEY_UP,
            { .scancode=(u8)(scancode - 0x80) }
        });
    }
}

extern "C" void init_keyboard() {
    interrupt_handlers[IRQ1] = keyboard_callback;
}

static inline char getc() {
    Event e {};
    
    while (
        !get_event(&e) ||
        e.type != KEY_DOWN ||
        sc_ascii[e.data.scancode] == 0x00
    )
        asm("hlt");
    
    return (keys_down[K_LSHIFT] || keys_down[K_RSHIFT]) ?
        sc_ascii_upper[e.data.scancode] :
        sc_ascii[e.data.scancode];
}

u32 gets(char* buf, u32 n) {
    u32 i;
    char c;
    
    for (i = 0; i < n; ++i) {
        c = getc();
        if (c == '\n') break;
        else if (c == 0x01) {
            char_back();
            i -= 2;
            continue;
        }
        putc(c);
        buf[i] = c;
    }
    
    putc('\n');
    buf[i] = 0x00;

    return i;
}

#endif
