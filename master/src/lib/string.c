#include <onix/string.h>

char *strcpy(char *dest, const char *src)
{
    char *ptr=dest;
    while(true)
    {
        *ptr++=*src;
        if(*src++==EOS)
            return dest;
    }
}

char *strcat(char *dest, const char *src)  //dest=dest + src ,dest的\0要去掉
{
    char *ptr=dest;
    while(*ptr!=EOS)
        *ptr++;
    while(true)
    {
        *ptr++=*src;
        if(*src++==EOS)
            return dest;
    }
}

size_t strlen(const char *str)
{
    char *ptr=(char *)str;
    while(*ptr!=EOS)
        ptr++;
    return ptr-str;
}
int strcmp(const char *lhs, const char *rhs)
{
    char *l=(char *)lhs;
    char *r=(char *)rhs;
    while(*l!=EOS && *r!=EOS && *l==*r)
    {
        *l++;
        *r++;
    }
    return *l < *r ? -1 : *l > *r;  //返回的是-1 或者*l > *r 的逻辑值， 所以当*l ==*r时，表达式*l>*r为假，返回0； 
}
char *strchr(const char *str, int ch)
{
    char * temp =(char *)str;
    while(true)
    {
        if(*temp==ch)
            return temp;
        if(*temp++==EOS)
            return NULL;
    }
}
char *strrchr(const char *str, int ch)
{
    char *last = NULL;             //从左往右记录下最后一个
    char *ptr = (char *)str;
    while (true)
    {
        if (*ptr == ch)
        {
            last = ptr;
        }
        if (*ptr++ == EOS)
        {
            return last;
        }
    }
}

int memcmp(const void *lhs, const void *rhs, size_t count)
{
    char *l=(char *)lhs;
    char *r=(char *)rhs;
    while(*l==*r && count-- > 0)
    {
        *l++;
        *r++;
    }
    return *l < *r ? -1 : *l > *r;
}



void *memset(void *dest, int ch, size_t count)  //将dest开始的count个内存置为ch
{
    char *ptr = dest;
    while (count--)
    {
        *ptr++ = ch;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *ptr=dest;
    while(count--)
    {
        *ptr++ = *((char*)src++);
    }
    return dest;
}

void *memchr(const void *ptr, int ch, size_t count)
{
    char *temp=ptr;
    while(count--)
    {
        if(*temp==ch)
            return temp;
        *temp++;
    }
    return NULL;
}

