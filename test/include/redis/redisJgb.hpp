extern "C"{
#include <hiredis-vip/hiredis.h>
#include <hiredis-vip/sds.h>
#include <hiredis-vip/hircluster.h>
}
#include <vector>
#include <map>
#include <set>
#include <boost/format.hpp>
#include <time.h>
#include <sys/time.h>
#include <memory>
#include <thread>

#include <iostream>
#include <sstream>

#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/classification.hpp>

using namespace std;

#define TESLOG(level,...) printf(__VA_ARGS__)

#define TIMEBEGIN(num)  struct timeval tv##num;\
                     struct timezone tz##num;\
                     gettimeofday(&tv##num,&tz##num);\
                     long int beginTime##num =  tv##num.tv_sec*1000000 + tv##num.tv_usec;\
                     printf("微妙，beginTime: %ld\n",beginTime##num);

#define TIMEEND(num) gettimeofday(&tv##num,&tz##num);\
                     long int endTime##num =  tv##num.tv_sec*1000000 + tv##num.tv_usec;\
                     printf("微妙，endTime: %ld; 微妙，runTime:%ld\n",\
                            endTime##num, endTime##num - beginTime##num);

typedef vector<string> vecString;

// 例: v = {"1","2","3"} 
		//    执行catsVector(v, "'", "_", out)
		//	  结果out = "'1'_'2'_'3'"
bool catsVector(vecString &sVec , const string &edge, const string &Connector, string &result)
{
    result = edge;
    for (size_t j = 0; j < sVec.size(); ++j)
    {
        result += sVec[j];
        if (j < sVec.size() - 1){
            result += edge + Connector + edge;
        }
    }
    result += edge;
    if (result.empty()){
        return false;
    }
    return true;
}

char** convertToSds(const std::vector<std::string>& vecCammond)
{
    size_t nJ;
    char **pSds = (char**)malloc(sizeof(char*)*vecCammond.size());
    for(nJ = 0; nJ < vecCammond.size(); nJ++)
    {
        pSds[nJ] = sdsnew((vecCammond[nJ]).c_str());
    }
    return pSds;
}

bool incr(const char *pcKey, int &nOut, redisClusterContext* _context)
{
	nOut = 0;
	if(!pcKey) return false;
    
    char cmd[24 + strlen(pcKey)] = { 0 };
    sprintf(cmd, "INCR %s", pcKey);


    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected\n", cmd);
        return false;
    }
    redisReply* reply =  (redisReply*)redisClusterCommand(_context, cmd);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply\n", cmd);
        return false;
    }

    switch(reply->type)
    {
    case REDIS_REPLY_INTEGER:
        TESLOG(INFO, "Execute cmd[%s] -- Reply[%llu]\n", cmd, reply->integer);
        break;
    case REDIS_REPLY_STRING:
        TESLOG(INFO, "Execute cmd[%s] -- Reply[%s]\n", cmd, reply->str);
        break;
    case REDIS_REPLY_STATUS:
        TESLOG(INFO, "Execute cmd[%s] -- Reply[%s]\n", cmd, reply->str);
        break;    
    case REDIS_REPLY_NIL:
        TESLOG(INFO, "Execute cmd[%s] -- Reply Nil\n", cmd);
        nOut = 0;
        break;
    default:
        break;
    }

	nOut = reply->integer;
    freeReplyObject(reply);

    return nOut == 0 ? false : true;

}

bool sadd(const char* key, const char* value, redisClusterContext* _context)
{
    if(!key) return false;

    char cmd[24 + strlen(key) + strlen(value)] = { 0 };
    sprintf(cmd, "SADD %s %s", key, value);


    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected\n", cmd);
        return false;
    }

    redisReply* reply =  (redisReply*)redisClusterCommand(_context, cmd);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply\n", cmd);
        return false;
    }

    freeReplyObject(reply);

    return true;
}

bool zadd(const char* key,const int score, const char* value, redisClusterContext* _context)
{
    if(!key) return false;

    char cmd[24 + strlen(key) + strlen(value)] = { 0 };
    sprintf(cmd, "ZADD %s %d %s", key, score, value);

    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected\n", cmd);
        return false;
    }
    redisReply* reply =  (redisReply*)redisClusterCommand(_context, "ZADD %s %d %s", key, score, value);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply\n", cmd);
        return false;
    }
    freeReplyObject(reply);

    return true;
}

bool del(const char* key, redisClusterContext* _context)
{
    if(!key) return false;

    char cmd[24 + strlen(key)] = { 0 };
    sprintf(cmd, "del %s", key);
    TESLOG(ERROR, "Execute cmd[%s]\n", cmd);
    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected\n", cmd);
        return false;
    }
    redisReply* reply =  (redisReply*)redisClusterCommand(_context, "del %s", key);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply\n", cmd);
        return false;
    }
    freeReplyObject(reply);

    return true;
}

bool Set(const char* key, const char* value, redisClusterContext* _context)
{
    if(!key) return false;

    char cmd[24 + strlen(key) + strlen(value)] = { 0 };
    sprintf(cmd, "set %s %s", key, value);
    TESLOG(ERROR, "Execute cmd[%s]\n", cmd);
    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected\n", cmd);
        return false;
    }
    redisReply* reply =  (redisReply*)redisClusterCommand(_context, "set %s %s", key, value);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply\n", cmd);
        return false;
    }
    freeReplyObject(reply);

    return true;
}

