[org 0x1000]

dw 0x55aa ;魔数，用于判断错误


mov si, booting
call print

;内存检查
dectecting_memory:
    ;int 0x15 设置入参
    xor ebx, ebx ; ebx清空
    mov ax, 0
    mov es, ax
    mov edi, ards_buffer
    mov edx, 0x534d4150
.next:
    mov ecx, 20  ; ards大小
    
    mov eax, 0xe820   ;bios内存检查的子功能号
    int 0x15     ; bios系统调用
    
    ;返回值
    jc error     ; CF=1,出错
    add di, cx   ; ards结构体指针后移
    inc word [ards_count] ; ards数量加1
    cmp ebx, 0   ; 检查结束
    jnz .next    ; 继续检查
    
    mov si, dectecting
    call print   ;打印检查成功信息
    
    jmp prepare_protected_mode

jmp $

prepare_protected_mode:
    cli ;关中断
    ;打开A20线
    in al, 0x92
    or al, 0b10
    out 0x92, al
    
    lgdt [gdt_ptr];加载gdt

    ;启动保护模式
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ;跳转刷新缓存，启用保护模式
    jmp dword code_selector:protected_mode

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
    db "Loading Onix...", 10 ,13 ,0 ;\n\r
dectecting:
    db "Dectecting memory success...", 10, 13, 0 
error:
    mov si, .msg
    call print
    .msg db "Loading error!!!", 10 , 13 0

[bits 32]
protected_mode:

    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x10000

    mov edi, 0x10000 ; 读取内核，内核二进制文件在master.img的10-210扇区
    mov ecx, 10 ;起始扇区10
    mov bl, 200 ;200扇区
    call read_disk ;读
    jmp dword code_selector:0x10000 ;转到内核开始执行
    
    ud2;表示出错，不会执行到这

jmp $

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
    


code_selector equ (1<<3)
data_selector equ (2<<3)

gdt_ptr:
    dw (gdt_end - gdt_base) - 1
    dd gdt_base

memory_base equ 0
memory_limit equ ((1024 * 1024 * 1024 * 4)/(1024 * 4)) - 1

gdt_base:
    dd 0, 0 ;gdt[0]==NULL
gdt_code: ;8Byte
    dw memory_limit & 0xffff   ;// 段界限 0 ~ 15 位
    dw memory_base & 0xffff    ;// 基地址 0 ~ 23 位
    db (memory_base >> 16) & 0xff
    db 0b_1001_1010 ; present=1 DPL=0 segment =1 ,type：代码段不依从，可读，未被访问
    db 0b_1100_0000 | ((memory_limit >> 16) & 0xf) ; 4KB粒度32位 段界限 16 - 19 位
    db (memory_base >> 8) & 0xff ;// 基地址 24 ~ 31 位
gdt_data:
    dw memory_limit & 0xffff   ;// 段界限 0 ~ 15 位
    dw memory_base & 0xffff    ;// 基地址 0 ~ 23 位
    db (memory_base >> 16) & 0xff
    db 0b_1001_0010 ; present=1 DPL=0 segment =1 ,type：数据段不依从，可读，未被访问
    db 0b_1100_0000 | ((memory_limit >> 16) & 0xf) ; 4KB粒度32位 段界限 16 - 19 位
    db (memory_base >> 8) & 0xff ;// 基地址 24 ~ 31 位
gdt_end:


ards_count:
    dw 0
ards_buffer:

