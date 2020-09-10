/*
 * CAsrTtsAliyun.cpp
 *
 *  Created on: 2020年5月11日
 *      Author: tianhuajian
 */
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/time.h>
#include <curl.h>
#include "rapidjson/rapidjson_helper.h"
#include "rapidjson/rapidjson.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
 #include "CServiceComm.h"
#include <mtsframe/logging/mtsLog.h>
#include <openssl/hmac.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "CAsrTtsAliyun.hpp"
#include "../tengxunyun/encrypt.h"
using namespace std;
using namespace glxeye_mts;

CAsrTtsAliyun::CAsrTtsAliyun()
{
}

CAsrTtsAliyun::~CAsrTtsAliyun()
{

}
size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream)
{
    //cout << "----->reply" << endl;
    std::string* str = (std::string*)stream;
    //cout << *str << endl;
    (*str).append((char*)ptr, size * nmemb);
    return size * nmemb;
}

size_t responseHeadersCallback(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    std::map<std::string, std::string> *headers = (std::map<std::string, std::string>*)userdata;
    std::string line((char*)ptr);
    std::string::size_type pos = line.find(':');
    if (pos != line.npos)
    {
        std::string name = line.substr(0, pos);
        std::string value = line.substr(pos + 2);
        size_t p = 0;
        if ((p = value.rfind('\r')) != value.npos) {
            value = value.substr(0, p);
        }
        headers->insert(make_pair(name, value));
    }
    return size * nmemb;
}

string canonicalizedQuery(std::map<std::string, std::string> &param){
    if(param.size() == 0){
        return "";
    }
    stringstream buffer;
    for(auto iter = param.begin(); iter != param.end(); iter++){
		buffer << "&" << urlEncode(iter->first) << "=" << urlEncode(iter->second);
	}
    return buffer.str().substr(1);
}

string CAsrTtsAliyun::createStringToSign(string &method, string &urlPath, string &queryString){
    stringstream buffer;
    buffer << method << "&" << urlEncode(urlPath) << "&" << urlEncode(queryString);
    return buffer.str();
}

string CAsrTtsAliyun::sign(string &stringToSign, string &accessKeySecret){
    unsigned char out[EVP_MAX_MD_SIZE] = {'\0'};
    char *k = (char *)malloc(accessKeySecret.length());
    char *d = (char *)malloc(stringToSign.length());
    memset(k, 0, accessKeySecret.length());
    memset(d, 0, stringToSign.length());
    memcpy(d, stringToSign.c_str(), stringToSign.length());
    memcpy(k, accessKeySecret.c_str(), accessKeySecret.length());
    unsigned int t = 0;
    HMAC(EVP_sha1(), k, strlen(k), (unsigned char*)d, strlen(d),  out, &t);
    string base64 = urlEncode(base64_encode(out, t));

    char mdString[41] = {'\0'};
    for(int i = 0; i < 20; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)out[i]);
    MTSLOG(enSvcId_ai_callcenter,INFO) << "stringToSign:" << stringToSign << endl;
    MTSLOG(enSvcId_ai_callcenter,INFO) << "stringToSign:" << d << endl;
    MTSLOG(enSvcId_ai_callcenter,INFO) << "accessKeySecret:" << accessKeySecret << endl;
    MTSLOG(enSvcId_ai_callcenter,INFO) << "accessKeySecret:" << k << endl;
    MTSLOG(enSvcId_ai_callcenter,INFO) << "sha1:" << mdString << endl;
    MTSLOG(enSvcId_ai_callcenter,INFO) << "base64:" << base64 << endl;
    return base64;
}

