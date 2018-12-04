#ifndef CLIENT_API
#define CLIENT_API

#include <string>

#ifdef WIN32
    #define DLL_PUBLIC _declspec(dllexport)
#else
    #define DLL_PUBLIC 
#endif

namespace GL{

    #define MAXLEN 65536
    
	extern "C" 
    {
        DLL_PUBLIC int initClient(const std::string &appId, const std::string &appKey, int type = 1);
        DLL_PUBLIC int login(const std::string &proId, int *uid);
        DLL_PUBLIC int sendMsg(int uid, const std::string &msg);
        DLL_PUBLIC int logout(const std::string &proId, int uid);
        DLL_PUBLIC int setRecvHandler(void * hand(const char *));
        DLL_PUBLIC void * callback(const char *msg);
    }
    
}

#endif