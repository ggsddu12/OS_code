#include<stdio.h>
#define is_digit(c) ((c) >= '0' && (c) <= '9')  //数字


//

static int skip_atoi(const char **s)
{
    int i=0;
    while(is_digit(**s))
        i=i*10 + *((*s)++) - '0';
    return i;
}

int main()
{

    // char *m;
    // char a='a';
    // m=&a;
    // printf("%lx\n",m);
    // printf("%c\n",m);
    const char* n="123456";
    const char* *q=&n;
    int *i;
    int x=100;
    i=&x;
    int c=16;
    printf("%+0*d\n",c,x);
    printf("%d\n",*i);
    printf("%lx\n",n);
    printf("%s\n", n);
    printf("%c\n",*n);
    printf("%s\n", n+1);
    printf("%lx\n",*q);
    printf("%s\n",*q+1);
    printf("%c\n",*(*q+1));
    int b=skip_atoi(&n);
    printf("%d\n",b);
}