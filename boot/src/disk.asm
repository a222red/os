disk_load_str: db "Loading disk... ", 0x00

disk_load:
    pusha
    push es
    push dx

    mov di, disk_load_str
    call print_str

    ;; Reset disk before reading
    xor ah, ah
    int 0x13

    mov es, ax
    mov ah, 0x02 ;; Read mode
    mov al, dh ;; Read dh number of sectors
    mov cl, 0x02 ;; Start from sector 2 (sector 1 is the boot sector)
    mov ch, 0x00 ;; Cylinder 0
    xor dh, dh ;; Head 0

    ;; dl = drive number is set as input to disk_load
    ;; es:bx = buffer pointer is set as input as well

    int 0x13 ;; BIOS interrupt for disk ops
    jc disk_error ;; Check carry bit for error

    pop dx ;; Get back original number of sectors to read
    cmp al, dh ;; BIOS sets 'al' to the # of sectors actually read
               ;; compare it to 'dh' and error out if they are !=
    jne sector_error

    mov di, done_str
    call print_str

    popa
    pop es
    ret

disk_err_str: db "Disk error", 0x0d, 0x0a, 0x00
sector_err_str: db "Sector error", 0x0d, 0x0a, 0x00

disk_error:
    mov ah, 0x01
    int 0x13 ;; Get last disk error
    mov di, disk_err_str
    call print_str
    jmp $

sector_error:
    mov di, sector_err_str
    call print_str
    jmp $
