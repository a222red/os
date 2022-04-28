#ifndef STR_HPP
#define STR_HPP

#include "types.hpp"

/// Converts an integer to a string,
/// with leading zeroes.
extern "C" void itoa(char* buf, i32 i);

/// Gets the length of a null-terminated string.
u32 strlen(const char* str) {
    u32 i;

    for (i = 0; str[i] != 0x00; ++i);
    
    return i;
}

/// Copies a null-terminated string to a buffer.
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

/// Trims at most `n` leading `c`s from `str`.
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

/// Converts `i` to a string and writes it to `buf`.
/// `buf` must be at least 11 bytes for this function
/// to be called safely.
u32 itos(char* buf, i32 i) {
    itoa(buf, i);
    u32 idx = trim(buf, '0', 10);
    buf[idx] = 0x00;
    return idx;
}

/// Checks whether the first `n` characters of `str` and
/// `s` are the same.
bool streq(const char* str, const char* s, u32 n) {
    for (int i = 0; i < n; ++i)
        if (str[i] != s[i]) return false;
    return true;
}

#endif
