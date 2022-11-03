[org 0x7c00]  ;程序从0x7c00开始



mov ax, 3
int 0x10 ;将显示模式设置为文本


mov ax, 0
mov ds, ax
mov ss, ax
mov es, ax
mov sp, 0x7c00



mov si, booting
call print

mov edi, 0x1000 ; 读取到目标内存
mov ecx, 2 ;起始扇区
mov bl, 4 ;扇区数量
call read_disk

cmp word [0x1000], 0x55aa
jnz error

jmp 0:0x1002

;将上面读出来的内容写到硬盘第二个扇区
; mov edi, 0x1000 ; 目标内存
; mov ecx, 2 ;起始扇区
; mov bl, 1 ;扇区数量
; call write_disk

jmp $

; - 0x1F0：16bit 端口，用于读写数据
; - 0x1F1：检测前一个指令的错误
; - 0x1F2：读写扇区的数量
; - 0x1F3：起始扇区的 0 ~ 7 位
; - 0x1F4：起始扇区的 8 ~ 15 位
; - 0x1F5：起始扇区的 16 ~ 23 位
; - 0x1F6:
;     - 0 ~ 3：起始扇区的 24 ~ 27 位
;     - 4: 0 主盘, 1 从片
;     - 6: 0 CHS, 1 LBA
;     - 5 ~ 7：固定为1
; - 0x1F7: out
;     - 0xEC: 识别硬盘
;     - 0x20: 读硬盘
;     - 0x30: 写硬盘
; - 0x1F7: in / 8bit
;     - 0 ERR
;     - 3 DRQ 数据准备完毕
;     - 7 BSY 硬盘繁忙
read_disk:
    mov dx, 0x1f2  ;0x1f2端口，用于存读写扇区数量
    mov al, bl
    out dx, al ; 将读写数量放入0x1f2端口

    inc dx; 0x1f3,起始扇区0-7
    mov al, cl ;
    out dx, al

    inc dx; 0x1f3,起始扇区8-15
    shr ecx, 8
    mov al, cl ;
    out dx, al

    inc dx; 0x1f3,起始扇区16-23
    shr ecx, 8
    mov al, cl ;
    out dx, al

    inc dx; 0x1f6
    shr ecx, 8
    and cl, 0b1111;高四位置零
    
    mov al, 0b1110_0000 ;主盘，LBA
    or al, cl
    out dx, al

    inc dx;0x1f7
    mov al, 0x20 ;读硬盘
    out dx, al

    xor ecx, ecx
    mov cl, bl
    .read:
        push cx
        call .waits ;等待数据准备完毕
        call .reads ;读一个扇区
        pop cx
        loop .read

    ret
    .waits:
        mov dx, 0x1f7
        .check:
            in al, dx ;0x1f7读到al
            jmp $+2;nop
            jmp $+2;
            jmp $+2;延迟
            and al, 0b1000_1000 ;第三位和第七位 准备完毕/繁忙
            cmp al, 0b0000_1000
            jnz .check
        ret
    .reads:
        mov dx, 0x1F0
        mov cx, 256 ;一个扇区256个字
        .readw:
            in ax, dx
            jmp $+2;nop
            jmp $+2;
            jmp $+2;延迟

            mov [edi], ax;一次读取1个字，即2Byte
            add edi, 2;内存地址+2
            loop .readw
        ret



write_disk:
    mov dx, 0x1f2  ;0x1f2端口，用于存读写扇区数量
    mov al, bl
    out dx, al ; 将读写数量放入0x1f2端口

    inc dx; 0x1f3,起始扇区0-7
    mov al, cl ;
    out dx, al

    inc dx; 0x1f3,起始扇区8-15
    shr ecx, 8
    mov al, cl ;
    out dx, al

    inc dx; 0x1f3,起始扇区16-23
    shr ecx, 8
    mov al, cl ;
    out dx, al

    inc dx; 0x1f6
    shr ecx, 8
    and cl, 0b1111;高四位置零
    
    mov al, 0b1110_0000 ;主盘，LBA
    or al, cl
    out dx, al

    inc dx;0x1f7
    mov al, 0x30 ;写硬盘
    out dx, al

    xor ecx, ecx
    mov cl, bl
    .write:
        push cx
        call .writes ;写一个扇区
        call .waits ;等待数据准备完毕     
        pop cx
        loop .write

    ret
    .waits:
        mov dx, 0x1f7
        .check:
            in al, dx ;0x1f7读到al
            jmp $+2;nop
            jmp $+2;
            jmp $+2;延迟
            and al, 0b1000_0000 ;第三位和第七位 繁忙
            cmp al, 0b0000_0000
            jnz .check
        ret
    .writes:
        mov dx, 0x1F0
        mov cx, 256 ;一个扇区256个字
        .writew:
            mov ax, [edi]
            out dx, ax
            jmp $+2;nop
            jmp $+2;
            jmp $+2;延迟

            add edi, 2;内存地址+2
            loop .writew
        ret
print:
    mov ah, 0x0e
.next:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret
booting:
    db "Booting Onix...", 10 ,13 ,0 ;\n\r

error:
    mov si, .msg
    call print
    hlt
    jmp $
    .msg db "Booting error!!!", 10 , 13 0

times 510 - ($ - $$) db 1
db 0x55, 0xaa
