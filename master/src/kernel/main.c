#include <onix/onix.h>
#include <onix/types.h>
#include <onix/io.h>

int magic = ONIX_MAGIC;
char message[] = "hello onix!!!!";
char buf[1024];

#define CRT_ADDR_REG 0x3d4
#define CRT_DATA_REG 0x3d5

#define CRT_START_ADDR_H 0xc
#define CRT_START_ADDR_L 0xd

#define CRT_CURSOR_H 0xe
#define CRT_CURSOR_L 0xf

void set_cursor(uint_16 pos){
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    outb(CRT_DATA_REG, (pos>>8)&0xff);
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    outb(CRT_DATA_REG, pos&0xff);
    return ;
}
uint_16 get_cursor(){
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    int_16 pos =inb(CRT_DATA_REG)<<8;
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    pos |= inb(CRT_DATA_REG);
    
    return pos;
}

void kernel_init()
{
    char *video = (char *)0xb8000 ;//文本显示器
    for(int i=0;i<sizeof(message);i++)
    {
        
        *(video+get_cursor()*2)= message[i]; //双字节
        set_cursor(get_cursor()+1); //光标后移
    }  
    return;
}