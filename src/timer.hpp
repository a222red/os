#ifndef TIMER_DRIVER_HPP
#define TIMER_DRIVER_HPP

#include "isr.hpp"

#define IRQ0 32

/// Global tick counter.
u32 ticks = 0;
/// Pointer to a function that is called every tick.
/// If this pointer is null, no function is called.
void (*timer_func)() = nullptr;

/// This function is called every tick.
/// If `timer_func` is non-null, it will be called.
static void timer_callback(Registers* regs) {
    ++ticks;
    if (timer_func) timer_func();
}

extern "C" void init_timer() {
    interrupt_handlers[IRQ0] = timer_callback;
}


#endif
