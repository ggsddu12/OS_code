[bits 32]

section .text;
global inb ;
inb:
    push ebp;
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp + 8];port
    in al, dx;dx端口的输出到al 1 byte

    jmp $+2;
    jmp $+2;
    jmp $+2; 延迟

    leave ;恢复栈帧
    ret

global outb ;
outb:  
    push ebp
    mov ebp, esp

    mov edx, [ebp + 8] ;port
    mov eax, [ebp + 12] ;value
    out dx, al ;value输出到port 1 byte

    jmp $+2;
    jmp $+2;
    jmp $+2; 延迟

    leave ;恢复栈帧
    ret

global inw ;
inw:
    push ebp
    mov ebp, esp

    mov edx, [ebx+8];port
    in ax, dx ;dx端口的输出到al 1 word

    jmp $+2;
    jmp $+2;
    jmp $+2; 延迟

    leave ;恢复栈帧
    ret

global outw
outw:
    push ebp
    mov ebp, esp

    mov edx, [ebp + 8];port
    mov eax, [ebp + 12];value

    out dx, ax ;value输出到port 1 word

    jmp $+2;
    jmp $+2;
    jmp $+2; 延迟

    leave ;恢复栈帧
    ret


