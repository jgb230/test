#include "clientAPI.hpp"
#include "clientMsg.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <thread>

namespace GL{

    typedef void * (*HDL)(const char *);
    #define LEN32 32

    class Client{
        public:
            Client();
            ~Client();
            int initClient(const std::string &appId, const std::string &appKey, int type);
            static Client *getInstance();
            int login(const std::string &proId, int *uid);
            int sendMsg(int uid, const std::string &msg);
            int logout(const std::string &proId, int uid);
            int setRecvHandler(void * hand(const char *));
            static void start();
            HDL getCallBack(){return m_callBack? m_callBack: nullptr;}
        
        private:
            int msg301(rapidjson::Document &document);
            int msg302(rapidjson::Document &document);
            int msg102(rapidjson::Document &document);
            int msg104(rapidjson::Document &document);
            int msg901(rapidjson::Document &document);

        private:
            static Client *m_client;
            std::string m_appId;
            std::string m_appKey;
            HDL  m_callBack;
            static clientMsg *m_clientMsg;
            int m_type;
			std::thread *m_pthread;
    };

    Client * Client::m_client = new Client();
    clientMsg *Client::m_clientMsg = new clientMsg();

    Client::Client(){
        m_appId = "";
        m_appKey = "";
        m_type = 1;
        m_callBack = nullptr;
		m_pthread = nullptr;
        
    }
    
    Client::~Client(){

    }

    int Client::initClient(const std::string &appId, const std::string &appKey, int type){

        LOG("");

        if (appId.empty() || appKey.empty()){
            LOG("appId or appKey is empty, appId:%s appKey:%s", appId.c_str(), appKey.c_str());
            return -1;
        }

        m_appId = appId;
        m_appKey = appKey;
        m_type = type;

		if (m_pthread == nullptr) {
			m_pthread = new std::thread(Client::start);
			if (m_pthread == nullptr) {
				LOG("create pthread error!\n");
				return -1;
			}
		}

    
        int ret = m_clientMsg->servLogin(m_appId, m_type);
        if (ret != 0){
            LOG("servLogin error! errno:%d", ret);
            return ret;
        }
        ret = m_clientMsg->servAuth(m_appId, m_appKey);
        if (ret != 0){
            LOG("servAuth error! errno:%d", ret);
            return ret;
        }
        return 0;
        //getRobotId(data);

    }

    Client * Client::getInstance(){
        if (m_client == NULL){
            m_client = new Client();
        }
        return m_client;
    }

    int Client::sendMsg(int uid, const std::string &msg){
        return m_clientMsg->aichat(uid, msg, m_appId);
    }

    int Client::login(const std::string &proId, int *uid){
        return m_clientMsg->login(m_appId, proId, uid);
    }

    int Client::logout(const std::string &proId, int uid){
        return m_clientMsg->logout(m_appId, proId, uid);
    }

    int Client::setRecvHandler( HDL hand){
        m_callBack = hand;
        return 0;
    }

    void  Client::start(){
        LOG(" "); 

        char buff[MAXLEN];
        memset(buff, 0, MAXLEN);
        int n = 0;
        uint32_t left = 4;
        uint32_t right = 0;
        uint16_t commond = 0;
        bool head = true;
        for (;;){

            n = recv(m_clientMsg->getSocket(), buff + right, left, 0);
            LOG("recv n:%d", n); 

            if (n < 0){
                LOG("recv error %s errno: %d socket:%d\n",strerror(errno) ,errno, m_clientMsg->getSocket());
#ifdef WIN32
                Sleep(1000);
#else
				Sleep(1);
#endif
            }
            left -= n;
            right += n;
            if (left != 0){
                continue;
            }
            if (0 == left){
                if (head){
                    memcpy(&left, buff, 4);
                    LOG("recv left: %d right: %d", left, right); 
                    memset(buff, 0, MAXLEN);
                    right = 0;
                    head = false;
                }else {
                    memcpy(&commond, buff + 4, 2);
                    LOG("recv commond: %d buff: %s left: %d right: %d", commond, buff + 10, left, right); 
                    rapidjson::Document document;  
                    document.Parse<0>(buff + 10);
                    switch (commond){
                        case 301: Client::getInstance()->msg301(document); break;
                        case 302: Client::getInstance()->msg302(document); break;
                        case 102: Client::getInstance()->msg102(document); break;
                        case 104: Client::getInstance()->msg104(document); break;
                        case 901: Client::getInstance()->msg901(document); break;
                        default: LOG("error commond %d", commond);
                    }
                    
                    left = 4;
                    right = 0;
                    head = true;
                    memset(buff, 0, MAXLEN);
                }
                
            }
        }
    }