bool get(const char* key, std::string &Out, redisClusterContext* _context)
{
    if(!key) return false;

    char cmd[24 + strlen(key)] = { 0 };
    sprintf(cmd, "get %s ", key);

    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected\n", cmd);
        return false;
    }
    redisReply* reply =  (redisReply*)redisClusterCommand(_context, "get %s", key);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply\n", cmd);
        return false;
    }
    switch(reply->type)
    {
        case REDIS_REPLY_INTEGER:
            TESLOG(INFO, "Execute cmd[%s] -- Reply[%llu]\n", cmd, reply->integer);
            break;
        case REDIS_REPLY_STRING:
            TESLOG(INFO, "Execute cmd[%s] -- Reply[%s]\n", cmd, reply->str);
            break;
        case REDIS_REPLY_STATUS:
            TESLOG(INFO, "Execute cmd[%s] -- Reply[%s]\n", cmd, reply->str);
            break;    
        case REDIS_REPLY_NIL:
            TESLOG(INFO, "Execute cmd[%s] -- Reply Nil\n", cmd);
            break;
        default:
            break;
    }

	Out = reply->str;
    freeReplyObject(reply);
    return true;
}

bool hgetall(const char *pcKey, std::map<std::string, std::string> &mapOut, redisClusterContext* _context){
    if(!pcKey) return false;

    char cmd[24 + strlen(pcKey)] = { 0 };
    sprintf(cmd, "HGETALL %s", pcKey);

    if(!_context) 
    {
        return false;
    }
    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected", cmd);
        return false;
    }
    redisReply* reply =  (redisReply*)redisClusterCommand(_context, "HGETALL %s", pcKey);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply", cmd);
        return false;
    }

    switch(reply->type)
    {
        case REDIS_REPLY_INTEGER:
            TESLOG(INFO, "Execute cmd[%s] -- Reply[%llu]", cmd, reply->integer);
            break;
        case REDIS_REPLY_STRING:
            TESLOG(INFO, "Execute cmd[%s] -- Reply[%s]", cmd, reply->str);
            break;
        case REDIS_REPLY_ARRAY:
            TESLOG(INFO, "Execute cmd[%s] -- Elements[%lu]", cmd, reply->elements);
            break;
        case REDIS_REPLY_STATUS:
            TESLOG(INFO, "Execute cmd[%s] -- Reply[%s]", cmd, reply->str);
            break;
        case REDIS_REPLY_NIL:
            TESLOG(INFO, "Execute cmd[%s] -- Reply Nil", cmd);
            break;
        default:
            break;
    }
    if (mapOut.size() != 0){
        mapOut.clear();
    }
    for (size_t i = 0; i < reply->elements; i+=2)
    {
        mapOut.insert(std::make_pair<std::string, std::string>(reply->element[i]->str, reply->element[i+1]->str));
    }

    freeReplyObject(reply);
    return true;
}

void test(redisClusterContext* _context){
    TIMEBEGIN(1);
    //redis.call('del', KEYS[1], KEYS[2])
    string strLuaScript = "redis.call('sadd', KEYS[1], 'test1') redis.call('sadd', KEYS[2], 'test1') redis.call('del', KEYS[1], KEYS[2])";
    int nI = 2;
    
    std::vector<std::string> vecKey;
    vecKey.push_back("{jgbtest}");
    vecKey.push_back("{jgbtest}1");

    TESLOG(INFO, "test, strLuaScript = %s\n", strLuaScript.c_str());
    for (auto &iter: vecKey){
        TESLOG(INFO, "%s\n", (iter).c_str());
    }

    const sds sdsLuaScript = sdsnew(strLuaScript.c_str());
    std::vector<std::string> vecCammond;
    vecCammond.push_back("EVAL");
    vecCammond.push_back(sdsLuaScript);
    sdsfree(sdsLuaScript);
    vecCammond.push_back(std::to_string(nI));
    vecCammond.insert(vecCammond.end(), vecKey.begin(), vecKey.end());


    char **pcArgv = convertToSds(vecCammond);
    size_t stArgc = vecCammond.size();
    std::unique_ptr<size_t[]> pArgvlen(new size_t[stArgc]);
    for (size_t nJ = 0; nJ < stArgc; nJ++)
    {
        pArgvlen[nJ] = sdslen(pcArgv[nJ]);
    }
    redisReply *pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
        sdsfreesplitres(pcArgv, stArgc);
        TIMEEND(1);
        return;
    }
    switch(pReply->type)
    {
        case REDIS_REPLY_INTEGER:
            TESLOG(INFO, "Reply[%llu]\n", pReply->integer);
            break;
        case REDIS_REPLY_STRING:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_ARRAY:
            TESLOG(INFO, "Elements[%lu]\n", pReply->elements);
            break;
        case REDIS_REPLY_STATUS:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_NIL:
            TESLOG(INFO, "Reply Nil\n");
            break;
        default:
            break;
    }

    sdsfreesplitres(pcArgv, stArgc);	
    freeReplyObject(pReply);
    TIMEEND(1);
}

