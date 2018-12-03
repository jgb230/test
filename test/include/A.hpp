#ifndef TEST_A_H
#define TEST_A_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

#define LOG(format,...) do {struct timeval tv; \
                     struct timezone tz;   \
                     gettimeofday(&tv,&tz);\
                     struct tm tm = *localtime((time_t *)&tv.tv_sec);\
                     char strTime[30];\
                     strftime(strTime, 29, "%Y-%m-%d %H:%M:%S", &tm);\
                     printf(" %s.%ld %-20s %-6d ", strTime, tv.tv_usec, __func__, __LINE__);\
                     printf(format"\n", ##__VA_ARGS__);\
                    } while (0)

class A{
    int age;
    public:
        A();
        ~A();
        virtual std::string call();
    private:
        virtual std::string me();
};

#endif