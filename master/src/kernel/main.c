#include <onix/onix.h>
#include <onix/types.h>
#include <onix/io.h>
#include <onix/console.h>
#include <onix/stdarg.h>

int magic = ONIX_MAGIC;
char message[] = "hello onix!!!!\n";
char buf[1024];

// #define CRT_ADDR_REG 0x3d4
// #define CRT_DATA_REG 0x3d5


// #define CRT_CURSOR_H 0xe
// #define CRT_CURSOR_L 0xf

void test_args(int cnt, ...)  //可变参数
{
    va_list args;
    va_start(args, cnt);
    int arg;
    while(cnt--)
    {
        arg=va_arg(args,int);  //逐个取出其中的内容
    }
    va_end(args);
}

void kernel_init()
{
    console_init();
    // while(true)
    // {    
    //     console_write(message, sizeof(message)-1);
    //     break;
    // }
    test_args(5, 1, 0xaa, 5, 0x55, 10);
    return;
}