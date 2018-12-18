
#include <stdio.h>  
#include <stdlib.h>

#include <regex.h>

#include "clientAPI.hpp"
#ifdef WIN32
#include <windows.h>
#endif
#include <thread>
#pragma comment(lib,"clientAPI.lib")

using namespace std;

#define LOG(format, ...) do {\
					 _printTime( __func__, __LINE__);\
                     printf(format"\n", ##__VA_ARGS__);\
                    } while (0)

class GL::Client;
class GL::callBack;

void _printTime(const char *func, long int line);

class mycallBack : public GL::callBack {
	public:
		void  callback(const char *msg) {
			rapidjson::Document document;
			document.Parse<0>(msg);
			LOG("=============content %s, uid %d", document["content"].GetString(), document["uid"].GetInt());
	}
};



int main1(int argc,char **argv)
{
    int i,j=0;
    char acErrBuf[256];
    int iRet=-1;
    regex_t tReg;    //定义一个正则实例
    const char *strPattern = "aaa(?<=2000)Windows";//定义模式串
    //profile-level-id=640020;profile-level-id=640021; 相同的,只能匹配前一个
    char *pStrBuf = "aaa2000Windows";   //定义待匹配串
    const size_t dwMatch = 5;    //定义匹配结果最大允许数        //表示允许几个匹配
    regmatch_t atMatch[5];   //定义匹配结果在待匹配串中的下标范围    //存储匹配串位置的数组
    //数组0单元存放主正则表达式位置，后边的单元依次存放子正则表达式位置



    //REG_ICASE 匹配字母时忽略大小写。
    iRet =regcomp(&tReg, strPattern, REG_EXTENDED);    //编译正则模式串
    if(iRet != 0) 
    {
        regerror(iRet, &tReg, acErrBuf, sizeof(acErrBuf));
        printf("Regex Error: %s\n", acErrBuf);
    }
    else
    {
        iRet = regexec(&tReg, pStrBuf, dwMatch, atMatch, 0); //匹配他
        if (iRet == REG_NOMATCH)
        { //如果没匹配上
            printf("No Match\n");
        }
        else if (iRet == REG_NOERROR)
        {  //如果匹配上了
            printf("Match\n");
            for(j=0;j<dwMatch;j++)
            {
                for (i= atMatch[j].rm_so; i < atMatch[j].rm_eo; i++)
                { //遍历输出匹配范围的字符串
                    printf("%c", pStrBuf[i]);
                }
                printf("\n");
            }
        }
        else
        {
            printf("Unknow err:%d",iRet);
        }
        
        regfree(&tReg);  //释放正则表达式
    }
    
    return 0;
}


void login(int i){
    
    std::string proid = "jgbtest" + std::to_string(i);

    std::string msg = "清除肾炎标签1";
    int uid = 0;
    int ret = GL::Client::getInstance()->login(proid, &uid);
    if (ret != 0){
        LOG("login faile");
    }
    
    LOG("====udi : %d proid, %s" ,uid , proid.c_str());
	
    for (int i = 0; i < 15; i++){
        int ret = GL::Client::getInstance()->sendMsg(uid, msg);
        if (0 != ret){
            LOG("send mes error: %s errno : %d\n",strerror(ret),ret);
        }
#ifdef WIN32
		Sleep(1000);
#else
		Sleep(1);
#endif
    }
	GL::Client::getInstance()->logout(proid, uid);

}

int main(){
 
    
	GL::clientInfo ci;
	ci.ip = "172.16.0.27";
	ci.port = 2345;
	ci.appId = "4.00002";
	ci.appKey = "!4j7oTLOXIKOFW@P";
	ci.type = 1;
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
        th[i] = std::thread(login, i);
    }
    
    for (int i = 0; i < 5; ++i){
        th[i].join();
    }

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