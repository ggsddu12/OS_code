global task_switch
task_switch:
    push ebp
    mov ebp, esp

    push ebx
    push esi
    push edi

    mov eax, esp;
    and eax, 0xfffff000 ;current

    mov [eax], esp      ;保存当前进程的esp

    mov eax, [ebp + 8]  ;next
    mov esp, [eax]      ;恢复到next进程的esp

    pop edi
    pop esi
    pop ebx
    pop ebp         ;从next进程的栈中恢复栈帧，恢复到next进程

    ret