void test1(redisClusterContext* _context){
    TIMEBEGIN(1);
    
    std::string cmd = "sadd {1jgbtest} test1";
    char ccmd[24 + cmd.length()] = { 0 };
    sprintf(ccmd, "%s", cmd.c_str());
    TESLOG(INFO, "test1, ccmd = %s\n", ccmd);

    redisReply* pReply =  (redisReply*)redisClusterCommand(_context, ccmd);

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
        TIMEEND(1);
        return;
    }
    switch(pReply->type)
    {
        case REDIS_REPLY_INTEGER:
            TESLOG(INFO, "Reply[%llu]\n", pReply->integer);
            break;
        case REDIS_REPLY_STRING:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_ARRAY:
            TESLOG(INFO, "Elements[%lu]\n", pReply->elements);
            break;
        case REDIS_REPLY_STATUS:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_NIL:
            TESLOG(INFO, "Reply Nil\n");
            break;
        default:
            break;
    }

    cmd = "sadd {1jgbtest}1 test1";
    memset(ccmd, 0, sizeof(ccmd));
    sprintf(ccmd, "%s", cmd.c_str());
    TESLOG(INFO, "test1, ccmd = %s\n", ccmd);

    pReply =  (redisReply*)redisClusterCommand(_context, ccmd);

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
        TIMEEND(1);
        return;
    }
    switch(pReply->type)
    {
        case REDIS_REPLY_INTEGER:
            TESLOG(INFO, "Reply[%llu]\n", pReply->integer);
            break;
        case REDIS_REPLY_STRING:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_ARRAY:
            TESLOG(INFO, "Elements[%lu]\n", pReply->elements);
            break;
        case REDIS_REPLY_STATUS:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_NIL:
            TESLOG(INFO, "Reply Nil\n");
            break;
        default:
            break;
    }
 
    cmd = "del {1jgbtest} {1jgbtest}1";
    memset(ccmd, 0, sizeof(ccmd));
    sprintf(ccmd, "%s", cmd.c_str());
    TESLOG(INFO, "test1, ccmd = %s\n", ccmd);

    pReply =  (redisReply*)redisClusterCommand(_context, ccmd);

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
        TIMEEND(1);
        return;
    }
    switch(pReply->type)
    {
        case REDIS_REPLY_INTEGER:
            TESLOG(INFO, "Reply[%llu]\n", pReply->integer);
            break;
        case REDIS_REPLY_STRING:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_ARRAY:
            TESLOG(INFO, "Elements[%lu]\n", pReply->elements);
            break;
        case REDIS_REPLY_STATUS:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_NIL:
            TESLOG(INFO, "Reply Nil\n");
            break;
        default:
            break;
    }

    
    freeReplyObject(pReply);
    TIMEEND(1);
}

void findIntersection(const std::string &strRobot, const std::map<std::string, std::string> &mapMemory,
                         std::set<std::string> &setTemp, redisClusterContext* 	_context)
{
    TIMEBEGIN(1);
    setTemp.clear();
	if(mapMemory.empty())
	{
        TIMEEND(1);
		return;
	}
    std::string strLuaScript = "return redis.call('SINTER'";
    int nI = 0;
    std::vector<std::string> vecKey;
    std::string strKey;
    for(const auto &item : mapMemory)
    {
        ++nI;
        strLuaScript += boost::str(boost::format(" ,KEYS[%1%]") % nI);
        strKey = "mi_{" + strRobot + "}" + "_" + item.first + "_" + item.second;
        vecKey.push_back(strKey);
    }
    strLuaScript += ")";
    TESLOG(INFO, "findIntersection, strLuaScript = %s\n", strLuaScript.c_str());
    for (auto &iter: vecKey){
        TESLOG(INFO, "%s\n", (iter).c_str());
    }	
    const sds sdsLuaScript = sdsnew(strLuaScript.c_str());
    std::vector<std::string> vecCammond;
    vecCammond.push_back("EVAL");
    vecCammond.push_back(sdsLuaScript);
    sdsfree(sdsLuaScript);
    vecCammond.push_back(std::to_string(nI));
    vecCammond.insert(vecCammond.end(), vecKey.begin(), vecKey.end());

    char **pcArgv = convertToSds(vecCammond);
    size_t stArgc = vecCammond.size();
    std::unique_ptr<size_t[]> pArgvlen(new size_t[stArgc]);
    for (size_t nJ = 0; nJ < stArgc; nJ++)
    {
        pArgvlen[nJ] = sdslen(pcArgv[nJ]);
    }
    TIMEBEGIN(0);
    redisReply *pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());
    TIMEEND(0);

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
        sdsfreesplitres(pcArgv, stArgc);
        TIMEEND(1);
        return;
    }
    switch(pReply->type)
    {
        case REDIS_REPLY_INTEGER:
            TESLOG(INFO, "Reply[%llu]\n", pReply->integer);
            break;
        case REDIS_REPLY_STRING:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_ARRAY:
            TESLOG(INFO, "Elements[%lu]\n", pReply->elements);
            break;
        case REDIS_REPLY_STATUS:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_NIL:
            TESLOG(INFO, "Reply Nil\n");
            break;
        default:
            break;
    }

    for (size_t i = 0; i < pReply->elements; i++)
    {
        setTemp.insert(pReply->element[i]->str);
    }
    sdsfreesplitres(pcArgv, stArgc);	
    freeReplyObject(pReply);
    TIMEEND(1);
}

