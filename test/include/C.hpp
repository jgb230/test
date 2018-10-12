#ifndef TEST_C_H
#define TEST_C_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "A.hpp"

class C:public A{
    public:
        C();
        ~C();
        std::string call();
    private:
        std::string me();
};

#endif