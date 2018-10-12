
#include "B.hpp"

using namespace std;

B::B(){
}

B::~B(){
}

string B::call(){
    return me();
}

string B::me(){
    return "B";
}

extern "C" A *createService()
{
	return new B();
}

