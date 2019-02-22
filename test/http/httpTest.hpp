
#include <stdio.h>
#include <string.h>

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

#include <curl.h>
#include "../src/common/cxxurl/SimpleForm.h"
#include "../src/common//crypto/md5.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>


size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream)
{
    //cout << "----->reply" << endl;
    string* str = (string*)stream;
    //cout << *str << endl;
    (*str).append((char*)ptr, size * nmemb);
    return size * nmemb;
}

/*******************************************************
* 函数名:            getTencentAnswer
* 函数说明:        获取腾讯第三方应答
* 参数:
* 返回值:            0 - success, other failed
*******************************************************/
int getTencentAnswer()
{
    string appid = "2112077926";
    string userid = "WDiV59cd47G9qAaV";
    string strUrl = "https://api.ai.qq.com/fcgi-bin/nlp/nlp_textchat";
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
        mapParam.insert(std::make_pair("app_id", appid));
        mapParam.insert(std::make_pair("time_stamp", to_string(time)));
        mapParam.insert(std::make_pair("nonce_str", to_string(time)));
        mapParam.insert(std::make_pair("session", "9999999999"));
        mapParam.insert(std::make_pair("question", "你好"));

        CXXUrl::SimpleForm *sfParam = new CXXUrl::SimpleForm();
        for (auto iter: mapParam){
            sfParam->add(iter.first, iter.second);
        }
        std::string strParam = sfParam->getData();
        strParam = strParam + "&app_key=" + userid;
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

int httpTest(){
    getTencentAnswer();
}