void findIntersection1(const std::string &strRobot, const std::map<std::string, std::string> &mapMemory,
                         std::set<std::string> &setTemp, redisClusterContext* 	_context)
{
    TIMEBEGIN(1);
    setTemp.clear();
	if(mapMemory.empty())
	{
        TIMEEND(1);
		return;
	}
    std::string cmd = "SINTER";
    std::vector<std::string> vecKey;
    std::string strKey;
    for(const auto &item : mapMemory)
    {
        cmd += " mi_{" + strRobot + "}" + "_" + item.first + "_" + item.second;
    }


    char ccmd[24 + cmd.length()] = { 0 };
    sprintf(ccmd, "%s", cmd.c_str());
    TESLOG(INFO, "findIntersection, ccmd = %s\n", ccmd);

    TIMEBEGIN(0);
    redisReply* pReply =  (redisReply*)redisClusterCommand(_context, ccmd);
    TIMEEND(0);

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
        TIMEEND(1);
        return;
    }
    switch(pReply->type)
    {
        case REDIS_REPLY_INTEGER:
            TESLOG(INFO, "Reply[%llu]\n", pReply->integer);
            break;
        case REDIS_REPLY_STRING:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_ARRAY:
            TESLOG(INFO, "Elements[%lu]\n", pReply->elements);
            break;
        case REDIS_REPLY_STATUS:
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
        case REDIS_REPLY_NIL:
            TESLOG(INFO, "Reply Nil\n");
            break;
        default:
            break;
    }

    for (size_t i = 0; i < pReply->elements; i++)
    {
        setTemp.insert(pReply->element[i]->str);
    }
    freeReplyObject(pReply);
    TIMEEND(1);
}

const static string g_underline = "_";
const static string g_liftBracket = "{";
const static string g_righeBracket = "}";
const static string g_score = "score";
const static string g_tilde = "~";
const static std::string g_kstrPrimaryTableHeader	=	"mp_";
const static std::string g_kstrIndexTableHeader		=	"mi_";
const static std::string g_kstrMemoryId				=	"memory_id"; 
const static std::string g_kstrLockName 			=	"distributed_lock";
const static std::string g_kstrBeginTime			=	"BeginTime";
const static std::string g_kstrEndTime				=	"EndTime";

void add_test(const string &strRobot, const string &strKey, const std::vector<vecString> &vVecContent, redisClusterContext* 	_context)
{
    TIMEBEGIN(1);
    string strSetKey = "{" + strRobot + "}" + "_"  + strKey;
    string strMember = "";
    string strSublist = "";
    // 分割strKey中的内容
    vector<string> vKeyMember;
    boost::split(vKeyMember, strKey, boost::is_any_of("_"), boost::token_compress_on);

    // 取出标记zadd的序列
    int score = 0;
    if (!incr(g_score.c_str(), score, _context))
    {
        TESLOG(ERROR, "incr %s %d fail!\n", g_score.c_str(), score);
        TIMEEND(1);
        return ;
    }

    string strValue = "";
    vecString vecTmp;
    string strContent = "";
    for (auto content: vVecContent)
    {
    	catsVector(content, "", "_", strContent);
    	vecTmp.push_back(strContent);
    }
    
    catsVector(vecTmp, "'", ",", strValue);
    // 拼接存入主表的内容
    string strLuaScript = boost::str(boost::format(" redis.call('sadd', KEYS[1], %1%) ") % strValue);
    vector<string> vKeys;
    int nI = 1;
    vKeys.push_back(strSetKey);
    
    string strTmpLua = "";
    // 拼接索引表的Key, 并序列标记插入
    for (auto keyMember : vKeyMember)
    {
        strSublist = "{" + strRobot + "}" + "_"  + keyMember;
        vKeys.push_back(strSublist);
        ++nI;
        strTmpLua = boost::str(boost::format(" redis.call('zadd', KEYS[%1%], %2%, '%3%') ") % nI % score % strKey);
        strLuaScript += strTmpLua;
        strTmpLua = "";
    }

    TESLOG(INFO, "strLuaScript : %s \n", strLuaScript.c_str());

    const sds sdsLuaScript = sdsnew(strLuaScript.c_str());
    std::vector<std::string> vecCammond;
    vecCammond.push_back("EVAL");
    vecCammond.push_back(sdsLuaScript);
    sdsfree(sdsLuaScript);
    vecCammond.push_back(std::to_string(nI));
    for(const auto &item : vKeys)
    {
        TESLOG(INFO, "vKeys : %s\n", item.c_str());
        vecCammond.push_back(item);
    }

    char **pcArgv = convertToSds(vecCammond);
    size_t stArgc = vecCammond.size();
    std::unique_ptr<size_t[]> pArgvlen(new size_t[stArgc]);
    for (size_t nJ = 0; nJ < stArgc; nJ++)
    {
        pArgvlen[nJ] = sdslen(pcArgv[nJ]);
    }

    redisReply *pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());
    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
    }
    sdsfreesplitres(pcArgv, stArgc);	
    freeReplyObject(pReply);
    TIMEEND(1);
}

