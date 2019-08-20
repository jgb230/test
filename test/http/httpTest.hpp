
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <set>
#include <map>    
#include <ctime>
#include <chrono>            
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/noncopyable.hpp>
#include "cJSON.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <curl.h>
#include "../src/common/cxxurl/SimpleForm.h"
#include "../src/common//crypto/md5.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>

#include "prettywriter.h"
#include "rapidjson.h"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "memorystream.h"
#include "common.hpp"
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::Writer;
using namespace rapidjson;


// #define TIMEBEGIN(num)  struct timeval tv##num;\
//                      struct timezone tz##num;\
//                      gettimeofday(&tv##num,&tz##num);\
//                      long int beginTime##num =  tv##num.tv_sec*1000000 + tv##num.tv_usec;

// #define TIMEEND(num) gettimeofday(&tv##num,&tz##num);\
//                      long int endTime##num =  tv##num.tv_sec*1000000 + tv##num.tv_usec;\
//                      printf("微妙，endTime: %ld; 微妙，runTime11:%ld\n", \
//                             endTime##num, endTime##num - beginTime##num);


// #define TESLOG(level,...) printf(__VA_ARGS__)

size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream)
{
    //cout << "----->reply" << endl;
    string* str = (string*)stream;
    //cout << *str << endl;
    (*str).append((char*)ptr, size * nmemb);
    return size * nmemb;
}

/*******************************************************
* 函数名:            getThirdPartyAnswer
* 函数说明:        获取第三方竹间智能应答
* 参数:
* 返回值:            0 - success, other failed
*******************************************************/
int getThirdPartyAnswer()
{

    string appid = "2993848c1503d131228ff41c4ec49cfa";
    string userid = "05126A8690C5DC21C34561750CD97738F";
    string strUrl = "http://idc.emotibot.com/api/ApiKey/openapi.php";
    if (appid.empty() || userid.empty())
    {
        TESLOG(ERROR, "APPID|USERID is Error\n");
        return -1;
    }
    appid = "&appid=" + appid;
    userid = "&userid=00000000000000000000";

    TESLOG(WARNING, "ThirdPartyAnswer Config [appid:%s, userid:%s]\n", appid.c_str(), userid.c_str());

    // get third party answer
    std::string strResponse = "", strValue = "";
    // init curl
    CURL* curl = curl_easy_init();
    // res code
    CURLcode res;
    if (curl)
    {
        // set params
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str()); // url
        std::string param = "cmd=chat" + appid + userid + "&text=" + "你好" + "&location=http://idc.emotibot.com/api/ApiKey/openapi.php";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, param.c_str()); // params
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        // start req
        res = curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        TESLOG(ERROR, "Curl Post Failed, ret[%d]\n", res);
         return -1;
    }
    TESLOG(INFO, "strResponse[%s]\n", strResponse.c_str());

    return 0;
}


/*******************************************************
* 函数名:            getThirdPartyAnswerTuling
* 函数说明:        获取第三方竹图灵能应答
* 参数:
* 返回值:            0 - success, other failed
*******************************************************/
int getThirdPartyAnswerTuling(std::string content)
{
    string strUrl = "http://openapi.tuling123.com/openapi/api/v2";
    bool hasRet = false;
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    Value perception(rapidjson::kObjectType);

    Value inputText(rapidjson::kObjectType);
    inputText.AddMember("text", rapidjson::StringRef(content.c_str()), allocator);
    perception.AddMember("inputText", inputText, allocator);

    document.AddMember("perception", perception, allocator);

    Value userInfo(rapidjson::kObjectType);
    userInfo.AddMember("apiKey", "9f6b23ea51e64949809be4bbe7054647", allocator);
    userInfo.AddMember("userId", "00000000000000000000", allocator);

    document.AddMember("userInfo", userInfo, allocator);


    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    string str = buffer.GetString();
    cout << str << endl;


    TESLOG(WARNING, "getThirdPartyAnswerTuling %s\n", str.c_str());

    // get third party answer
    std::string strResponse = "", strValue = "";
    // init curl
    CURL* curl = curl_easy_init();
    // res code
    CURLcode res;
    if (curl)
    {
        struct curl_slist* http_header = NULL;
        http_header = curl_slist_append(http_header, "Charset: UTF-8");
        http_header = curl_slist_append(http_header, "Content-Type:application/json;charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);//修改协议头

        // set params
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str.c_str()); // params
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        // start req
        res = curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        TESLOG(ERROR, "Curl Post Failed, ret[%d]\n", res);
         return -1;
    }else{
        TESLOG(INFO, "Curl Post success, ret[%d]\n", res);

        cJSON* pJson = cJSON_Parse(strResponse.c_str());
        std::string strAnswer = "";
        std::string strSession = "";
        if (pJson)
        {
            cJSON* pIntent = cJSON_GetObjectItem(pJson, "intent");
            cJSON* pCode = cJSON_GetObjectItem(pIntent, "code");
            TESLOG(INFO, "JpCode:%d\n", pCode->valueint);
            cJSON* pResults = cJSON_GetObjectItem(pJson, "results");
            if (pResults)
            {   
                if (cJSON_Array == pResults->type){
                    for(int i = 0; i < cJSON_GetArraySize(pResults); ++i){
                        cJSON* pArrayValue = cJSON_GetArrayItem(pResults, i);
                        cJSON* pValue = cJSON_GetObjectItem(pArrayValue, "values");
                        cJSON* pText = cJSON_GetObjectItem(pValue, "text");
                        if(pText){
                            strAnswer = pText->valuestring;
                            break;
                        }
                    }
                    
                }
            }
            if (strAnswer.empty()){
                TESLOG(ERROR, "strValue is empty!");
            }else{
                hasRet = true;
            }
        }
        if(hasRet){ 
            std::cout << strAnswer << std::endl;
        }
        
        cJSON_Delete(pJson);
    }

    return 0;
}

