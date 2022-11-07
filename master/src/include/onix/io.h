#ifndef ONIX_IO_H
#define ONIX_IO_H

#include <onix/types.h>


extern uint_8 inb(uint_16 port);
extern uint_16 inw(uint_16 port);

extern void outb(uint_16 port, uint_8 value);
extern void outw(uint_16 port, uint_16 value);

#endif