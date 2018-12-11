
#include "A.hpp"
#include "jgb_LoadLD.hpp"
#include "loadAPI/loadAPI.hpp"
#include "redis/redisJgb.hpp"
#include "clientAPI.hpp"
using namespace std;

int main(){
    #if 0
    A *sa = new A();
    printf("1 %s\n",sa->call().c_str());

    typename LoadLD<A>::funcp1 f21;
    string libPath = "/home/jgb/c++/test/lib/libB.so";
    LoadLD<A> *ld = new LoadLD<A>(libPath);
    f21 = ld->getSO();
    if(f21 != NULL){
        sa = f21();
        printf("21 %s\n",sa->call().c_str());
    }

    libPath = "/home/jgb/c++/test/lib/libC.so";
    ld = new LoadLD<A>(libPath);
    f21 = ld->getSO();
    if(f21 != NULL){
        sa = f21();
        printf("31 %s\n",sa->call().c_str());
    }


//---------------------------------------------------------
    typedef A*(*funcp12)();
    typedef A*(func12)();

    A* (*f1)();
    boost::dll::shared_library *lib;
    libPath = "/home/jgb/c++/test/lib/libB.so";
    lib = new boost::dll::shared_library(libPath);
 
    if (lib->has("createService")) {
        f1 = lib->get<A * ()>("createService");
        sa = f1();
        printf("2 %s\n",sa->call().c_str());
    }

    if(lib->is_loaded())
    {
        lib->unload();
    }

    libPath = "/home/jgb/c++/test/lib/libC.so";
    lib = new boost::dll::shared_library(libPath);
 
    if (lib->has("createService")) {
        f1 = lib->get<func12>("createService");
        sa = f1();
        printf("3 %s\n",sa->call().c_str());
    }

    if(lib->is_loaded())
    {
        lib->unload();
    }
    delete lib;

    int testa[10] = { 0 };
    for (int i = 1; i <= 10; ++i){
        testa[i-1] = i;
    }

#define TEST(OFF) testa + OFF

    for (int i = 1; i <= 10; ++i){
        printf("cout %d\n",*TEST(i - 1));
    }
    testAPI();

    redis_test();
#endif
    std::string appId = "4.00002";
    std::string appKey = "Appkey";
    std::string proid = "jgbtest";
    std::string msg = "清除肾炎标签1";
    int ret = 0;
    ret = GL::initClient(appId, appKey);
    if (ret != 0 ){
        LOG("intit faile");
        return 0;
    }
    int uid = 0;
    ret = GL::login(proid, &uid);
    if (ret != 0){
        LOG("login faile");
    }
    
    LOG("====udi : %d" ,uid );
    GL::setRecvHandler(&GL::callback);
    for (int i = 0; i < 15; i++){
        ret = GL::sendMsg(uid, msg);
        if (0 != ret){
            LOG("send mes error: %s errno : %d\n",strerror(ret),ret);
        }
        sleep(1);
    }
    GL::logout(proid, uid);

    return 0;
}