bool setMultiLatitude(const string &strRobot, const string &strKey, const std::vector<vecString> &vVecContent, redisClusterContext* _context)
{
    TIMEBEGIN(1);
    string strSetKey = g_liftBracket + strRobot + g_righeBracket + g_underline  + strKey;
    string strMember = "";
    string strSublist = "";
    // 分割strKey中的内容
    vector<string> vKeyMember;
    boost::split(vKeyMember, strKey, boost::is_any_of(g_underline), boost::token_compress_on);

    // 取出标记zadd的序列
    int score = 0;
    if (!incr(g_score.c_str(), score, _context))
    {
        TESLOG(ERROR, "incr %s %d fail!\n", g_score.c_str(), score);
        TIMEEND(1);
        return false;
    }
    for (auto content : vVecContent)
    {
        // 拼接存入主表的内容
        for (size_t i = 0; i < content.size(); ++i)
        {
            strMember += content[i];
            if (i < (content.size() - 1))
            {
                strMember += g_underline;
            }
        }
        if (!sadd(strSetKey.c_str(), strMember.c_str(), _context))
        {
            TESLOG(ERROR, "sadd %s %s fail!\n", strSetKey.c_str(), strMember.c_str());
        }
        // 拼接索引表的Key, 并序列标记插入
        for (auto keyMember : vKeyMember)
        {
            strSublist += g_liftBracket + strRobot + g_righeBracket + g_underline  + keyMember;
            if (!zadd(strSublist.c_str(), score, strKey.c_str(), _context))
            {
                TESLOG(ERROR, "zadd %s %d %s fail!\n", strSublist.c_str(), score, strKey.c_str());
            }
            strSublist = "";
        }
        strMember = "";
    }
    TIMEEND(1);
    return true;
}

int splitMemoryId(const std::string &item)
{
    size_t stPos = item.find_last_of("_");
    std::string	strMemoryId = item.substr(stPos+1); 
    int nMemoryId = std::atoi(strMemoryId.c_str());
    return nMemoryId;
}

void getMemory(const std::string &strRobot, const std::map<std::string, std::string> &mapCondition,
    bool bFlag, std::vector<int> &vecMemoryId, redisClusterContext* _context)
{
    TIMEBEGIN(1);
    vecMemoryId.clear();
    
    TESLOG(INFO, "getMemory, robot = %s, bflag = %d", strRobot.c_str(), bFlag);
	std::map<std::string, std::string> mapMemory;
	std::vector<std::string> vecNa;
	std::vector<std::string> vecNb;
    int nFieldNum = 0;
    for(const auto &item : mapCondition)
    {
        TESLOG(INFO, "getMemory, key:%s, value:%s", item.first.c_str(), item.second.c_str());
		if(item.second == "NA")
		{
			vecNa.push_back(item.first);
		}
		else if(item.second == "NB")
		{
			vecNb.push_back(item.first);
            nFieldNum++;
		}
		else
		{
			mapMemory.insert(item);
            nFieldNum++;
		}
    }

    std::set<std::string> setTemp;
    findIntersection(strRobot, mapMemory, setTemp, _context);
    if(setTemp.empty())
    {
        TESLOG(INFO, "query condition is not exist");
        TIMEEND(1);
        return;
    }
    std::string cmd = "";
    char ccmd[24 + cmd.length()] = { 0 };

    if(bFlag)
    {
        std::string strKey;
        std::map<std::string, std::string> mapOut;
        for(const auto& item : setTemp)
        {
            strKey.clear();
            mapOut.clear();
            strKey = "mp_" + item;

            if(hgetall(strKey.c_str(), mapOut, _context))
            {
				bool bFlag = true;
				for(const auto & i : vecNa)
				{
					auto iter = mapOut.find(i);
					if(iter != mapOut.end())
					{
						bFlag = false;
						break;
					}
				}
				if(!bFlag)
				{
					continue;
				}
				for(const auto & i : vecNb)
				{
					auto iter = mapOut.find(i);
					if(iter == mapOut.end())
					{
						bFlag = false;
						break;
					}
				}
				if(!bFlag)
				{
					continue;
				}

                int nNum = 0;
                for(const auto &item : mapOut)
                {
                    if((item.first == "BeginTime") || (item.first == "EndTime"))
                    {
                        continue;
                    }
                    ++nNum;
                }
                TESLOG(ERROR, "nNum: %d  nFieldNum: %d", nNum, nFieldNum);
                if(nNum == nFieldNum)
                {
                    vecMemoryId.push_back(splitMemoryId(item));
                }
            }
            else
            {
                TESLOG(ERROR, "hgetall %s failed", strKey.c_str());
                TIMEEND(1);
                return ;
            }
        }	
    }
    else
    {
		std::string strKey;
        std::map<std::string, std::string> mapOut;
        for(const auto& item : setTemp)
        {
            strKey.clear();
            mapOut.clear();
            strKey = "mp_" + item;

            if(hgetall(strKey.c_str(), mapOut, _context))
            {
				bool bFlag = true;
				for(const auto & i : vecNa)
				{
					auto iter = mapOut.find(i);
					if(iter != mapOut.end())
					{
						bFlag = false;
						break;
					}
				}
				if(!bFlag)
				{
					continue;
				}
				for(const auto & i : vecNb)
				{
					auto iter = mapOut.find(i);
					if(iter == mapOut.end())
					{
						bFlag = false;
						break;
					}
				}
				if(!bFlag)
				{
					continue;
				}
				vecMemoryId.push_back(splitMemoryId(item));
			}
		}
    }
    TIMEEND(1);
}

