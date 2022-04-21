extern memcpy

global itoa

itoa:
    push ebx
    push edi
    push esi

    xor ecx, ecx ;; Digit counter
    mov edi, 1000000000 ;; Divisor
    mov esi, 10 ;; Constant
    mov eax, [esp + 20] ;; Dividend
    mov ebx, [esp + 16]
_itoa_loop:
    xor edx, edx ;; Remainder
    div edi ;; Divide eax by edi and put the remainder in eax
    add al, 0x30 ;; Convert remainder to digit

    mov byte [ebx + ecx], al ;; Copy the digit to the buffer

    inc ecx ;; Increment the counter

    push edx

    xor edx, edx
    mov eax, edi

    div esi ;; Divide edi by 10
    mov edi, eax

    pop eax

    cmp ecx, 10
    jl _itoa_loop

    pop esi
    pop edi
    pop ebx
    ret
