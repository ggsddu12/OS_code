[bits 32]

extern kernel_init ; 外部符号


global _start
_start:
    ;mov byte[0xb8000], 'K'
    call kernel_init 
    xchg bx, bx
    int 0x80
    ; xchg bx, bx
    ; mov bx, 0;
    ; xchg bx, bx
    ; div bx      ;除0异常
    ; mov ax, 0x20
    ; mov cs, ax    ;无效操作码异常
    jmp $