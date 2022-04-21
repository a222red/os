bits 32

extern lf_offset
extern memcpy
extern memset

VID_MEM equ 0x000b8000
VGA_CTRL equ 0x3d4
VGA_DATA equ 0x3d5
VGA_OFFSET_LOW equ 0x0f
VGA_OFFSET_HIGH equ 0x0e
MAX_ROWS equ 25
MAX_COLS equ 80

global set_cursor
set_cursor:
    mov ecx, [esp + 4]
    mov dx, VGA_CTRL
    mov al, VGA_OFFSET_HIGH
    out dx, al
    mov dx, VGA_DATA
    mov ax, cx
    shr ax, 8
    out dx, al
    mov dx, VGA_CTRL
    mov al, VGA_OFFSET_LOW
    out dx, al
    mov dx, VGA_DATA
    mov ax, cx
    out dx, al
    ret

global get_cursor
get_cursor:
    mov dx, VGA_CTRL
    mov al, VGA_OFFSET_HIGH
    out dx, al
    mov dx, VGA_DATA
    in al, dx
    shl eax, 8
    xor ecx, ecx
    mov cx, ax
    mov dx, VGA_CTRL
    mov al, VGA_OFFSET_LOW
    out dx, al
    mov dx, VGA_DATA
    in al, dx
    and eax, 0xff
    or eax, ecx
    ret

global set_char
set_char:
    mov eax, [esp + 4]
    mov ecx, [esp + 8]
    mov edx, [esp + 12]
    mov byte [VID_MEM + edx], al
    mov byte [(VID_MEM + 1) + edx], cl
    ret

global putc
putc:
    call get_cursor
    mov edx, eax
    shl edx, 1
    mov eax, [esp + 4]
    mov ecx, [esp + 8]
    cmp al, 0x0a
    je _putc_lf
    mov byte [VID_MEM + edx], al
    mov byte [VID_MEM + edx + 1], cl
    add edx, 2
    jmp _putc_end
_putc_lf:
    push edx
    call lf_offset
    add esp, 4
    mov edx, eax
_putc_end:
    shr edx, 1
    push edx
    call set_cursor
    add esp, 4
    ret

global puts
puts:
    call get_cursor
    mov edx, eax
    shl edx, 1
    mov eax, [esp + 4]
    mov ecx, [esp + 8]
    push esi
    xor esi, esi
    push ebx
_puts_loop:
    cmp edx, (MAX_ROWS * (MAX_COLS * 2))
    jge _puts_scroll
    mov bl, [eax + esi]
    test bl, bl
    jz _puts_end
    cmp bl, 0x0a
    je _puts_lf
    mov byte [VID_MEM + edx], bl
    mov byte [VID_MEM + edx + 1], cl
    inc esi
    add edx, 2
    jmp _puts_loop
_puts_lf:
    push eax
    push ecx
    push edx
    call lf_offset
    mov edx, eax
    add esp, 4
    pop ecx
    pop eax
    inc esi
    jmp _puts_loop
_puts_scroll:
    push eax
    push ecx
    push esi
    push dword 1
    call scroll
    add esp, 4
    pop esi
    pop ecx
    pop eax
    mov edx, ((MAX_ROWS - 1) * (MAX_COLS * 2))
    jmp _puts_loop
_puts_end:
    shr edx, 1
    push edx
    call set_cursor
    add esp, 4
    pop ebx
    pop esi
    ret

global scroll
scroll:
    push dword 4000
    mov eax, 160
    mul dword [esp + 8]
    ;; TODO: This copies more data than it needs to,
    ;; but I'm too tired to write an algorithm to fix it
    add eax, 0x000b8000
    push eax
    push dword VID_MEM
    call memcpy
    add esp, 12
    ret

global clear
clear:
    xor eax, eax
_clear_loop:
    cmp eax, 4000
    je _clear_end
    mov byte [VID_MEM + eax], ' ',
    mov byte [(VID_MEM + 1) + eax], 7
    add eax, 2
    jmp _clear_loop
_clear_end:
    push dword 0
    call set_cursor
    add eax, 4
    ret