bool delMemoryAchieve(const std::string &strRobot, const std::multimap<std::string, std::pair<std::string, std::string>> &mapCondition, 
                        bool bFlag, redisClusterContext* _context)
{
	TIMEBEGIN(1);
	TESLOG(INFO, "delMemory, robot = %s, bFlag = %d", strRobot.c_str(), bFlag);
	std::map<std::string, std::string> mapMemory;
	for(const auto& item : mapCondition)
	{
		TESLOG(INFO, "operator: %s, 标签名:%s, 标签值:%s", item.first.c_str(), item.second.first.c_str(), item.second.second.c_str());
		mapMemory.insert(item.second);
	}
	std::vector<int> vecMemoryId;

	getMemory(strRobot, mapMemory, bFlag, vecMemoryId, _context);
	if(vecMemoryId.empty())	
	{
		TESLOG(INFO, "the memory is not existed");
		TIMEEND(1);
		return true;
	}
	std::set<std::string> setTemp;
	for(auto item : vecMemoryId)
	{
		setTemp.insert(boost::str(boost::format("{%1%}_%2%")% strRobot % item ));
	}

	std::vector<std::string> vecDelPrimaryTable;
	std::vector<std::string> vecDelIndex;
	std::map<std::string, std::vector<std::string>> mapIndexItem;
	if(bFlag)
	{
		std::string strIndex;
		for(const auto &item : mapMemory)
		{
			strIndex = g_kstrIndexTableHeader + "{" + strRobot + "}_" + item.first + "_" + item.second;
			vecDelIndex.push_back(strIndex);
		}	

		std::string strKey;
		std::map<std::string, std::string> mapOut;
		for(const auto &item : setTemp)
		{
			strKey.clear();
			mapOut.clear();
			strKey = g_kstrPrimaryTableHeader + item;
			if(hgetall(strKey.c_str(), mapOut, _context))
			{
				size_t sNum = 0;
				for(const auto &item : mapOut)
				{
					if((item.first == g_kstrBeginTime) || (item.first == g_kstrEndTime))
					{
						continue;
					}
					++sNum;
				}
				if(sNum == mapMemory.size())
				{
					vecDelPrimaryTable.push_back(strKey);
					mapIndexItem[item] = vecDelIndex;
				}
			}
			else
			{
				TESLOG(ERROR, "hvals %s failed", strKey.c_str());
				TIMEEND(1);
				return false;
			}
		}
		if(vecDelPrimaryTable.empty())
		{
			TESLOG(INFO, "complete matching is not existed");
			TIMEEND(1);
			return false;
		}
	}
	else
	{
		std::string strKey;
		std::map<std::string, std::string> mapOut;
		for(const auto& item : setTemp)
		{
			strKey = g_kstrPrimaryTableHeader + item;
			vecDelPrimaryTable.push_back(strKey);
			mapOut.clear();
            vecDelIndex.clear();
			if(!hgetall(strKey.c_str(), mapOut, _context))
			{
				TESLOG(ERROR, "hgetall %s failed", strKey.c_str());
				TIMEEND(1);
				return false;
			}
			std::string strIndex;
			for(const auto &item : mapOut)
			{
				if((item.first == g_kstrBeginTime) || (item.first == g_kstrEndTime))
				{
					continue;
				}
				strIndex = g_kstrIndexTableHeader + "{" + strRobot + "}_" + item.first + "_" + item.second;
				vecDelIndex.push_back(strIndex);
			}
			mapIndexItem[item] = vecDelIndex;
		}

	}
		
	std::string strLuaScript = "redis.call('del' ";
	int nI = 0;
	for(const auto &item : vecDelPrimaryTable)
	{
		++nI;
		strLuaScript += boost::str(boost::format(" ,KEYS[%1%]") % nI);	
	}
	strLuaScript += ") ";

    int argNum = 0;
	for(const auto &item : mapIndexItem)
	{
        ++argNum;
		for(const auto &em : item.second)
		{
			++nI;
			strLuaScript += boost::str(boost::format(" redis.call('srem' ,KEYS[%1%] ,'%2%')") % nI % item.first);
		}
	}

	TESLOG(INFO, "del memory : %s", strLuaScript.c_str());

	const sds sdsLuaScript = sdsnew(strLuaScript.c_str());
	std::vector<std::string> vecCammond;
	vecCammond.push_back("EVAL");
	vecCammond.push_back(sdsLuaScript);
	sdsfree(sdsLuaScript);
	vecCammond.push_back(std::to_string(nI));
	for(const auto &item : vecDelPrimaryTable)
	{
		vecCammond.push_back(item);
	}

	for(const auto &item : mapIndexItem)
	{
		for(const auto &em : item.second)
		{
			vecCammond.push_back(em);
		}
	}

	char **pcArgv = convertToSds(vecCammond);
	size_t stArgc = vecCammond.size();
	//size_t *pstArgvlen = (size_t *)malloc(stArgc * sizeof(size_t));
	std::unique_ptr<size_t[]> pArgvlen(new size_t[stArgc]);
	for (int nJ = 0; nJ < stArgc; nJ++)
	{
		pArgvlen[nJ] = sdslen(pcArgv[nJ]);
	}

	redisReply *pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());
	if(pReply == NULL)
	{
		sdsfreesplitres(pcArgv, stArgc);	
		TESLOG(ERROR, "pReply == NULL");
		TIMEEND(1);
		return false;
	}
	sdsfreesplitres(pcArgv, stArgc);	
	freeReplyObject(pReply);

	TIMEEND(1);
	return true;
}

