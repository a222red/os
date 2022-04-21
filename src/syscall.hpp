#ifndef SYSCALL_HPP
#define SYSCALL_HPP

#include "types.hpp"
#include "isr.hpp"
#include "vga.hpp"
#include "keyboard.hpp"
#include "timer.hpp"
#include "mem.hpp"

typedef u32 (*Syscall)(u32, u32, u32);
#define SYS [](u32 b, u32 c, u32 d) -> u32

Syscall syscall_arr[256];
extern "C" void* syscalls = syscall_arr;

extern "C" void init_syscalls() {
    syscall_arr[0] = SYS {
        puts((char*)b, c);
        return 0;
    };
    syscall_arr[1] = SYS {
        return gets((char*)b, c);
    };
    syscall_arr[2] = SYS {
        return (u32)mem_alloc(b);
    };
    syscall_arr[3] = SYS {
        mem_free((void*)b);
        return 0;
    };
}

#endif
