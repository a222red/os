#include "timer.hpp"
#include "syscall.hpp"
#include "disk.hpp"
#include "shell.hpp"

extern "C" void kernel_main(u16 kernel_sectors) {
    using namespace colors;
    
    puts(
R"(Welcome to <insert OS name here>!
Type `help` for a list of commands.
)",
        text_color(GREEN, BLACK, true)
    );
    start_shell();
}
