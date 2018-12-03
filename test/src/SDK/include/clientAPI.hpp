#ifndef CLIENT_API
#define CLIENT_API

#include <string>
#ifdef WIN32
#endif
namespace GL{

    #define MAXLEN 65536
    
#ifdef WIN32
	extern "C" _declspec(dllexport) int initClient(const std::string &appId, const std::string &appKey, int type = 1);
	extern "C" _declspec(dllexport) int login(const std::string &proId, int *uid);
	extern "C" _declspec(dllexport) int sendMsg(int uid, const std::string &msg);
	extern "C" _declspec(dllexport) int logout(const std::string &proId, int uid);
	extern "C" _declspec(dllexport) int setRecvHandler(void * hand(const char *));
	extern "C" _declspec(dllexport) void * callback(const char *msg);
    #else
    extern "C"  int initClient(const std::string &appId, const std::string &appKey, int type = 1);
	extern "C"  int login(const std::string &proId, int *uid);
	extern "C"  int sendMsg(int uid, const std::string &msg);
	extern "C"  int logout(const std::string &proId, int uid);
	extern "C"  int setRecvHandler(void * hand(const char *));
	extern "C"  void * callback(const char *msg);
    #endif
}

#endif