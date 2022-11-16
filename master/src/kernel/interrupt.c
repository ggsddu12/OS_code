#include<onix/interrupt.h> 
#include<onix/global.h>
#include<onix/debug.h>

gate_t idt[IDT_SIZE];
pointer_t idt_ptr;   //global.h，结构与gdt指针相同

extern void interrupt_handler();  //中断函数,定义在handler.asm中

void interrupt_init()
{
    for(size_t i=0;i<IDT_SIZE;i++)
    {
        gate_t *gate = &idt[i];
        gate->offset0=(uint_32)interrupt_handler & 0xffff;
        gate->offset1=((uint_32)interrupt_handler>>16) &0xffff;
        gate->selector=1<<3;
        gate->reversed=0;
        gate->type=0b1110;
        gate->segment=0;
        gate->DPL=0; //内核态
        gate->present=1;
    }
    idt_ptr.base=(uint_32)idt;
    idt_ptr.limit=sizeof(idt) - 1;
    BMB;
    asm volatile("lidt idt_ptr\n");
}