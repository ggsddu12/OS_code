[bits 32]

extern kernel_init ; 外部符号


global _start
_start:
    ;mov byte[0xb8000], 'K'
    xchg bx, bx
    call kernel_init 
    jmp $