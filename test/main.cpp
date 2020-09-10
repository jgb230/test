
#include <stdio.h>  
#include <stdlib.h>


#include <redis/redisJgb.hpp>
#include <redis/redisTest.hpp>
#include <http/httpTest.hpp>
#include "clientAPI.hpp"
#include "mongoTest.hpp"
#include "http/websocket.hpp"
#ifdef WIN32
#include <windows.h>
#endif
#include <thread>
#include "common.hpp"
#include "md5_jgb.h"
#include "toolUtl.hpp"
#include "myTime.hpp"
#include "mySql.hpp"
#include "redis/slot.hpp"
#pragma comment(lib,"clientAPI.lib")
#include "src/common/md5/my_md5.hpp"
#include "leetCodeEasy.h"

using namespace std;

class GL::Client;
class GL::callBack;


void _printTime(const char *func, long int line) {
#ifdef WIN32
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	printf("%lu %4d-%02d-%02d %02d:%02d:%02d.%ld  %-20s %-6d",
		std::this_thread::get_id(), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, func, line);
#else
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	struct tm tm = *localtime((time_t *)&tv.tv_sec);
	char strTime[30];
	strftime(strTime, 29, "%Y-%m-%d %H:%M:%S", &tm);
	printf("%lu %s.%ld %-20s %-6d ", std::this_thread::get_id(), strTime, tv.tv_usec, func, line);
#endif
}

#define LOG(format, ...) do {\
					 _printTime( __func__, __LINE__);\
                     printf(format"\n", ##__VA_ARGS__);\
                    } while (0)



class mycallBack : public GL::callBack {
	public:
		void  callback(const char *msg) {
			rapidjson::Document document;
			document.Parse<0>(msg);
			LOG("=============content %s, uid %d", document["content"].GetString(), document["uid"].GetInt());
	}
};



void login(std::string appId, int i){
    
    std::string proid = "jgbtest" + std::to_string(i);

    std::string msg = "清除肾炎标签1";
    int uid = 0;
    int ret = GL::Client::getInstance()->login(appId, proid, &uid);
    if (ret != 0){
        LOG("login faile");
    }
    
    LOG("====udi : %d proid, %s" ,uid , proid.c_str());
	
    for (int i = 0; i < 15; i++){
        int ret = GL::Client::getInstance()->sendMsg(appId, uid, msg);
        if (0 != ret){
            LOG("send mes error: %s errno : %d\n",strerror(ret),ret);
        }
		
#ifdef WIN32
		Sleep(1000);
#else
		Sleep(1);
#endif
    }
	GL::Client::getInstance()->logout(appId, proid, uid);

}

int main_app(){
 
	std::string appId = "4.00002";
	std::string appKey = "!4j7oTLOXIKOFW@P";
	int type = 1;

	GL::clientInfo ci;
	ci.ip = "172.16.0.27";
	ci.port = 2345;
	ci.appId_key.push_back(*(new GL::app(appId, appKey, type)));
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

    std::thread th[5];
    for (int i = 0; i < 5; ++i){
        th[i] = std::thread(login, appId, i);
    }
    
    for (int i = 0; i < 5; ++i){
        th[i].join();
    }

	getchar();
    return 0;
}

void main_redis(int num, std::string file, int choice){

	//redis_test();
	//redis_thread(num, file.c_str(), choice);
	redis_base_test();
}

void main_redis_test(){
	redisTest000();
}

void main_http_run(int count, int thread){
	TIMEBEGIN(0);
	httpTest(count, thread);
	TIMEEND(0);
}

int main_http(int argc, char** argv){
	int num = 1;
	int count = 10;
	if(argc > 1){
		LOG("%s",argv[1]);
		num = atoi(argv[1]);
	}
	
	if(argc > 2){
		LOG("%s",argv[2]);
		count = atoi(argv[2]);
	}

	std::thread th[num];
    for (int i = 0; i < num; ++i){
        th[i] = std::thread(main_http_run, (int)count/num, i);
    }
    
    for (int i = 0; i < num; ++i){
        th[i].join();
    }

	getchar();
    return 0;
}

void main_websocket(){
	websocketTest();
}

void main_mongoTest(int num, std::string file, int choice){
	//mongoTest();
	mongo_thread(num, file.c_str(), choice);
}

void mainM(int argc, char** argv){
	//main_http(argc, argv);
	std::string test = "  jgb \t\n";
	LOG("=====%s====",trim(test).c_str());
	int type = 0;
	int num = 2;
	int choice = 0;
	std::string file = "/home/jgb/Downloads/publicMemory";
	if (argc == 5){
		type = atoi(argv[1]);
		LOG(" %d ", type);
		num = atoi(argv[2]);
		LOG(" %d ", num);
		file = argv[3];
		LOG(" %s ", file.c_str());
		choice = atoi(argv[4]);
		LOG(" %d ", choice);
	}
	switch(type){
		case 0:
			main_mongoTest(num, file, choice);
			break;
		case 1:
			main_redis(num, file, choice);
			break;
	}
	
	//
}

void json_test(){
	std::string docstr="{\"type\":\"M\",\"data\":{\"meaning\":\"相同语义输出\",\"paramsss\":{\"省份\":\"e浙江省\",\"城市\":\"e杭州市\",\"区县\":\"e滨江区\",\"街道\":\"e白马湖街道\"}}}";
	json_mk_document(doc);
	json_parse(doc, docstr.c_str());
	auto &data = json_get_object(doc, "data");
    if(json_is_object(data, "params")){
        auto &param = json_get_object(data, "params");
        std::string skey = "";
        std::string svalue = "";
        bool act = true;
        for(auto iter = param.MemberBegin(); iter != param.MemberEnd(); ++iter)
        {
            skey = std::string("s") + iter->name.GetString(); 
            if(!iter->value.IsString()){
                std::cout << iter->name.GetString() << "对应值[" << json_encode(iter->value) <<"]不是字符串类型";
                act = false;
                break;
            }
            
        }
        if(act){
            for(auto iter = param.MemberBegin(); iter != param.MemberEnd(); ++iter)
            {
                skey = std::string("s") + iter->name.GetString(); 
                svalue = iter->value.GetString();
                std::cout << "设置s变元:" << skey << " " << svalue ;
            }
        }
        

    }else{
         std::cout << "param字段不合法:" << json_encode(data) ;
    }
}

void main_md5(){
	std::string test = "你好";
	std::cout << MYMD5(test);
}

void main_time(){
	timeTest();
}

void main_TimerDB(){
	TestTimerDB();
}

void test_Easy(){
	testEasy();
}

int main(int argc, char** argv){
	main_redis_test();
	return 0;
}