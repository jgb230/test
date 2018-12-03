#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

#define TESLOG(level,...) printf(__VA_ARGS__)

#define CC_PROP_REFER(openType, varType, varName, funName, refer)\
openType: varType m_##varName; \
public: inline varType refer get##funName(){ return this->m_##varName;}\
public: inline void set##funName(varType refer var){ this->m_##varName = var;}\
openType:

#define CC_PROP(openType, varType, varName, funName)\
        CC_PROP_REFER(openType, varType, varName, funName,)

std::map<std::string, void*> m_call;

// 成员函数指针不能直接转换成void* ，原因 成员函数指针还包含类的一些信息
// 所以增加转换函数

template <typename F>
void * memfunccastvoid(F f)
{
    void * p = malloc(sizeof(F));
    new(p) F(f);
    return p;
}

class externSYS{

    public:

        externSYS(std::string name, std::string addr, int age){
            setName(name);
            setAddr(addr);
            setAge(age);
        }

        void printName(){
            char outb[64];
            memset(outb, 0, 64);
            sprintf(outb, "name %s", m_name.c_str());
            std::cout << outb << " " << m_name << std::endl;
        }

        void printAddr(std::string & ext){
            char outb[64];
            memset(outb, 0, 64);
            sprintf(outb, "addr %s %s", m_addr.c_str(), ext.c_str());
            std::cout << outb << " " << m_addr << std::endl;
        }

        int printAge(){
            printf("age %d\n", m_age);
            return m_age;
        }

    CC_PROP(public, std::string, name, Name);
    CC_PROP(public, std::string, addr, Addr);
    CC_PROP(public, int, age, Age);

};

typedef void (externSYS::*FUN1)();
typedef int (externSYS::*FUN2)();
typedef void (externSYS::*FUN3)(std::string &);
#define CALLP1(obj, FAG, pcall) (obj.*(*(FAG*)pcall))()
#define CALLP2(obj, FAG, pcall, arg1) (obj.*(*(FAG*)pcall))(arg1)

void loadAPI(){

    FUN1 f1 = &externSYS::printName;
    void *i = memfunccastvoid<FUN1>(f1);
    m_call.insert(std::make_pair("name", i));

    FUN3 f2 = &externSYS::printAddr;
    i = memfunccastvoid<FUN3>(f2);
    m_call.insert(std::make_pair("addr", i));

    FUN2 f3 = &externSYS::printAge;
    i = memfunccastvoid<FUN2>(f3);
    m_call.insert(std::make_pair("age", i));

}

void testAPI(){

    loadAPI();
    externSYS es("jgb", "hz", 28);

    std::string ex = "zj";

    std::cout << es.m_name << std::endl;
    std::cout << es.m_addr << std::endl;
    std::cout << es.m_age  << std::endl;

    es.printName();
    es.printAddr(ex);
    es.printAge();

    auto iter1 = m_call.find("name");
    if (iter1 == m_call.end())
    {
        TESLOG(ERROR, "Not Find szKey=[name] in m_call");
        return;
    }
    void* pCallFun = iter1->second;
    CALLP1(es, FUN1, pCallFun);

    iter1 = m_call.find("addr");
    if (iter1 == m_call.end())
    {
        TESLOG(ERROR, "Not Find szKey=[addr] in m_call");
        return;
    }
    pCallFun = iter1->second;
    CALLP2(es, FUN3, pCallFun, ex);

    iter1 = m_call.find("age");
    if (iter1 == m_call.end())
    {
        TESLOG(ERROR, "Not Find szKey=[age] in m_call");
        return;
    }
    pCallFun = iter1->second;
    int age = CALLP1(es, FUN2, pCallFun);
    std::cout << "cout " << age << std::endl;

}