
#include "A.hpp"
#include "jgb_LoadLD.hpp"
#include "loadAPI/loadAPI.hpp"
#include "redis/redisJgb.hpp"
#include "clientAPI.hpp"
using namespace std;

int main(){
   
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
