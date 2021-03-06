#include "clientMsg.hpp"
#include "md5_jgb.h"

#include<ctype.h>
namespace GL{


    clientMsg::clientMsg(){
        rwlock = new WfirstRWLock();

		m_slock = new std::mutex();
		m_scond = new std::condition_variable();

#ifdef __linux__

        signal(SIGPIPE,SIG_IGN);

#endif
        m_result = 0;
        m_randomSeed = "";
        m_heartTime = 0;
        m_uid = 0;
    }

	int clientMsg::init(clientInfo *ci) {
		m_ci = ci;
		int nTimeout = 10000;
#ifdef WIN32
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
		struct sockaddr_in sockaddr;
		m_socket = socket(AF_INET, SOCK_STREAM, 0);
		memset(&sockaddr, 0, sizeof(sockaddr));
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(m_ci->port);
#ifdef WIN32
		sockaddr.sin_addr.s_addr = inet_addr(m_ci->ip.c_str());
#else
		inet_pton(AF_INET, m_ci->ip.c_str(), &sockaddr.sin_addr);
#endif
		int ret = 0;
		if (ret = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO,
			(char *)&nTimeout, sizeof(int)) != 0)
		{
			LOG("setsockopt error %s errno: %d ret %d", strerror(errno), errno, ret);
		}

		if ((ret = connect(m_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) < 0)
		{
			LOG("connect error %s errno: %d ret %d", strerror(errno), errno, ret);
		}
		LOG("connect ");
		return ret;
	}

    int clientMsg::reconnect(){
        struct sockaddr_in sockaddr;
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        memset(&sockaddr, 0, sizeof(sockaddr));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(m_ci->port);
#ifdef WIN32
		sockaddr.sin_addr.S_un.S_addr = inet_addr(m_ci->ip.c_str());
#else
		inet_pton(AF_INET, m_ci->ip.c_str(), &sockaddr.sin_addr);
#endif
		int time = 0;
        int ret = 0;
        while((ret = connect(m_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) < 0)
        {
            ++time;
#ifdef WIN32
			Sleep(1000);
#else
			Sleep(1);
#endif
            if (time >= 3){
                LOG("reconnect time: %d ret: %d error %s errno: %d", time, ret, strerror(errno),errno);
                return -1;
            }
        }
		std::string appId = "";
		std::string appKey = "";
		int type = 0;
		for (auto iter = m_ci->appId_key.begin(); iter != m_ci->appId_key.end(); iter++) {
			appId = iter->appId;
			appKey = iter->appKey;
			type = iter->type;
			ret = servLogin(appId, type);
			if (ret != 0) {
				LOG("servLogin error! errno:%d", ret);
				return ret;
			}
			ret = servAuth(appId, appKey);
			if (ret != 0) {
				LOG("servAuth error! errno:%d", ret);
				return ret;
			}
		}

        LOG(" reconnect !");
        return 0;
    }

    clientMsg::~clientMsg(){
		closesocket(m_socket);
#ifdef WIN32
		WSACleanup();
#endif
        LOG(" ");
    }

    int clientMsg::heartBeat(){
		time_t nowTime = time(NULL);
		if (nowTime - m_heartTime < 30) {
			return 0;
		}
		m_heartTime = nowTime;
        rapidjson::Document jsonDoc;
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
        jsonDoc.SetObject();
		jsonDoc.AddMember("nowTime", 0, allocator);

        return sendMsg(9999, jsonDoc);
    }

    int clientMsg::servLogin(const std::string &appId, int type){
        
        rapidjson::Document jsonDoc;
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
        jsonDoc.SetObject();
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

		std::unique_lock <std::mutex> lck(*m_slock);
		m_scond->wait(lck);
		ret = m_result;


        return ret == 1? 0: -1;
    }   

    int clientMsg::servAuth(const std::string &appId,const std::string &appKey){
        std::string data = m_randomSeed + appKey;

		std::string sign = md5(data);
        rapidjson::Document jsonDoc;
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
        jsonDoc.SetObject();
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

        return ret == 1? 0: -1;
    }   


    int clientMsg::aichat(int uid,const std::string &msg,const std::string &appId){
        
        rapidjson::Document jsonDoc;
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
        jsonDoc.SetObject();
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

        rapidjson::Document jsonDoc;
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
        jsonDoc.SetObject(); 
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

        int tmpid = 0;

		do{
            rwlock->lock_read();
            tmpid = recvedUid(proId);
            ret = m_result;
            rwlock->release_read();
        }while ( tmpid == 0 );
		
		*uid = tmpid;
        

        return ret == 1? 0:ret ;
    }

    int clientMsg::recvedUid(const std::string &proId){
        
        for (auto iter: m_loginId){
            if(strcmp(iter.second.c_str(), proId.c_str()) == 0){
                return iter.first;
            }
        }
        return 0;
    }

    int clientMsg::logout(const std::string &appId,const std::string &proId, int uid){

        rapidjson::Document jsonDoc;
        rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
        jsonDoc.SetObject();
        jsonDoc.AddMember("uid", uid, allocator);
        jsonDoc.AddMember("appid", appId, allocator);
        jsonDoc.AddMember("seq", proId, allocator);
		m_loginId.erase(uid);
        int ret = sendMsg(4, jsonDoc);
        if (ret < 0){
            LOG("send mes error: %s errno : %d",strerror(ret),ret);
            return ret;
        }else if( ret == 0){
            LOG("send socket close ");
            return -1;
        }
        unique_readguard<WfirstRWLock> lc(*rwlock);
		std::map<int,std::string>::iterator iter = m_loginId.find(uid);
		if (iter != m_loginId.end()) {
			m_loginId.erase(iter);
		}
        return 0;
    }

    void clientMsg::setUid(int uid, int result, const std::string &proId){

        unique_writeguard<WfirstRWLock> lc(*rwlock);
		m_result = result;
        m_loginId.insert(std::make_pair(uid, proId));

    }

    void clientMsg::setSeed(const std::string seed, int result){
		std::unique_lock <std::mutex> lck(*m_slock);
		m_randomSeed = seed;
		m_result = result;
		m_scond->notify_all();
    }

    void clientMsg::setAuth(int result){
		std::unique_lock <std::mutex> lck(*m_slock);
		m_result = result;
		m_scond->notify_all();

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
        head->Version = m_ci->version;
        head->Magic = m_ci->magic;
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
                LOG("send mes error: %s errno : %d ret: %d", strerror(errno), errno, ret);
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