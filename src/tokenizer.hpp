#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "container.hpp"

List<Range> tokenize(const char* s) {
    Range r { 0, 0 };
    List<Range> l;
    bool in_str = false;

    while (1) {
        char c = s[r.end];
        
        if (c == 0x00) {
            if (s[r.end - 1] != ' ' && r.end > r.start)
                l.push_back(r);
            break;
        }
        else if (c == '"') {
            in_str = !in_str;
            if (r.end > 0 && s[r.end - 1] != ' ')
                l.push_back(r);
            r.start = r.end + 1;
        }
        else if (c == ' ' && !in_str) {
            if (s[r.end - 1] != ' ' && r.end > r.start)
                l.push_back(r);
            r.start = r.end + 1;
        }

        ++r.end;
    }

    return l;
}

#endif
