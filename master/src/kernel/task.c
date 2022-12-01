#include <onix/task.h>
#include <onix/printk.h>
#include <onix/debug.h>

#define PAGE_SIZE 0x1000

task_t *a = (task_t *)0x1000;
task_t *b = (task_t *)0x2000;

extern void task_switch(task_t *next);

task_t *running_task()
{
    // DEBUGK("in running task!!!\n");
    // BMB;
    asm volatile(
        "movl %esp, %eax\n"
        "andl $0xfffff000, %eax\n");
}

void schedule()
{
    // DEBUGK("in schedule!!!\n");
    // BMB;
    task_t *current = running_task();
    task_t *next = current == a ? b : a;
    task_switch(next);
}

uint_32 _ofp thread_a()
{
    asm volatile("sti\n");
    while (true)
    {
        printk("A");
        schedule();
    }
}

uint_32 _ofp thread_b()
{
    // DEBUGK("Thread_b!!!\n");
    // BMB;
    asm volatile("sti\n");
    while (true)
    {
        printk("B");
        schedule();
    }
}



static void task_create(task_t *task, target_t target)
{
    DEBUGK("task_create !!!\n");
    BMB;
    uint_32 stack = (uint_32)task + PAGE_SIZE;

    stack -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)stack;
    frame->ebx = 0x11111111;
    frame->esi = 0x22222222;
    frame->edi = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void *)target;

    task->stack = (uint_32 *)stack;
}

void task_init()
{
    task_create(a, thread_a);
    //DEBUGK("task A create over !!!\n");
    task_create(b, thread_b);
    // DEBUGK("task B create over !!!\n");
    // DEBUGK("task_init schedule!!!\n");
    // BMB;
    schedule();
}