bool delMemoryAchieve1(const std::string &strRobot, const std::multimap<std::string, std::pair<std::string, std::string>> &mapCondition, 
                        bool bFlag, redisClusterContext* _context)
{
	TIMEBEGIN(1);
	TESLOG(INFO, "delMemory, robot = %s, bFlag = %d", strRobot.c_str(), bFlag);
	std::map<std::string, std::string> mapMemory;
	for(const auto& item : mapCondition)
	{
		TESLOG(INFO, "operator: %s, 标签名:%s, 标签值:%s", item.first.c_str(), item.second.first.c_str(), item.second.second.c_str());
		mapMemory.insert(item.second);
	}
	std::vector<int> vecMemoryId;

	getMemory(strRobot, mapMemory, bFlag, vecMemoryId, _context);
	if(vecMemoryId.empty())	
	{
		TESLOG(INFO, "the memory is not existed");
		TIMEEND(1);
		return true;
	}
	std::set<std::string> setTemp;
	for(auto item : vecMemoryId)
	{
		setTemp.insert(boost::str(boost::format("{%1%}_%2%")% strRobot % item ));
	}

	std::vector<std::string> vecDelPrimaryTable;
	std::vector<std::string> vecDelIndex;
	std::map<std::string, std::vector<std::string>> mapIndexItem;
	if(bFlag)
	{
		std::string strIndex;
		for(const auto &item : mapMemory)
		{
			strIndex = g_kstrIndexTableHeader + "{" + strRobot + "}_" + item.first + "_" + item.second;
			vecDelIndex.push_back(strIndex);
		}	

		std::string strKey;
		std::map<std::string, std::string> mapOut;
		for(const auto &item : setTemp)
		{
			strKey.clear();
			mapOut.clear();
			strKey = g_kstrPrimaryTableHeader + item;
			if(hgetall(strKey.c_str(), mapOut, _context))
			{
				size_t sNum = 0;
				for(const auto &item : mapOut)
				{
					if((item.first == g_kstrBeginTime) || (item.first == g_kstrEndTime))
					{
						continue;
					}
					++sNum;
				}
				if(sNum == mapMemory.size())
				{
					vecDelPrimaryTable.push_back(strKey);
					mapIndexItem[item] = vecDelIndex;
				}
			}
			else
			{
				TESLOG(ERROR, "hvals %s failed", strKey.c_str());
				TIMEEND(1);
				return false;
			}
		}
		if(vecDelPrimaryTable.empty())
		{
			TESLOG(INFO, "complete matching is not existed");
			TIMEEND(1);
			return false;
		}
	}
	else
	{
		std::string strKey;
		std::map<std::string, std::string> mapOut;
		for(const auto& item : setTemp)
		{
			strKey = g_kstrPrimaryTableHeader + item;
			vecDelPrimaryTable.push_back(strKey);
			mapOut.clear();
            vecDelIndex.clear();
			if(!hgetall(strKey.c_str(), mapOut, _context))
			{
				TESLOG(ERROR, "hgetall %s failed", strKey.c_str());
				TIMEEND(1);
				return false;
			}
			std::string strIndex;
			for(const auto &item : mapOut)
			{
				if((item.first == g_kstrBeginTime) || (item.first == g_kstrEndTime))
				{
					continue;
				}
				strIndex = g_kstrIndexTableHeader + "{" + strRobot + "}_" + item.first + "_" + item.second;
				vecDelIndex.push_back(strIndex);
			}
			mapIndexItem[item] = vecDelIndex;
		}

	}
		
	std::string strLuaScript = "redis.call('del' ";
	int nI = 0;
	for(const auto &item : vecDelPrimaryTable)
	{
		++nI;
		strLuaScript += boost::str(boost::format(" ,KEYS[%1%]") % nI);	
	}
	strLuaScript += ") ";

    int argNum = 0;
	for(const auto &item : mapIndexItem)
	{
        ++argNum;
		for(const auto &em : item.second)
		{
			++nI;
			strLuaScript += boost::str(boost::format(" redis.call('srem' ,KEYS[%1%] ,ARGV[%2%])") % nI % argNum);
		}
	}

	TESLOG(INFO, "del memory : %s", strLuaScript.c_str());

	const sds sdsLuaScript = sdsnew(strLuaScript.c_str());
	std::vector<std::string> vecCammond;
	vecCammond.push_back("EVAL");
	vecCammond.push_back(sdsLuaScript);
	sdsfree(sdsLuaScript);
	vecCammond.push_back(std::to_string(nI));
	for(const auto &item : vecDelPrimaryTable)
	{
		vecCammond.push_back(item);
	}

	for(const auto &item : mapIndexItem)
	{
		for(const auto &em : item.second)
		{
			vecCammond.push_back(em);
		}
	}

    for(const auto &item : mapIndexItem)
	{
        vecCammond.push_back(item.first);
	}

	char **pcArgv = convertToSds(vecCammond);
	size_t stArgc = vecCammond.size();
	//size_t *pstArgvlen = (size_t *)malloc(stArgc * sizeof(size_t));
	std::unique_ptr<size_t[]> pArgvlen(new size_t[stArgc]);
	for (int nJ = 0; nJ < stArgc; nJ++)
	{
		pArgvlen[nJ] = sdslen(pcArgv[nJ]);
	}

	redisReply *pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());
	if(pReply == NULL)
	{
		sdsfreesplitres(pcArgv, stArgc);	
		TESLOG(ERROR, "pReply == NULL");
		TIMEEND(1);
		return false;
	}
	sdsfreesplitres(pcArgv, stArgc);	
	freeReplyObject(pReply);

	TIMEEND(1);
	return true;
}

