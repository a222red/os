prot_str: db "Entering protected mode... ", 0x00

bits 16
activate_protected:
    mov di, prot_str
    call print_str
    cli ;; Disable interrupts
    lgdt [gdt_descriptor] ;; Load GDT descriptor
    mov eax, cr0
    or eax, 0x1 ;; Enable protected mode
    mov cr0, eax
    jmp CODE_SEG:init_protected ;; Far jump

bits 32
init_protected:
    mov ax, DATA_SEG ;; Update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, STACK_OFFSET ;; Reset the stack pointer
    mov esp, ebp

    jmp protected_start ;; Jump back to the bootloader
