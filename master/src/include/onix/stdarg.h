#ifndef ONIX_STDARG_H
#define ONIX_STDARG_H

typedef char* va_list;
#define _INTSIZEOF(n) ((sizeof(n)+sizeof(int)-1)&~(sizeof(int)-1))


#define va_start(ap, v) (ap = (va_list)&v + _INTSIZEOF(v))

/*
#define va_arg(ap, t) (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
先执行ap+=_INTSIZEOF(t)，使ap指向下一参数开头
 - 操作获得ap之前指向的数据的地址，然后用*操作取出内容。
*/

#define va_arg(ap, t) (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))//先是ap+=，使ap指向下一参数开头，
#define va_end(ap) (ap = (va_list)0)

#endif