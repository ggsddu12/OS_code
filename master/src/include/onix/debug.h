#ifndef ONIX_DEBUG_H
#define ONIX_DEBUG_H

void debugk(char* file, int line, const char* fmt,...);

//bochs magic breakpoint : xchg bx, bx
#define BMB asm volatile("xchgw %bx, %bx")

#define DEBUGK(fmt, args...) debugk(__BASE_FILE__, __LINE__, fmt, ##args)

#endif