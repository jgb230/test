
#include "C.hpp"

using namespace std;

C::C(){
}

C::~C(){
}

string C::call(){
    return me();
}

string C::me(){
    return "C";
}

extern "C" A *createService()
{
	return new C();
}