#ifndef TIMER_DRIVER_HPP
#define TIMER_DRIVER_HPP

#include "isr.hpp"

#define IRQ0 32

u32 ticks = 0;
void (*timer_func)() = nullptr;

static void timer_callback(Registers* regs) {
    ++ticks;
    if (timer_func) timer_func();
}

extern "C" void init_timer() {
    interrupt_handlers[IRQ0] = timer_callback;
}


#endif
