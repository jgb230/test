#ifndef CLIENT_API
#define CLIENT_API
#include "clientMsg.hpp"

#include <string>
#include <rapidjson_helper.h>

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <thread>


#ifdef WIN32
    #define DLL_PUBLIC _declspec(dllexport)
#else
    #define DLL_PUBLIC 
#endif

namespace GL{

    #define MAXLEN 65536
	
#define LEN32 32
	class DLL_PUBLIC callBack {
		public:
			virtual void callback(const char *) = 0;
	};

	typedef void (callBack::*HDL)(const char *);

	class DLL_PUBLIC Client {
	public:
		Client();
		~Client();
		int initClient(const std::string &appId, const std::string &appKey, int type);
		static Client *getInstance();
		int login(const std::string &proId, int *uid);
		int sendMsg(int uid, const std::string &msg);
		int logout(const std::string &proId, int uid);
		int setRecvHandler(callBack *cb, GL::HDL hand);
		static void start();

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
		void*  m_callBack;
		callBack *m_cbInstance;
		static clientMsg *m_clientMsg;
		int m_type;
		std::thread *m_pthread;
	};
    
	
}

#endif