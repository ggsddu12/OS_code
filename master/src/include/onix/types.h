#ifndef ONIX_TYPES_H
#define ONIX_TYPES_H

#define EOF -1 //文件结尾
#define EOS '\0' //字符串结尾

#define NULL 0 //空指针
#define NULL ((void*)0) //空指针

#define bool _Bool
#define true 1
#define false 0


//特殊结构体
#define _packed __attribute__((packed)) 

// 用于省略函数的栈帧
#define _ofp __attribute__((optimize("omit-frame-pointer")))

typedef unsigned int size_t;
typedef char int_8;
typedef short int_16;
typedef int int_32;
typedef long long int_64;

typedef unsigned char uint_8;
typedef unsigned short uint_16;
typedef unsigned int uint_32;
typedef unsigned long long uint_64;

#endif