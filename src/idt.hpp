#ifndef IDT_HPP
#define IDT_HPP

#include "types.hpp"

constexpr u32 IDT_ENTRIES = 256;

#define low_word(addr) (u16)((addr) & 0xFFFF)
#define high_word(addr) (u16)(((addr) >> 16) & 0xFFFF)

typedef struct {
    u16 low_offset;
    u16 selector;
    u8 always0;
    u8 flags;
    u16 high_offset;
} __attribute__((packed)) IdtGate;

typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) IdtRegister;

__attribute__((aligned(0x10))) 
IdtGate idt[256];

IdtRegister idt_reg;

void set_idt_gate(u8 n, u32 handler) {
    idt[n].low_offset = low_word(handler);
    idt[n].selector = 0x08; // see GDT
    idt[n].always0 = 0;
    // 0x8E = 1  00 0 1  110
    //        P DPL 0 D Type
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_word(handler);
}

void load_idt() {
    idt_reg.base = reinterpret_cast<u32>(&idt);
    idt_reg.limit = IDT_ENTRIES * sizeof(IdtGate) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_reg));
}

#endif
