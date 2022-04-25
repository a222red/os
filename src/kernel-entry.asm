bits 32
extern clear
extern puts
extern isr_install
extern init_keyboard
extern init_timer
extern init_syscalls
extern init_alloc

extern kernel_main
    call clear
    call isr_install
    sti
    call init_keyboard
    call init_timer
    call init_alloc
    call init_syscalls
    call kernel_main
    cli
_kernel_loop:
    push 0x0a
    push str_kernel_done
    call puts
    hlt
    jmp _kernel_loop

str_kernel_done: db "Kernel has exited, you may shut off the machine.", 0x0a, 0x00