/*******************************************************
* 函数名:            getTencentAnswer
* 函数说明:        获取腾讯第三方应答
* 参数:
* 返回值:            0 - success, other failed
*******************************************************/

int getTencentAnswer(std::string content, int i)
{
    string appid[] = {"2115245050", "2112077926"};
    string userid[] = {"c0YzelN03a1b4BxH", "WDiV59cd47G9qAaV"};
    string strUrl = "https://api.ai.qq.com/fcgi-bin/nlp/nlp_textchat";

    TESLOG(ERROR, "账号%d\n", i%2);

    // get third party answer
    std::string strResponse = "", strValue = "";
    // init curl
    CURL* curl = curl_easy_init();
    // res code
    CURLcode res;
    if (curl)
    {
        // set params
        std::map<std::string, std::string> mapParam ;
        time_t time = std::time(0);
        mapParam.insert(std::make_pair("app_id", appid[i%2]));
        mapParam.insert(std::make_pair("time_stamp", to_string(time)));
        mapParam.insert(std::make_pair("nonce_str", to_string(time)));
        mapParam.insert(std::make_pair("session", "9999999999"));
        mapParam.insert(std::make_pair("question", content));

        CXXUrl::SimpleForm *sfParam = new CXXUrl::SimpleForm();
        for (auto iter: mapParam){
            sfParam->add(iter.first, iter.second);
        }
        std::string strParam = sfParam->getData();
        strParam = strParam + "&app_key=" + userid[i%2];
        sfParam->add("sign", boost::algorithm::to_upper_copy(md5(strParam)));
        strParam = sfParam->getData();
        std::cout << "getTencentAnswer param:" << strParam << std::endl;
        strUrl = strUrl + "?" + strParam;
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str() ); // url
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        res = curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        std::cout << "Curl Post Failed, ret:" << res << std::endl; 
         return -1;
    }
    std::cout << "Curl Post success, ret:" << res << " strResponse: " << strResponse << std::endl;

    return 0;
}



/*******************************************************
* 函数名:            getErTong
* 函数说明:        儿童测试
* 参数:
* 返回值:            0 - success, other failed
*******************************************************/
int getErTong()
{
    string strUrl = "http://115.236.55.162:8081/educ-server/ai/aiChatResp.json";
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("uid", 325301, allocator);
    document.AddMember("content", "小朋友，我是巴菲。我是只可爱的兔子，你可以称呼我巴菲兔！上午好", allocator);
    document.AddMember("appid", "5.00301", allocator);
    document.AddMember("action", "{\"type\":\"所属场景\",\"data\":{\"cj\":\"mainscene\"}}", allocator);
    document.AddMember("seq", "27ec05f89da39a3f4cb9591a87b3d9db", allocator);
    document.AddMember("sign", "775a68e6ac20013495bdabc01c196a77", allocator);
    document.AddMember("timestamp", 1555384874, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    string str = buffer.GetString();
    cout << str << endl;

    TESLOG(WARNING, "getErTong %s\n", str.c_str());

    // get third party answer
    std::string strResponse = "", strValue = "";
    // init curl
    CURL* curl = curl_easy_init();
    // res code
    CURLcode res;
    if (curl)
    {
        struct curl_slist* http_header = NULL;
        http_header = curl_slist_append(http_header, "Charset: UTF-8");
        http_header = curl_slist_append(http_header, "Content-Type:application/json;charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);//修改协议头

        // set params
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str.c_str()); // params
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        // start req
        res = curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        TESLOG(ERROR, "Curl Post Failed, ret[%d]\n", res);
         return -1;
    }
    TESLOG(INFO, "strResponse[%s]\n", strResponse.c_str());

    return 0;
}

std::string getContent(){
    std::string  contents[] = {"茄子","黄瓜", "土豆", "我去上学", "我是不是个好人", "你是不是个机器人"};
    TIMEBEGIN(2);
    srand(0);
    int seed = rand()%6;
    return contents[seed];
}

int httpTest(int count, int thread){
    std::string content = "";

    for (int i = 0; i < count; i++){
        TIMEBEGIN(1);
        content = getContent();
        getThirdPartyAnswerTuling(content);
        TIMEEND(1);
    }
}