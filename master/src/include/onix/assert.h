#ifndef ONIX_ASSERT_H
#define ONIX_ASSERT_H

void assertion_failure(char *exp, char *file, char *base, int line);

#define assert(exp) \
    if (exp)        \
        ;           \
    else            \
        assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)  

//#用于将#后的内容转换为字符串，即加了双引号

void panic(const char *fmt, ...);

#endif