bool CAsrTtsAliyun::asrRequest(const asr_parameter *asr_param,const char* audioFile,char*result) {
    
	std::string l_str = "",l_key_id,l_key_secret, l_token;
	l_key_id=asr_param->key_id;
	l_key_secret=asr_param->key_secret;
    int retry = 0;
	while(l_token.empty()){
        if(retry == 3){
            return -1;
        }
        generateToken(l_key_id, l_key_secret, l_token, asr_param->reserve3);
        retry++;
    }

	CURL* curl = NULL;
    CURLcode res;

    /**
    * 读取音频文件
    */
    ifstream fs;
    fs.open(audioFile, ios::out | ios::binary);
    if (!fs.is_open()) {
        MTSLOG(enSvcId_ai_callcenter,INFO) << "The audio file is not exist!" << endl; 
        return -1;
    }
    stringstream buffer;
    buffer << fs.rdbuf();
    string audioData(buffer.str());

    curl = curl_easy_init();
    if (curl == NULL) {
        return -1;
    }

    string url = asr_param->reserve2;
    string format = "wav";
    int sampleRate = atoi(asr_param->sample_rate);
    bool enablePunctuationPrediction = true;
    bool enableInverseTextNormalization = true;
    bool enableVoiceDetection = false;
    /**
    * 设置RESTful请求参数
    */
    ostringstream oss;
    oss << url;
    oss << "?appkey=" << asr_param->app_id;
    oss << "&format=" << format;
    oss << "&sample_rate=" << sampleRate;
    if (enablePunctuationPrediction) {
        oss << "&enable_punctuation_prediction=" << "true";
    }
    if (enableInverseTextNormalization) {
        oss << "&enable_inverse_text_normalization=" << "true";
    }
    if (enableVoiceDetection) {
        oss << "&enable_voice_detection=" << "true";
    }

    string request = oss.str();
    /**
    * 设置HTTP请求行
    */
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, request);

    /**
    * 设置HTTP请求头部
    */
    struct curl_slist* headers = NULL;
    // token
    string X_NLS_Token = "X-NLS-Token:";
    X_NLS_Token += l_token;
    headers = curl_slist_append(headers, X_NLS_Token.c_str());
    // Content-Type
    headers = curl_slist_append(headers, "Content-Type:application/octet-stream");
    // Content-Length
    string content_Length = "Content-Length:";
    ostringstream ossh;
    ossh << content_Length << audioData.length();
    content_Length = ossh.str();
    headers = curl_slist_append(headers, content_Length.c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /**
    * 设置HTTP请求数据
    */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, audioData.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, audioData.length());

    /**
    * 设置HTTP请求的响应回调函数
    */
   	string srResult = "";
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, srResult);

    /**
    * 发送HTTP请求
    */
    res = curl_easy_perform(curl);

    // 释放资源
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
	cout << "srResult" << srResult<< endl;
    if (res != CURLE_OK) {
        MTSLOG(enSvcId_ai_callcenter,INFO) << "curl_easy_perform failed: " << curl_easy_strerror(res) << endl;
        return -1;
    }

    return 0;
}

int CAsrTtsAliyun::text_to_speech(const char* src_text, const char* des_path,
		const tts_parameter *tts_param)
{

	int ret = -1;
	if (NULL == src_text || NULL == des_path)
	{
		MTSLOG(enSvcId_ai_callcenter,INFO)
				<< "params is error!" << std::endl;
		return ret;
	}

	std::string l_str = "",l_key_id,l_key_secret, l_token;
	l_key_id=tts_param->key_id;
	l_key_secret=tts_param->key_secret;
    int retry = 0;
    while(l_token.empty()){
        if(retry == 3){
            return ret;
        }
        generateToken(l_key_id, l_key_secret, l_token, tts_param->reserve3);
        retry++;
    }

    std::string retStr = "";

	json_mk_document(doc);
	json_addstring_member(doc, "appkey", tts_param->reserve2);
	json_addstring_member(doc, "token", l_token);
	json_addstring_member(doc, "text", src_text);
	json_addstring_member(doc, "format", "wav");
	json_add_member(doc, "sample_rate", atoi(tts_param->sample_rate));
	json_addstring_member(doc, "voice", tts_param->voice_name);
	json_add_member(doc, "volume", atoi(tts_param->volume));
	json_add_member(doc, "speech_rate", atoi(tts_param->speed));
	json_add_member(doc, "pitch_rate", atoi(tts_param->pitch));
	std::string strParam = json_encode(doc);
	std::string strResponse;
    // init curl
    CURL* curl = curl_easy_init();
	if (curl == NULL) {
        return -1;
    }
    // res code
    CURLcode res = CURLE_OK;
	std::string strUrl = tts_param->reserve1;
	MTSLOG(enSvcId_ai_callcenter,INFO)  << "url:" << strUrl << std::endl<< " param:" << strParam << std::endl;
    
	struct curl_slist* http_header = NULL;
	http_header = curl_slist_append(http_header, "Content-Type:application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);//修改协议头
	// set params
	curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str()); // url
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strParam.c_str()); // params
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
	std::map<std::string, std::string> responseHeaders;
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, responseHeadersCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
	// start req
	
	res = curl_easy_perform(curl);
	curl_slist_free_all(http_header);
    curl_easy_cleanup(curl);
    // release curl
   	for(auto iter = responseHeaders.begin(); iter != responseHeaders.end(); iter++){
		MTSLOG(enSvcId_ai_callcenter,INFO) << "iter:" << iter->first << ":" << iter->second << std::endl;
	}
    if (res != CURLE_OK)
    {
        MTSLOG(enSvcId_ai_callcenter,INFO) << "Curl Post Failed, ret["<< res<<"]"<< std::endl;
		return -1;
    }
	std::map<std::string, std::string>::iterator it = responseHeaders.find("Content-Type");
    if (it != responseHeaders.end() && it->second.compare("audio/mpeg") == 0) {
        std::ofstream fs;
        fs.open(des_path, std::ios::out | std::ios::binary);
        if (!fs.is_open()) {
            MTSLOG(enSvcId_ai_callcenter,INFO)  << "The audio save file can not open!";
            return -1;
        }
        fs.write(strResponse.c_str(), strResponse.size());
        fs.close();
        MTSLOG(enSvcId_ai_callcenter,INFO)  << "The POST  request succeed!" << std::endl;
    }
    else {
        MTSLOG(enSvcId_ai_callcenter,INFO)  << "The POST  request failed: " + strResponse << std::endl;
        return -1;
    }
	return 0;
}

