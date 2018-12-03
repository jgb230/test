#ifndef CLIENT_MSG
#define CLIENT_MSG
#include "md5.h"
#include <rapidjson_helper.h>

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <fcntl.h>
#ifdef WIN32
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#define Sleep sleep
#define closesocket close
#endif
#include <sys/types.h>
#include <ctime>
#include <string.h>
#include <time.h>
#include <mutex>
#include <condition_variable>

namespace GL{

    #define LOG(format, ...) do {\
					 printTime( __func__, __LINE__);\
                     printf(format"\n", ##__VA_ARGS__);\
                    } while (0)

    #define TIMEBEGIN(num)  struct timeval tv##num;\
                     struct timezone tz##num;\
                     gettimeofday(&tv##num,&tz##num);\
                     long int beginTime##num =  tv##num.tv_sec*1000000 + tv##num.tv_usec;\
                     printf("微妙，beginTime: %ld\n",beginTime##num);

    #define TIMEEND(num) gettimeofday(&tv##num,&tz##num);\
                     long int endTime##num =  tv##num.tv_sec*1000000 + tv##num.tv_usec;\
                     printf("微妙，endTime: %ld; 微妙，runTime:%ld\n",\
                            endTime##num, endTime##num - beginTime##num);

    #pragma pack(push,1)//一字节对齐
    typedef struct HEAD
    {
        uint32_t PkgLen;
        uint32_t CheckSum;
        uint16_t Command;
        uint8_t Version;
        uint8_t Magic;
        uint16_t Retcode;
    }HEAD,*PHEAD;
    #pragma pack(pop)

    class clientMsg{            
        public:
            clientMsg();
            ~clientMsg();
            int heartBeat();
            int servLogin(const std::string &appId, int type);
            int servAuth(const std::string &appId,const std::string &appKey);
            int aichat(int uid,const std::string &msg,const std::string &appId);
            int login(const std::string &appId,const std::string &proId, int *uid);
            int logout(const std::string &appId,const std::string &proId, int uid);
            int sendMsg(const int commond, rapidjson::Document &jsonDoc);

            int reconnect();
            int getSocket(){return m_socket;}
            void setUid(int uid, int result);
            void setAuth(int result);
            void setSeed(const std::string seed, int result);
        private:
            long m_heartTime;
            int  m_socket;

            int m_uid;
			std::mutex *m_ulock; // 全局互斥锁.
			std::condition_variable *m_ucond; // 全局条件变量.

            std::string m_randomSeed;
            int m_result;

			std::mutex *m_slock; // 全局互斥锁.
			std::condition_variable *m_scond; // 全局条件变量.

    };
    

}


#endif