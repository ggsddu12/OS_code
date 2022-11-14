#ifndef ONIX_TASK_H
#define ONIX_TASK_H

#include<onix/types.h>

typedef uint_32 target_t();

typedef struct task_t
{
    uint_32 *stack; //内核栈顶
}task_t;

typedef struct task_frame_t
{
    uint_32 edi;
    uint_32 esi;
    uint_32 ebx;
    uint_32 ebp;
    void(*eip)(void);
}task_frame_t;

void task_init();
#endif