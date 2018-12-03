#include "../include/clientMsg.hpp"
#include "md5.h"
#include <ctype.h>

namespace GL{

    const  int         g_brokerPort = 2345;
    const  char       *g_brokerIP = "172.16.0.27";
    const  uint8_t     g_version = 0x01;
    const  uint8_t     g_magic = '$'; 

    clientMsg::clientMsg(){
        struct sockaddr_in sockaddr;
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        memset(&sockaddr, 0, sizeof(sockaddr));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(g_brokerPort);
        inet_pton(AF_INET, g_brokerIP, &sockaddr.sin_addr);
        if((connect(m_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) < 0 )
        {
            LOG("connect error %s errno: %d",strerror(errno),errno);
        }
        LOG("connect ");

        signal(SIGPIPE,SIG_IGN);

        // m_ulock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        // m_ucond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
        // pthread_mutex_init(m_ulock, NULL);
        // pthread_cond_init(m_ucond, NULL);

        // m_slock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        // m_scond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
        // pthread_mutex_init(m_slock, NULL);
        // pthread_cond_init(m_scond, NULL);
        m_ulock = new std::mutex();
		m_ucond = new std::condition_variable();

		m_slock = new std::mutex();
		m_scond = new std::condition_variable();
        m_result = 0;
        m_randomSeed = "";
        m_heartTime = 0;
        m_uid = 0;
    }

    int clientMsg::reconnect(){
        struct sockaddr_in sockaddr;
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        memset(&sockaddr, 0, sizeof(sockaddr));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(g_brokerPort);
        inet_pton(AF_INET, g_brokerIP, &sockaddr.sin_addr);
        int time = 0;
        int ret = 0;
        while((ret = connect(m_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) < 0)
        {
            ++time;
            sleep(1);
            if (time >= 3){
                LOG("reconnect time: %d ret: %d error %s errno: %d", time, ret, strerror(errno),errno);
                return -1;
            }
        }

        LOG(" reconnect !");
        return 0;
    }

    clientMsg::~clientMsg(){
        close(m_socket);
        LOG(" ");
    }

    int clientMsg::heartBeat(){
        m_heartTime = time(NULL);

        //生成heartbeat Json串
        rapidjson::Document jsonDoc;    //生成一个dom元素Document
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator(); //获取分配器
        jsonDoc.SetObject();    //将当前的Document设置为一个object，也就是说，整个Document是一个Object类型的dom元素
        jsonDoc.AddMember("nowTime", 0, allocator);

        return sendMsg(9999, jsonDoc);
    }

    int clientMsg::servLogin(const std::string &appId, int type){
        
        //生成heartbeat Json串
        rapidjson::Document jsonDoc;    //生成一个dom元素Document
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator(); //获取分配器
        jsonDoc.SetObject();    //将当前的Document设置为一个object，也就是说，整个Document是一个Object类型的dom元素
        jsonDoc.AddMember("servType", type, allocator);
        jsonDoc.AddMember("appid", appId, allocator);

        int ret = sendMsg(201, jsonDoc);
        if (ret < 0){
            LOG("send mes error: %s errno : %d",strerror(ret),ret);
            return ret;
        }else if( ret == 0){
            LOG("send socket close ");
            return -1;
        }
        // pthread_mutex_lock(m_slock);
        // pthread_cond_wait(m_scond, m_slock);
        // ret = m_result;
        // pthread_mutex_unlock(m_slock);

        std::unique_lock <std::mutex> lck(*m_slock);
		m_scond->wait(lck);
		ret = m_result;
        return ret == 1? 0: -1;
    }   

    int clientMsg::servAuth(const std::string &appId,const std::string &appKey){
        std::string data = m_randomSeed + appKey; 

        std::string sign = md5(data);
        //生成heartbeat Json串
        rapidjson::Document jsonDoc;    //生成一个dom元素Document
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator(); //获取分配器
        jsonDoc.SetObject();    //将当前的Document设置为一个object，也就是说，整个Document是一个Object类型的dom元素
        jsonDoc.AddMember("sign", sign, allocator);
        jsonDoc.AddMember("appid", appId, allocator);



        int ret = sendMsg(202, jsonDoc);
        if (ret < 0){
            LOG("send mes error: %s errno : %d",strerror(ret),ret);
            return ret;
        }else if( ret == 0){
            LOG("send socket close ");
            return -1;
        }
        std::unique_lock <std::mutex> lck(*m_slock);
		m_scond->wait(lck);
		ret = m_result;
        // pthread_mutex_lock(m_slock);
        // pthread_cond_wait(m_scond, m_slock);
        // ret = m_result;
        // pthread_mutex_unlock(m_slock);
        
        return ret == 1? 0: -1;
    }   


    int clientMsg::aichat(int uid,const std::string &msg,const std::string &appId){
        
        //生成heartbeat Json串
        rapidjson::Document jsonDoc;    //生成一个dom元素Document
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator(); //获取分配器
        jsonDoc.SetObject();    //将当前的Document设置为一个object，也就是说，整个Document是一个Object类型的dom元素
        jsonDoc.AddMember("uid", uid, allocator);
        jsonDoc.AddMember("content", msg, allocator);
        jsonDoc.AddMember("appid", appId, allocator);

        int ret = sendMsg(801, jsonDoc);
        if (ret < 0){
            LOG("send mes error: %s errno : %d",strerror(ret),ret);
            return ret;
        }else if( ret == 0){
            LOG("send socket close ");
            return -1;
        }
        
        return 0;
    }   

    int clientMsg::login(const std::string &appId,const std::string &proId, int *uid){

        //生成heartbeat Json串
        rapidjson::Document jsonDoc;    //生成一个dom元素Document
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator(); //获取分配器
        jsonDoc.SetObject();    //将当前的Document设置为一个object，也就是说，整个Document是一个Object类型的dom元素
        jsonDoc.AddMember("uname", proId, allocator);
        jsonDoc.AddMember("passwd", "", allocator);
        jsonDoc.AddMember("keytp", "openid", allocator);
        jsonDoc.AddMember("appid", appId, allocator);
        jsonDoc.AddMember("seq", proId, allocator);

        int ret = sendMsg(2, jsonDoc);
        if (ret < 0){
            LOG("send mes error: %s errno : %d",strerror(ret),ret);
            return ret;
        }else if( ret == 0){
            LOG("send socket close ");
            return -1;
        }
        std::unique_lock <std::mutex> lck(*m_ulock);
		m_ucond->wait(lck);
        *uid = m_uid;
		ret = m_result;
        // pthread_mutex_lock(m_ulock);
        // pthread_cond_wait(m_ucond, m_ulock);
        // *uid = m_uid;
        // ret = m_result;
        // pthread_mutex_unlock(m_ulock);
        
        return ret == 1? 0:ret ;
    }

    int clientMsg::logout(const std::string &appId,const std::string &proId, int uid){

        //生成heartbeat Json串
        rapidjson::Document jsonDoc;    //生成一个dom元素Document
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator(); //获取分配器
        jsonDoc.SetObject();    //将当前的Document设置为一个object，也就是说，整个Document是一个Object类型的dom元素
        jsonDoc.AddMember("uid", uid, allocator);
        jsonDoc.AddMember("appid", appId, allocator);
        jsonDoc.AddMember("seq", proId, allocator);

        int ret = sendMsg(4, jsonDoc);
        if (ret < 0){
            LOG("send mes error: %s errno : %d",strerror(ret),ret);
            return ret;
        }else if( ret == 0){
            LOG("send socket close ");
            return -1;
        }
        
        return 0;
    }

    void clientMsg::setUid(int uid, int result){
        std::unique_lock <std::mutex> lck(*m_ulock);
		m_uid = uid;
		m_result = result;
		m_ucond->notify_all();
        // pthread_mutex_lock(m_ulock);    
        // m_uid = uid;
        // m_result = result;
        // pthread_cond_signal(m_ucond);
        // pthread_mutex_unlock(m_ulock);
    }

    void clientMsg::setSeed(const std::string seed, int result){

        std::unique_lock <std::mutex> lck(*m_slock);
		m_randomSeed = seed;
		m_result = result;
		m_scond->notify_all();
        // pthread_mutex_lock(m_slock);    
        // m_randomSeed = seed;
        // m_result = result;
        // pthread_cond_signal(m_scond);
        // pthread_mutex_unlock(m_slock);
    }

    void clientMsg::setAuth(int result){
        std::unique_lock <std::mutex> lck(*m_slock);
		m_result = result;
		m_scond->notify_all();
        // pthread_mutex_lock(m_slock); 
        // m_result = result;  
        // pthread_cond_signal(m_scond);
        // pthread_mutex_unlock(m_slock);
    }

    int clientMsg::sendMsg(const int commond, rapidjson::Document &jsonDoc){

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        jsonDoc.Accept(writer);
    
        std::string strJson = buffer.GetString();

        LOG("发送的Json: %s", strJson.c_str());
        int len = strJson.length();
        HEAD *head = new HEAD;
        head->Command = commond;
        head->Version = g_version;
        head->Magic = g_magic;
        head->PkgLen = 10 + len;
        int ret = 0;
        if (m_socket){
            LOG("PkgLen: %d" ,head->PkgLen);

            char *buf = (char *)malloc(head->PkgLen + 4);
            memcpy(buf, head, 14);
            memcpy(buf + 14, strJson.c_str(), len);
            ret = send(m_socket, buf, head->PkgLen + 4,0);
            if(ret < 0)
            {
                LOG("send mes error: %s errno : %d",strerror(errno),errno);
                return errno;
            }else if (ret == 0){
                LOG("socket closed reconnect!");
                ret = reconnect();
                if (ret == 0){
                    ret = sendMsg(commond, jsonDoc);
                }
            }

        }
        return ret;
    }
}