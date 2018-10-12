
#include "A.hpp"

using namespace std;

A::A(){
}

A::~A(){
}

string A::call(){
        return me();
}

string A::me(){
        return "A";
}

