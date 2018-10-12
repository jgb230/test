#ifndef TEST_A_H
#define TEST_A_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

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