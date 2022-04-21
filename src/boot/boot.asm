bits 16

;; Boot sector is at 0x7c00
org 0x7c00

KERNEL_OFFSET equ 0xfffc
STACK_OFFSET equ 0xfff8

;; This label only exists for readability's sake
boot:
    ;; Store the drive # we booted from
    mov [boot_drive], dl

    ;; Zero out some registers
    xor ax, ax
    mov dx, ax
    mov cx, ax
    mov ds, ax

    ;; Setup the stack below the boot sector
    ;; This will be reset when we switch to 32-bit mode
    cli
    mov bp, STACK_OFFSET
    mov ss, ax
    mov sp, bp
    sti

    ;; Load the kernel into memory
    call load_kernel
    ;; Switch to 32-bit mode
    call activate_protected

    jmp $

print_str: ;; di -> Pointer to string (null terminated)
    mov ah, 0x0e
    _print_str_loop:
    mov al, [di]
    test al, al
    jz _print_str_end
    int 0x10
    inc di
    jmp _print_str_loop
    _print_str_end:
    ret

print_char: ;; dil -> Character
    mov ax, di
    mov ah, 0x0e
    int 0x10
    ret

done_str: db "Done!", 0x0d, 0x0a, 0x00

%include "disk.asm"
%include "gdt.asm"
%include "protected.asm"

bits 16

load_kernel:
    mov ax, 0x0000
    mov bx, KERNEL_OFFSET ;; bx -> destination
    mov dh, KERNEL_SECTOR_NUM ;; Build script generates this macro
    mov dl, [boot_drive] ;; dl -> disk #
    jmp disk_load

bits 32

protected_start:
    push dword KERNEL_SECTOR_NUM
    jmp 0xfffc ;; Start the kernel

boot_drive: db 0x80

;; Padding to 512 bytes
    times 510 - ($ - $$) db 0
;; Magic number
    dw 0xaa55
