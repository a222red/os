#ifndef SHELL_HPP
#define SHELL_HPP

#include "types.hpp"
#include "keyboard.hpp"
#include "vga.hpp"
#include "tokenizer.hpp"
#include "str.hpp"

inline bool cmdeq(
    const char* cmd, Range<> r, const char* str, u32 len
) {
    return (
        r.end - r.start == len &&
        streq(cmd + r.start, str, len)
    );
}

bool exec_cmd(
    const char* cmd, const List<Range<>>& argl
) {
    using namespace colors;
    
    u32 argc = argl.len();
    auto arg0 = argl[0];

    if (argc > 0) {
        if (cmdeq(cmd, arg0, "clear", 5)) clear();
        else if (cmdeq(cmd, arg0, "help", 4)) puts(
R"(Available commands:
    clear: Clear the console
    help: Show this message
    echo [MSGS]: Print [MSGS] to the console
    greet [PPL]: Say hello to [PPL]
    exit: Exit the shell
)",
            text_color(GREEN, BLACK, true)
        );
        else if (cmdeq(cmd, arg0, "echo", 4))
            for (u32 i = 1; i < argc; ++i) {
                auto r = argl[i];
                
                for (u32 j = r.start; j < r.end; ++j)
                    putc(cmd[j]);
                
                putc('\n');
            }
        else if (cmdeq(cmd, arg0, "greet", 5))
            for (u32 i = 1; i < argc; ++i) {
                auto r = argl[i];
    
                puts("Hello, ");
                
                for (u32 j = r.start; j < r.end; ++j)
                    putc(cmd[j]);
                
                puts("!\n");
            }
        else if (cmdeq(cmd, arg0, "exit", 4))
            return false;
        else puts("Unknown command\n");
    }

    return true;
}

void start_shell() {
    using namespace colors;
    
    char cmd[256];
    
    while (1) {
        putc('>', text_color(GREEN, BLACK, true));
        gets(cmd, 255);

        if (!exec_cmd(cmd, tokenize(cmd))) break;
    }
}

#endif
