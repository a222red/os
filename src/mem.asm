bits 32

global memcpy
memcpy:
    push ebx
    push esi

    mov eax, [esp + 4]
    xor esi, esi
    mov ecx, [esp + 8]
    mov edx, [esp + 12]
    push ebx
    xor ebx, ebx
_memcpy_loop:
    cmp esi, edx
    je _memcpy_end
    mov bl, [ecx + esi]
    mov byte [eax + esi], bl
    inc esi
    jmp _memcpy_loop
_memcpy_end:
    pop ebx

    pop esi
    pop ebx
    ret

global memset
memset:
    mov eax, [esp + 4]
    mov ecx, [esp + 8]
    mov edx, [esp + 12]
    push ebx
    xor ebx, ebx
_memset_loop:
    cmp ebx, edx
    je _memset_end
    mov byte [eax + ebx], cl
    inc ebx
    jmp _memset_loop
_memset_end:
    pop ebx
    ret