bool run(std::string key){
    redisClusterContext* 	_context;
    std::string 			_addr = "172.16.0.23:7000";
    int flags = HIRCLUSTER_FLAG_NULL;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    _context = redisClusterConnectWithTimeout(_addr.c_str(), timeout, flags);
    if(_context == NULL || _context->err)
    {
        if (_context) 
        {
            TESLOG(ERROR, "Redis Connection Error[%s]\n", _context->errstr);
            redisClusterFree(_context);
        } 
        else 
        {
            TESLOG(ERROR, "Redis Connection Error[can't allocate redis context]\n");
        }
        _context = NULL;
    }
    TESLOG(INFO, "Redis Connect to addr[%s] Success\n", _addr.c_str());

    Set(key.c_str(), "123", _context);
    std::string out;
    get(key.c_str(), out, _context);
    TESLOG(INFO, "key %s, out %s\n", key.c_str(), out.c_str());
    del(key.c_str(), _context);
    sleep(10);
    if(_context)
    {
        TESLOG(ERROR, "Disconnect From Redis Server[%s]\n", _addr.c_str());
        redisClusterFree(_context);
    }
}

bool thread_run(int num){

    std::string key = "jgbtest";
    char k[100]= {0};
    std::thread *p[num];
    for(int i = 0; i < num; i++){
        sprintf(k, "%s%d", key.c_str(), i);
        std::string keys = k;
        TESLOG(INFO, "keys %s\n", keys.c_str());
        p[i] = new std::thread(run, keys);
        if (p[i] == nullptr) {
            TESLOG(INFO, "create pthread error!\n");
            return -1;
        }
        
    }
    for(int i = 0; i < num; i++){
        p[i]->join();
    }
}

void redis_test(){
    redisClusterContext* 	_context;
    std::string 			_addr = "172.16.0.23:7000";
    int flags = HIRCLUSTER_FLAG_NULL;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    _context = redisClusterConnectWithTimeout(_addr.c_str(), timeout, flags);
    if(_context == NULL || _context->err)
    {
        if (_context) 
        {
            TESLOG(ERROR, "Redis Connection Error[%s]\n", _context->errstr);
            redisClusterFree(_context);
        } 
        else 
        {
            TESLOG(ERROR, "Redis Connection Error[can't allocate redis context]\n");
        }
        _context = NULL;
    }
    TESLOG(INFO, "Redis Connect to addr[%s] Success\n", _addr.c_str());

#if 0
    std::map<std::string, std::string> mapMemory;
// mi_{181026033809ffdc196c965aRI000004}_需提醒_是
// mi_{181026033809ffdc196c965aRI000004}_程度_剧烈
// mi_{181026033809ffdc196c965aRI000004}_类型_刺痛
// mi_{181026033809ffdc196c965aRI000004}_症状_咳嗽
    mapMemory.insert(make_pair("习惯行为","下班"));
    mapMemory.insert(make_pair("主语","1812111752213087b6fe75a7RI003706"));
    mapMemory.insert(make_pair("日期","NA"));
    mapMemory.insert(make_pair("分钟","NA"));
    //mapMemory.insert(make_pair("类型","刺痛"));

    std::vector<int> setTemp;

    TESLOG(INFO, "fun\n");
    getMemory("1812111752213087b6fe75a7RI003706", mapMemory, false, setTemp, _context);
    for (auto &iter: setTemp){
        TESLOG(INFO, "%d\n", iter);
    }
    
#endif

#if 0
    std::multimap<std::string, std::pair<std::string, std::string>> mapCondition;
    mapCondition.emplace(std::make_pair("=",std::make_pair("姓名","江冠斌")));
    mapCondition.emplace(std::make_pair("=",std::make_pair("年龄","23")));
    delMemoryAchieve("000000000000000000000000RI000000", mapCondition, true, _context);
#endif
#if 1
    thread_run(1000);
#endif
    if(_context)
    {
        TESLOG(ERROR, "Disconnect From Redis Server[%s]\n", _addr.c_str());
        redisClusterFree(_context);
    }
}
