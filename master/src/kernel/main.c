#include <onix/onix.h>
#include <onix/types.h>
#include <onix/io.h>
#include <onix/console.h>
#include <onix/stdarg.h>
#include <onix/printk.h>
#include <onix/assert.h>
#include <onix/debug.h>
#include <onix/task.h>
#include <onix/interrupt.h>

int magic = ONIX_MAGIC;
char message[] = "hello onix!!!!\n";
char buf[1024];



void kernel_init()
{
    console_init();
    gdt_init();
    interrupt_init();
    //task_init();
    return;
}

//debug test
// void kernel_init()
// {
//     console_init();
//     BMB;
//     char *a="this is debug test";
//     DEBUGK("Debug trap: %s\n",a);
//     return;
// }

//assert 测试
// void kernel_init()
// {
//     console_init();
//     assert(3>5);
//     panic("Out of Memory");
//     return;
// }

// //printk测试
// void kernel_init()
// {
//     console_init();
//     int cnt = 15;
//     while (cnt--)
//     {
//         printk("hello onix %#010x\n", cnt);
//     }
// }

// 可变参数测试
// void test_args(int cnt, ...)  //可变参数
// {
//     va_list args;
//     va_start(args, cnt);
//     int arg;
//     while(cnt--)
//     {
//         arg=va_arg(args,int);  //逐个取出其中的内容
//     }
//     va_end(args);
// }
// void kernel_init()
// {
//     console_init();
//     test_args(5, 1, 0xaa, 5, 0x55, 10);
//     return;
// }


// console测试
// void kernel_init()
// {
//     console_init();
//     while(true)
//     {    
//         console_write(message, sizeof(message)-1);
//     }
//     return;
// }