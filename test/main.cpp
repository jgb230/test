
#include "clientAPI.hpp"
#ifdef WIN32
#include <windows.h>
#endif
#include <thread>
#pragma comment(lib,"clientAPI.lib")

using namespace std;

#define LOG(format, ...) do {\
					 _printTime( __func__, __LINE__);\
                     printf(format"\n", ##__VA_ARGS__);\
                    } while (0)

class GL::Client;
class GL::callBack;

void _printTime(const char *func, long int line);

class mycallBack : public GL::callBack {
	public:
		void  callback(const char *msg) {
			rapidjson::Document document;
			document.Parse<0>(msg);
			LOG("=============content %s, uid %d", document["content"].GetString(), document["uid"].GetInt());
	}
};

int main(){
 
    std::string proid = "jgbtest";
    std::string msg = "清除肾炎标签1";

	GL::clientInfo ci;
	ci.ip = "172.16.0.27";
	ci.port = 2345;
	ci.appId = "4.00002";
	ci.appKey = "Appkey";
	ci.type = 1;
	ci.version = 0x01;
	ci.magic = '$';

    int ret = 0;
	mycallBack *cb = new mycallBack();
    ret = GL::Client::getInstance()->initClient(&ci, cb, (GL::HDL)&mycallBack::callback);
    if (ret != 0 ){
        LOG("intit faile");
		getchar();
        return 0;
    }

    int uid = 0;
    ret = GL::Client::getInstance()->login(proid, &uid);
    if (ret != 0){
        LOG("login faile");
    }
    
    LOG("====udi : %d" ,uid );
	
    for (int i = 0; i < 15; i++){
        int ret = GL::Client::getInstance()->sendMsg(uid, msg);
        if (0 != ret){
            LOG("send mes error: %s errno : %d\n",strerror(ret),ret);
        }
#ifdef WIN32
		Sleep(1000);
#else
		Sleep(1);
#endif
    }
	GL::Client::getInstance()->logout(proid, uid);
	getchar();
    return 0;
}

void _printTime(const char *func, long int line) {
#ifdef WIN32
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	printf("%d %4d-%02d-%02d %02d:%02d:%02d.%ld  %-20s %-6d",
		std::this_thread::get_id(), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, func, line);
#else
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	struct tm tm = *localtime((time_t *)&tv.tv_sec);
	char strTime[30];
	strftime(strTime, 29, "%Y-%m-%d %H:%M:%S", &tm);
	printf("%d %s.%ld %-20s %-6d ", getpid(), strTime, tv.tv_usec, func, line);
#endif
}