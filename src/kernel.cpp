#include "timer.hpp"
#include "syscall.hpp"
#include "disk.hpp"
#include "shell.hpp"

extern "C" void kernel_main(u16 kernel_sectors) {
    start_shell();
}