    int Client::msg301(rapidjson::Document &document){
        int result = 0;
        if (document.HasMember("result") && document["result"].IsInt()){
            result = document["result"].GetInt();
        }
        std::string randomSeed = "";
        if (result != 1){
            LOG("msg: %s result: %d", document["msg"].GetString(), result); 
        }else {
            if (document.HasMember("randomSeed") && document["randomSeed"].IsString()){
                randomSeed = document["randomSeed"].GetString();
            }
            LOG("randomSeed: %s, result %d", randomSeed.c_str(), result); 
        }
        m_clientMsg->setSeed(randomSeed, result);
        return 0;
    }

    int Client::msg302(rapidjson::Document &document){
        int result = 0;
        if (document.HasMember("result") && document["result"].IsInt()){
            result = document["result"].GetInt();
        }
        m_clientMsg->setAuth(result);
        if (result != 1){
            LOG("msg: %s result: %d", document["msg"].GetString(), result);
        }else {
            LOG("auth result:%d", result);
        }
        return 0;
    }

    int Client::msg102(rapidjson::Document &document){
        int result = 0;
        if (document.HasMember("result") && document["result"].IsInt()){
            result = document["result"].GetInt();
        }
        int uid = 0;
        if (result != 1){
            LOG("msg: %s, result: %d", document["msg"].GetString(), result);
        }else {
            if (document.HasMember("uid") && document["uid"].IsInt()){
                uid = document["uid"].GetInt();
            }
            LOG("login uid: %d , result: %d", uid, result); 
        }
        m_clientMsg->setUid(uid, result);
        return 0;
    }

    int Client::msg104(rapidjson::Document &document){
        int result = 0;
        if (document.HasMember("result") && document["result"].IsInt()){
            result = document["result"].GetInt();
        }
        int uid = 0;
        std::string seq = "";
        if (result != 1){
            LOG("msg: %s, result %d", document["msg"].GetString(), result); 
        }else {
            if (document.HasMember("uid") && document["uid"].IsInt()){
                uid = document["uid"].GetInt();
            }
            if (document.HasMember("seq") && document["seq"].IsString()){
                seq = document["seq"].GetString();
            }
            LOG("logout uid: %d, seq: %s", uid, seq.c_str()); 
        }
        return 0;
    }

    int Client::msg901(rapidjson::Document &document){
        int result = 0;
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        std::string strJson = buffer.GetString();

        if (document.HasMember("result") && document["result"].IsInt()){
            result = document["result"].GetInt();
        }
        if (result != 1){
            LOG("msg: %s, result %d", document["msg"].GetString(), result); 
            return -1;
        }else {
            (Client::getInstance()->getCallBack())(strJson.c_str());
            LOG("aichat result: %d", result);
        }
        return 0;
    }


    int initClient(const std::string &appId, const std::string &appKey ,int type){
        return Client::getInstance()->initClient(appId, appKey, type);
    }

    int sendMsg(int uid, const std::string &msg){
        LOG("uid %d, msg %s", uid, msg.c_str());
        return Client::getInstance()->sendMsg(uid, msg);
    }

    int setRecvHandler( HDL hand){
        return Client::getInstance()->setRecvHandler(hand);
    }

    int login(const std::string &proId, int *uid){

        return Client::getInstance()->login(proId, uid);
    }

    int logout(const std::string &proId, int uid){
        return Client::getInstance()->logout(proId, uid);
    }

    void *callback(const char *msg){
        
        rapidjson::Document document;  
        document.Parse<0>(msg);
        LOG("content %s, uid %d", document["content"].GetString(), document["uid"].GetInt());

        return nullptr;
    }


}