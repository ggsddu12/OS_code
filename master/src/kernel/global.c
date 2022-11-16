//将gdt从loader中搬到内核

#include <onix/global.h>
#include <onix/string.h>
#include <onix/debug.h>
#include <onix/printk.h>


descriptor_t gdt[GDT_SIZE]; //内核全局描述符表
pointer_t gdt_ptr;          //全局描述符指针

void gdt_init()
{
    asm volatile("sgdt gdt_ptr");  //将loader 中gdt指针加载到gdt_ptr
    //从gdt_ptr.base开始，拷贝gdt size（gdt_ptr.limit=gdt.size-1）到新的gdt中
    memcpy(&gdt, (void *)gdt_ptr.base, gdt_ptr.limit + 1);
    gdt_ptr.base=(uint_32)&gdt;
    gdt_ptr.limit=sizeof(gdt)-1;
    asm volatile("lgdt gdt_ptr\n"); //装载gdt
}