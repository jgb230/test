
#include "clientAPI.hpp"
#include <windows.h>
#include <thread>
#pragma comment(lib,"clientAPI.lib")

using namespace std;

#define LOG(format, ...) do {\
					 _printTime( __func__, __LINE__);\
                     printf(format"\n", ##__VA_ARGS__);\
                    } while (0)

extern "C" _declspec(dllexport) int GL::initClient(const std::string &appId, const std::string &appKey, int type );
extern "C" _declspec(dllexport) int GL::login(const std::string &proId, int *uid);
extern "C" _declspec(dllexport) int GL::sendMsg(int uid, const std::string &msg);
extern "C" _declspec(dllexport) int GL::logout(const std::string &proId, int uid);
extern "C" _declspec(dllexport) int GL::setRecvHandler(void * hand(const char *));
extern "C" _declspec(dllexport) void * GL::callback(const char *msg);

void _printTime(const char *func, long int line);

int main(){
    
    std::string appId = "4.00002";
    std::string appKey = "Appkey";
    std::string proid = "jgbtest";
    std::string msg = "清除肾炎标签1";
    int ret = 0;
    ret = GL::initClient(appId, appKey);
    if (ret != 0 ){
        LOG("intit faile");
		getchar();
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
        int ret = GL::sendMsg(uid, msg);
        if (0 != ret){
            LOG("send mes error: %s errno : %d\n",strerror(ret),ret);
        }
#ifdef WIN32
		Sleep(1000);
#else
		Sleep(1);
#endif
    }
    GL::logout(proid, uid);
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