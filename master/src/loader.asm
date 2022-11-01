[org 0x1000]

dw 0x55aa ;魔数，用于判断错误

xchg bx, bx

mov si, booting
call print


jmp $

print:
    mov ah, 0x0e
.next 
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret
booting:
    db "Loading Onix...", 10 ,13 ,0 ;\n\r