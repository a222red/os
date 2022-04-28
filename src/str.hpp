#ifndef STR_HPP
#define STR_HPP

#include "types.hpp"

extern "C" void itoa(char* buf, i32 i);

u32 strlen(const char* str) {
    u32 i;

    for (i = 0; str[i] != 0x00; ++i);
    
    return i;
}

void strcpy(char* dst, const char* src) {
    char c;
    u32 i = 0;

    while (1) {
        c = src[i];
        if (c == 0x00) break;
        dst[i] = c;
        ++i;
    }
}

u32 trim(char* str, char c, u32 n) {
    char buf[n];
    u32 idx = 0;
    bool lead = true;

    for (u32 i = 0; i < n; ++i) {
        char ci = str[i];

        if (ci == c && lead) continue;
        else lead = false;

        buf[idx++] = ci;
    }

    strcpy(str, buf);
    return idx;
};

u32 itos(char* buf, i32 i) {
    itoa(buf, i);
    u32 idx = trim(buf, '0', 10);
    buf[idx] = 0x00;
    return idx;
}

bool streq(const char* str, const char* s, u32 n) {
    for (int i = 0; i < n; ++i)
        if (str[i] != s[i]) return false;
    return true;
}

#endif
