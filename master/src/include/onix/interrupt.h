#ifndef ONIX_INTERRUPT_H
#define ONIX_INTERRUPT_H

#include <onix/types.h>

#define IDT_SIZE 256

typedef struct gate_t
{
    uint_16 offset0;
    uint_16 selector;
    uint_8 reserved;
    uint_8 type :4 ;
    uint_8 segment :1;
    uint_8 DPL:2;
    uint_8 present :1;
    uint_16 offset1;
} _packed gate_t;


typedef void * handler_t;//中断处理函数
void interrupt_init();



#endif