bool CAsrTtsAliyun::ttsRequest(const tts_parameter *tts_param,const char* contents,const char* audioFile)
{

	MTSLOG(enSvcId_ai_callcenter,INFO)  << "tts_param:" << tts_param->to_string()<< std::endl;
	text_to_speech(contents, audioFile, tts_param);
	return true;
}
bool CAsrTtsAliyun::generateToken( std::string akId, std::string akSecret, 
                                    std::string&token ,std::string url)
{
    auto iter = m_token_map.find(akId);
    if(iter != m_token_map.end() && m_token_map[akId].m_expireTime - std::time(0) > 10){
        token = iter->second.m_token;
        return true;
    }
    map<string, string> param;
    param.insert(make_pair("AccessKeyId",akId));
    param.insert(make_pair("Action","CreateToken"));
    param.insert(make_pair("Version","2019-02-28"));
    param.insert(make_pair("Timestamp",getISO8601Time()));
    param.insert(make_pair("Format","JSON"));
    param.insert(make_pair("RegionId","cn-shanghai"));
    param.insert(make_pair("SignatureMethod","HMAC-SHA1"));
    param.insert(make_pair("SignatureVersion","1.0"));
    param.insert(make_pair("SignatureNonce",getUniqueNonce()));
    string paramStr = canonicalizedQuery(param);

    string method = "GET";
    string urlPath = "/";
    string signParam = createStringToSign(method, urlPath, paramStr);
    string secret = akSecret + "&";
    string signNature = sign(signParam, secret);
    string querySign = "Signature=" + signNature + "&" + paramStr;
    ostringstream oss;
    oss << url;
    oss << "/?" << querySign;
    string tokenurl = oss.str();
    std::cout << "tokenurl:" << tokenurl << std::endl;

    CURL*  curl = curl_easy_init();
    if (curl == NULL) {
        return -1;
    }
    string strResponse = "";
    // set params
    curl_easy_setopt(curl, CURLOPT_URL, tokenurl.c_str()); // url
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResponse);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 2 * 1000);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2 * 1000);
    // start req
    int res = curl_easy_perform(curl);
    long exp = 0l; 
    if (res != CURLE_OK) {
        MTSLOG(enSvcId_ai_callcenter, ERROR)<<"Curl Post Failed, ret = "<<res;
    }else{
        MTSLOG(enSvcId_ai_callcenter, ERROR)<<"Curl Post success, ret = "<<res << ",strResponse=" << strResponse;
		json_mk_document(doc);
        if (json_parse(doc, strResponse.c_str())) {
            if(json_is_object(doc, "Token")){
                auto &tokenObj = json_get_object(doc, "Token");
                if (!tokenObj.HasMember("Id") || !tokenObj.HasMember("ExpireTime")){
                    MTSLOG(enSvcId_ai_callcenter, ERROR)<<"Id/ExpireTime  is empty!";
                }else{
                    token = json_get_string(tokenObj, "Id");
                    exp = json_get_int64(tokenObj, "ExpireTime");
                }
                
            }else{
                MTSLOG(enSvcId_ai_callcenter, ERROR)<<"token is empty!";
            }
            
        } else {
            MTSLOG(enSvcId_ai_callcenter, ERROR)<<"Json Parse Failed";
        }

    }
    // 释放资源
    curl_easy_cleanup(curl);
    token_info l_token_info;
    m_token_map[akId] = l_token_info;
	m_token_map[akId].m_expireTime = exp;
	m_token_map[akId].m_token = token;

	return true;
}

string CAsrTtsAliyun::getISO8601Time(time_t tm){
    std::time_t course_start_time1 = tm;
    if(course_start_time1 == 0){
        course_start_time1 = time(0);
    }
    char buf[32];
    strftime(buf, sizeof(buf), "%FT%TZ", gmtime(&course_start_time1));
    std::cout << "time:" << buf << std::endl;
    return buf;
}
string CAsrTtsAliyun::getUniqueNonce() {
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); 
    string uuid_string = boost::uuids::to_string(a_uuid);
    return uuid_string;
}

extern "C" CAsrTtsBase *createService()
{
	return new CAsrTtsAliyun();
}
