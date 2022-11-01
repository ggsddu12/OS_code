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
.next
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
dectecting:
    db "Dectecting memory success...", 10, 13, 0 

error:
    mov si, .msg
    call print
    hlt
    jmp $
    .msg db "Loading error!!!", 10 , 13 0

ards_count:
    dw 0
ards_buffer:

