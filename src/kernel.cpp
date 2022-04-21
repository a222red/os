#include "types.hpp"
#include "mem.hpp"
#include "str.hpp"
#include "vga.hpp"
#include "keyboard.hpp"
#include "timer.hpp"
#include "syscall.hpp"
#include "disk.hpp"
#include "shell.hpp"

extern "C" void kernel_main(u16 kernel_sectors) {
    shell_init();
}
