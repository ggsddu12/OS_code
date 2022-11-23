#include <onix/stdlib.h>

void delay(uint_32 count)
{
    while (count--)
        ;
}

void hang()
{
    while (true)
        ;
}