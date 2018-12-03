#ifndef CLIENT_MSG
#define CLIENT_MSG

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <rapidjson/rapidjson_helper.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctime>
#include <mutex>
#include <condition_variable>

namespace GL{

    #define LOG(format,...) do {struct timeval tv; \
                     struct timezone tz;   \
                     gettimeofday(&tv,&tz);\
                     struct tm tm = *localtime((time_t *)&tv.tv_sec);\
                     char strTime[30];\
                     strftime(strTime, 29, "%Y-%m-%d %H:%M:%S", &tm);\
                     printf("%d %s.%ld %-20s %-6d ", getpid(), strTime, tv.tv_usec, __func__, __LINE__);\
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

            // pthread_mutex_t *m_ulock;
            // pthread_cond_t *m_ucond;

            std::string m_randomSeed;
            int m_result;
            // pthread_mutex_t *m_slock;
            // pthread_cond_t *m_scond;

            std::mutex *m_ulock; // 全局互斥锁.
			std::condition_variable *m_ucond; // 全局条件变量.
            std::mutex *m_slock; // 全局互斥锁.
			std::condition_variable *m_scond; // 全局条件变量.
    };
    

}

#endif