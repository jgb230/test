#ifndef TEST_B_H
#define TEST_B_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "A.hpp"

class B:public A{
    public:
        B();
        ~B();
        std::string call();
    private:
        std::string me();
};


#endif