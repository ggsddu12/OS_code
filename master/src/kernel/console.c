#include <onix/console.h>
#include <onix/io.h>
#include <onix/string.h>

#define CRT_ADDR_REG 0x3D4 // CRT(6845)索引寄存器
#define CRT_DATA_REG 0x3D5 // CRT(6845)数据寄存器

#define CRT_START_ADDR_H 0xC // 显示内存起始位置 - 高位
#define CRT_START_ADDR_L 0xD // 显示内存起始位置 - 低位
#define CRT_CURSOR_H 0xE     // 光标位置 - 高位
#define CRT_CURSOR_L 0xF     // 光标位置 - 低位

#define MEM_BASE 0xB8000              // 显卡内存起始位置
#define MEM_SIZE 0x4000               // 显卡内存大小
#define MEM_END (MEM_BASE + MEM_SIZE) // 显卡内存结束位置
#define WIDTH 80                      // 屏幕文本列数
#define HEIGHT 25                     // 屏幕文本行数
#define ROW_SIZE (WIDTH * 2)          // 每行字节数
#define SCR_SIZE (ROW_SIZE * HEIGHT)  // 屏幕字节数

#define ASCII_NUL 0x00
#define ASCII_ENQ 0x05
#define ASCII_BEL 0x07 // \a
#define ASCII_BS 0x08  // \b  退格，光标回退1
#define ASCII_HT 0x09  // \t
#define ASCII_LF 0x0A  // \n  line feed, 换行
#define ASCII_VT 0x0B  // \v
#define ASCII_FF 0x0C  // \f  换页
#define ASCII_CR 0x0D  // \r  carriage return, 回车，回到当前行首
#define ASCII_DEL 0x7F // delete

static uint_32 screen; // 当前显示器开始的内存位置

static uint_32 pos; // 记录当前光标的内存位置

static uint_32 x, y; // 当前光标的坐标

static uint_8 attr = 7;        // 字符样式 0111
static uint_16 erase = 0x0720; // 空格

static void get_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);//把光标高位位置给地址寄存器，那么就可以通过数据寄存器得到和设置光标位置的高位值
    screen=inb(CRT_DATA_REG)<<8;
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    screen |=inb(CRT_DATA_REG);

    screen<<1;
    screen+=MEM_BASE;
}
static void set_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 9) & 0xff);
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 1) & 0xff);
}

static void get_cursor()
{
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    pos=inb(CRT_DATA_REG)<<8;
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    pos|=inb(CRT_DATA_REG);

    get_screen();

    pos<<1;
    pos+=MEM_BASE;

    uint_32 delta=(pos-screen)>>1;
    x=delta%WIDTH;
    y=delta/WIDTH;
}

static void set_cursor()
{   
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 9) & 0xff);
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 1) & 0xff);
}

void console_clear()
{
    screen=MEM_BASE;
    pos=MEM_BASE;
    x=y=0;
    set_cursor();
    set_screen();

    uint_16 *ptr=(uint_16*)MEM_BASE;
    while(ptr<(uint_16*)MEM_END)
    {
        *ptr++=erase;
    }
}

static void scroll_up()
{
    if(screen + SCR_SIZE + ROW_SIZE < MEM_END)  //显存没满
    {
        uint_32 *ptr=(uint_32 *)(screen + SCR_SIZE);
        for(size_t i = 0;i<WIDTH;i++) //清空最后一行的下一行
        {
            *ptr++ = erase;
        }
        screen += ROW_SIZE;  //屏幕位置加一行
        pos += ROW_SIZE;     //光标位置加一行
    }
    else //显存满了，从头开始显示
    {
        memcpy((void*)MEM_BASE, (void *)screen, SCR_SIZE);  //拷贝到开头
        pos -= (screen - MEM_BASE);  //光标减
        screen = MEM_BASE;           //屏幕位置回到开头
    }
    set_screen();
}

static void command_lf()  //换行
{
    if(y+1<HEIGHT)        //屏幕没满
    {
        y++;
        pos += ROW_SIZE;  //光标移到下一行
        return;
    }
    scroll_up();          //屏幕满了，滚屏
}

static void command_cr()  //光标回到行首
{
    pos-=(x<<1);
    x=0;
}

static void command_bs()
{
    if(x)
    {
        x--;
        pos-=2;
        *(uint_16*)pos=erase;
    }
}

static void command_del()
{
    *(uint_16*)pos=erase;
}

void console_write(char *buf, uint_32 count)
{
    char ch;
    //char *ptr = (char *)pos;
    while (count--)
    {
        ch = *buf++;
        switch (ch)
        {
        case ASCII_NUL:
            break;
        case ASCII_BEL:
            // todo \a
            break;
        case ASCII_BS:
            command_bs();
            break;
        case ASCII_HT:
            break;
        case ASCII_LF:
            command_lf();
            command_cr();
            break;
        case ASCII_VT:
            break;
        case ASCII_FF:
            command_lf();
            break;
        case ASCII_CR:
            command_cr();
            break;
        case ASCII_DEL:
            command_del();
            break;
        default:
            if (x >= WIDTH)
            {
                x -= WIDTH;
                pos -= ROW_SIZE;
                command_lf();
            }

            *((char*)pos) = ch;
            pos++;
            *((char*)pos) = attr;
            pos++;

            x++;
            break;
        }
    }
    set_cursor();
}

void console_init()
{
    console_clear();
}