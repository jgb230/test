#ifndef __REDIS_H
#define __REDIS_H

extern "C"{
#include <hiredis-vip/hiredis.h>
#include <hiredis-vip/adlist.h>
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


#include <iostream>
#include <sstream>

#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/classification.hpp>

#include "common.hpp"

using namespace std;

typedef vector<string> vecString;

void getMemory(const std::string &strRobot, const std::multimap<std::string, std::pair<std::string, std::string>> &mapMemory,
                   bool bFlag, std::vector<int> &vecMemoryId, redisClusterContext* _context);
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


bool zinterstore(const char* robot, const char** key, const int number, std::string &strOut, redisClusterContext* _context)
{
    if(!key) return false;

    char interstore[1024] = { 0 };

    if(!_context)
    {
        TESLOG(ERROR, "Execute interstore[%s] Failed, No Connected", interstore);
        return false;
    }

    int count = number + 3;
    const char *keys[count];
    char inter[64] = {0};
    sprintf(inter, "{%s}:out:jgbtest", robot);
    TESLOG(ERROR, "inter %s \n", inter);
    char num[64] = {0};
    sprintf(num, "%d", number);
    TESLOG(ERROR, "num %s \n", num);
    keys[0] = "ZINTERSTORE";
    keys[1] = inter;
    keys[2] = num;

    for (int i = 0; i < number; i++){
        keys[i + 3] = key[i];
        TESLOG(ERROR, "keys %s \n", keys[i + 3]);
    }


    redisReply* reply = (redisReply*)redisClusterCommandArgv(_context, count, keys, nullptr );
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute interstore Failed, No Reply cc->type[%d], cc->errstr[%s]\n", 
                                _context->err, _context->errstr);
        if (REDIS_ERR_EOF == _context->err){ //　连接被关闭，重新执行命令
            reply = (redisReply*)redisClusterCommandArgv(_context, count, keys, nullptr );
            if (reply == NULL ){
                TESLOG(ERROR, "reply == NULL, No Reply ,cc->type[%d], cc->errstr[%s]", 
                                _context->err, _context->errstr);
                return false;
            }
        }else{
            return false;
        }
    }

    char out[128] = { 0 };
    sprintf(out, "ZRANGE {%s}:out:jgbtest -1 -1", robot);
    TESLOG(ERROR, "out %s \n", out);
    reply = (redisReply*)redisClusterCommand(_context, "ZRANGE {%s}:out:jgbtest -1 -1", robot);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute out[%s] Failed, No Reply  cc->type[%d], cc->errstr[%s]\n", 
                                _context->err, _context->errstr);
        if (REDIS_ERR_EOF == _context->err){ //　连接被关闭，重新执行命令
            reply = (redisReply*)redisClusterCommand(_context, "ZRANGE {%s}:out:jgbtest -1 -1", robot);
            if (reply == NULL ){
                TESLOG(ERROR, "reply == NULL, No Reply ,cc->type[%d], cc->errstr[%s]", 
                                _context->err, _context->errstr);
                return false;
            }
        }else{
            return false;
        }
    }
    TESLOG(ERROR, "reply %d\n", reply->type);
    switch(reply->type)
    {
    case REDIS_REPLY_INTEGER:
        TESLOG(INFO, "Execute out[%s] -- Reply[%llu]", out, reply->integer);
        break;
    case REDIS_REPLY_STRING:
        TESLOG(INFO, "Execute out[%s] -- Reply[%s]", out, reply->str);
        break;
    case REDIS_REPLY_STATUS:
        TESLOG(INFO, "Execute out[%s] -- Reply[%s]", out, reply->str);
        break;
    case REDIS_REPLY_NIL:
        TESLOG(INFO, "Execute out[%s] -- Reply Nil", out);
        break;
    default:
        break;
    }
    TESLOG(ERROR, "element %p\n", reply->element);
    TESLOG(ERROR, "str %s\n", (*reply->element)->str);
    if ((reply->element) != NULL)
    	strOut = (*reply->element)->str == NULL ? "" : (*reply->element)->str;

    freeReplyObject(reply);

    return strOut == "" ? false : true;

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
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply ,cc->type[%d], cc->errstr[%s]\n", cmd, _context->err, _context->errstr);
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
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply ,cc->type[%d], cc->errstr[%s]\n", cmd, _context->err, _context->errstr);
        return false;
    }
    freeReplyObject(reply);

    return true;
}


bool clusterSlots(std::string &Out, redisClusterContext* _context)
{

    char cmd[24] = { 0 };
    sprintf(cmd, "cluster slots");

    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected\n", cmd);
        return false;
    }
    redisReply* reply =  (redisReply*)redisClusterCommand(_context, "cluster slots");
    if(reply == NULL)
    {
        TESLOG(INFO, "Execute cmd[%s] Failed, No Reply ,cc->type[%d], cc->errstr[%s], retry:\n", cmd, _context->err, _context->errstr);
        if (REDIS_ERR_EOF == _context->err){
            reply =  (redisReply*)redisClusterCommand(_context, "cluster slots");
            if (reply == NULL ){
                TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply ,cc->type[%d], cc->errstr[%s]\n", cmd, _context->err, _context->errstr);
                return false;
            }
        }
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
        case REDIS_REPLY_ARRAY:
            TESLOG(INFO, "Execute cmd[%s] -- Elements[%lu]", cmd, reply->elements);
            break;
        default:
            break;
    }

	Out = reply->str;
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
        TESLOG(INFO, "Execute cmd[%s] Failed, No Reply ,cc->type[%d], cc->errstr[%s], retry:\n", cmd, _context->err, _context->errstr);
        if (REDIS_ERR_EOF == _context->err){
            reply =  (redisReply*)redisClusterCommand(_context, "get %s", key);
            if (reply == NULL ){
                TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply ,cc->type[%d], cc->errstr[%s]\n", cmd, _context->err, _context->errstr);
                return false;
            }
        }
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

bool hmset(redisClusterContext* _context, const char* key, const char* format, ...)
{
    if(!format) return false;

    va_list ap;
    char filedValues[1024] = { 0 };  
    va_start(ap, format);
    int res = vsnprintf(filedValues, 1024, format, ap);
    va_end(ap);

    if(res == -1)
    {
        TESLOG(ERROR, "Redis cmd Truncated (And Not Execute) For Format[%s]", format);
        return false;          
    }

    char cmd[24 + strlen(key) + strlen(filedValues)] = { 0 };
    sprintf(cmd, "hmset %s %s", key, filedValues);

    if(!_context)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Connected", cmd);
        return false;
    }
    redisReply* reply =  (redisReply*)redisClusterCommand(_context, "hmset %s %s", key, filedValues);
    if(reply == NULL)
    {
        TESLOG(ERROR, "Execute cmd[%s] Failed, No Reply , cc->type[%d], cc->errstr[%s]", cmd, _context->err, _context->errstr);
        if (REDIS_ERR_EOF == _context->err){ //　连接被关闭，重新执行命令
            reply =  (redisReply*)redisClusterCommand(_context, "hmset %s %s", key, filedValues);
            if (reply == NULL ){
                TESLOG(ERROR, "reply == NULL, No Reply ,cc->type[%d], cc->errstr[%s]", 
                                _context->err, _context->errstr);
                return false;
            }
        }else{
            return false;
        }
    }



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
}

void test1(redisClusterContext* _context){
    
    std::string cmd = "sadd {1jgbtest} test1";
    char ccmd[24 + cmd.length()] = { 0 };
    sprintf(ccmd, "%s", cmd.c_str());
    TESLOG(INFO, "test1, ccmd = %s\n", ccmd);

    redisReply* pReply =  (redisReply*)redisClusterCommand(_context, ccmd);

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
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
}

void findIntersection(const std::string &strRobot, const std::map<std::string, std::string> &mapMemory,
                         std::set<std::string> &setTemp, redisClusterContext* 	_context)
{
    setTemp.clear();
	if(mapMemory.empty())
	{
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
    redisReply *pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
        sdsfreesplitres(pcArgv, stArgc);
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
}

void findIntersection1(const std::string &strRobot, const std::map<std::string, std::string> &mapMemory,
                         std::set<std::string> &setTemp, redisClusterContext* 	_context)
{
    setTemp.clear();
	if(mapMemory.empty())
	{
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

    redisReply* pReply =  (redisReply*)redisClusterCommand(_context, ccmd);

    if(pReply == NULL)
    {
        TESLOG(ERROR, "pReply == NULL\n");
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
}

bool setMultiLatitude(const string &strRobot, const string &strKey, const std::vector<vecString> &vVecContent, redisClusterContext* _context)
{
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
    vecMemoryId.clear();
    
    TESLOG(INFO, "getMemory, robot = %s, bflag = %d\n", strRobot.c_str(), bFlag);
	std::map<std::string, std::string> mapMemory;
	std::vector<std::string> vecNa;
	std::vector<std::string> vecNb;
    int nFieldNum = 0;
    for(const auto &item : mapCondition)
    {
        TESLOG(INFO, "getMemory, key:%s, value:%s\n", item.first.c_str(), item.second.c_str());
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
        TESLOG(INFO, "query condition is not exist\n");
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
                TESLOG(ERROR, "nNum: %d  nFieldNum: %d\n", nNum, nFieldNum);
                if(nNum == nFieldNum)
                {
                    vecMemoryId.push_back(splitMemoryId(item));
                }
            }
            else
            {
                TESLOG(ERROR, "hgetall %s failed\n", strKey.c_str());
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
}

bool delMemoryAchieve(const std::string &strRobot, const std::multimap<std::string, std::pair<std::string, std::string>> &mapCondition, 
                        bool bFlag, redisClusterContext* _context)
{
	TESLOG(INFO, "delMemory, robot = %s, bFlag = %d\n", strRobot.c_str(), bFlag);
	std::map<std::string, std::string> mapMemory;
	for(const auto& item : mapCondition)
	{
		TESLOG(INFO, "operator: %s, 标签名:%s, 标签值:%s\n", item.first.c_str(), item.second.first.c_str(), item.second.second.c_str());
		mapMemory.insert(item.second);
	}
	std::vector<int> vecMemoryId;

	getMemory(strRobot, mapMemory, bFlag, vecMemoryId, _context);
	if(vecMemoryId.empty())	
	{
		TESLOG(INFO, "the memory is not existed\n");
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
				TESLOG(ERROR, "hvals %s failed\n", strKey.c_str());
				return false;
			}
		}
		if(vecDelPrimaryTable.empty())
		{
			TESLOG(INFO, "complete matching is not existed\n");
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
				TESLOG(ERROR, "hgetall %s failed\n", strKey.c_str());
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

	TESLOG(INFO, "del memory : %s\n", strLuaScript.c_str());

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
		TESLOG(ERROR, "pReply == NULL\n");
		return false;
	}
	sdsfreesplitres(pcArgv, stArgc);	
	freeReplyObject(pReply);

	return true;
}

bool delMemoryAchieve1(const std::string &strRobot, const std::multimap<std::string, std::pair<std::string, std::string>> &mapCondition, 
                        bool bFlag, redisClusterContext* _context)
{
	TESLOG(INFO, "delMemory, robot = %s, bFlag = %d\n", strRobot.c_str(), bFlag);
	std::map<std::string, std::string> mapMemory;
	for(const auto& item : mapCondition)
	{
		TESLOG(INFO, "operator: %s, 标签名:%s, 标签值:%s\n", item.first.c_str(), item.second.first.c_str(), item.second.second.c_str());
		mapMemory.insert(item.second);
	}
	std::vector<int> vecMemoryId;

	getMemory(strRobot, mapMemory, bFlag, vecMemoryId, _context);
	if(vecMemoryId.empty())	
	{
		TESLOG(INFO, "the memory is not existed\n");
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
				TESLOG(ERROR, "hvals %s failed\n", strKey.c_str());
				return false;
			}
		}
		if(vecDelPrimaryTable.empty())
		{
			TESLOG(INFO, "complete matching is not existed\n");
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
				TESLOG(ERROR, "hgetall %s failed\n", strKey.c_str());
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

	TESLOG(INFO, "del memory : %s\n", strLuaScript.c_str());

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
		TESLOG(ERROR, "pReply == NULL\n");
		return false;
	}
	sdsfreesplitres(pcArgv, stArgc);	
	freeReplyObject(pReply);

	return true;
}


std::string makeTableForAddMemory(const std::string &strRobot, int nMemoryId, const std::map<std::string, std::string> &mapHashTable)
{
    std::string strMemoryId = std::to_string(nMemoryId);
    std::string strIndexContent = "{" + strRobot + "}_" + strMemoryId;
    std::string strPrimaryTable = "redis.call('hmset', KEYS[1]";
    std::string strIndexTable;
    int nI = 1;
    for(const auto& item : mapHashTable)
    {
        strPrimaryTable += boost::str(boost::format(", '%1%', '%2%'") % item.first % item.second);
        if((item.first == g_kstrBeginTime) || (item.first == g_kstrEndTime))
        {
            continue;
        }
        ++nI;
        strIndexTable += boost::str(boost::format("redis.call('sadd', KEYS[%1%], ") % nI);
        strIndexTable += "'" + strIndexContent;
        strIndexTable += "') ";
    }

    strPrimaryTable += ") ";
    strPrimaryTable += strIndexTable;
    return strPrimaryTable;
}

bool addMemoryAchieve(const std::string &strRobot, const std::map<std::string, std::string> &mapMemory, redisClusterContext* _context)
{

    std::map<std::string, std::string> mapTime;
    std::map<std::string, std::string> mapNoTime;
    for(const auto & item : mapMemory)
    {
        TESLOG(INFO, "add memory, key: %s, value: %s\n", item.first.c_str(), item.second.c_str());
        if((item.first == g_kstrBeginTime) || (item.first == g_kstrEndTime))
        {
            mapTime.insert(std::pair<std::string, std::string>(item.first, item.second));
        }
        else
        {
            mapNoTime.insert(std::pair<std::string, std::string>(item.first, item.second));
        }
    }

    std::vector<int> vecMemoryId;
    getMemory(strRobot, mapNoTime, true, vecMemoryId, _context);
    if(!vecMemoryId.empty())
    {
        for(const auto & item : vecMemoryId)
        {
            std::string strKey = g_kstrPrimaryTableHeader + "{" + strRobot + "}_" + std::to_string(item);
            TESLOG(WARNING, "add memory fail, and update the time of the memory: %d, primary key: %s\n", item, strKey.c_str());
            bool ret = hmset(_context, strKey.c_str(), "%s %s %s %s", g_kstrBeginTime.c_str(), mapTime[g_kstrBeginTime].c_str(), g_kstrEndTime.c_str(), mapTime[g_kstrEndTime].c_str());
            if (!ret)
            {
                TESLOG(ERROR, "hmset %s fail\n", strKey.c_str());	
                return false;
            }
        }
        return true;
    }

    int nMemoryId = 0;
    if(!incr(g_kstrMemoryId.c_str(), nMemoryId, _context))
    {
        TESLOG(ERROR, "can not get zhe memory id\n");
        return false;
    }
    std::string strLuaScript = makeTableForAddMemory(strRobot, nMemoryId, mapMemory);
    TESLOG(INFO, "add memory ,strLuaScript = %s\n", strLuaScript.c_str());
    std::string strMemoryPrimary = "mp_{" + strRobot + "}_" + std::to_string(nMemoryId);
    const sds sdsLuaScript = sdsnew(strLuaScript.c_str());
    std::vector<std::string> vecCammond;
    vecCammond.push_back("EVAL");
    vecCammond.push_back(sdsLuaScript);
    sdsfree(sdsLuaScript);
    std::string strKeyNum = std::to_string(mapNoTime.size()+1);
    vecCammond.push_back(strKeyNum);
    vecCammond.push_back(strMemoryPrimary);
    std::string strIndex;
    for(const auto& item : mapNoTime)
    {
        strIndex.clear();
        strIndex = "mi_{" + strRobot + "}_" + item.first + "_" + item.second;
        vecCammond.push_back(strIndex);
    }

    for(auto &item : vecCammond)
    {
        TESLOG(INFO, "cammond: %s\n", item.c_str());
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
        TESLOG(INFO, "pReply == NULL, No Reply ,cc->type[%d], cc->errstr[%s], retry:\n", 
                     _context->err, _context->errstr);
        if (REDIS_ERR_EOF == _context->err){ //　连接被关闭，重新执行命令
            pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());
            if (pReply == NULL ){
                sdsfreesplitres(pcArgv, stArgc);
                TESLOG(ERROR, "pReply == NULL, No Reply ,cc->type[%d], cc->errstr[%s]\n", 
                                _context->err, _context->errstr);
                return false;
            }
        }else{
            sdsfreesplitres(pcArgv, stArgc);	
            return false;
        }
    }
    sdsfreesplitres(pcArgv, stArgc);	
    freeReplyObject(pReply);
    return true;
}


bool runLuaScript(const std::string &strKey, const std::map<std::string, std::string> &mapKeyValue, const std::string &strItem,
                  const std::vector<std::string> &vecDelIndex, const std::vector<std::string> &vecAddIndex, redisClusterContext* _context)
{
	std::map<std::string, std::string> mapNa;
	std::map<std::string, std::string> mapExcludeNa;

	for(const auto& item : mapKeyValue)
	{
		if(item.second == "NA")
		{
			mapNa.insert(std::pair<std::string, std::string>(item.first, item.second));
		}
		else
		{
			mapExcludeNa.insert(std::pair<std::string, std::string>(item.first, item.second));
		}
	}

	int nI = 1;
    std::string strLuaScript;
    if(!mapExcludeNa.empty())
	{	
		strLuaScript += boost::str(boost::format("redis.call('hmset', KEYS[%1%]") % nI++);
    	for(const auto& item : mapExcludeNa)
    	{
        	strLuaScript += boost::str(boost::format(", '%1%', '%2%'") % item.first % item.second);
    	}
    	strLuaScript += ") ";
	}
	if(!mapNa.empty())
	{
		strLuaScript += boost::str(boost::format("redis.call('hdel', KEYS[%1%]") % nI++);
    	for(const auto& item : mapNa)
    	{
        	strLuaScript += boost::str(boost::format(", '%1%'") % item.first);
    	}
    	strLuaScript += ") ";

	}

    if(!vecDelIndex.empty())
    {
        for(const auto &item : vecDelIndex)
        {
            strLuaScript += boost::str(boost::format(" redis.call('srem' ,KEYS[%1%] ,'%2%')") % nI++ % strItem);	
        }
    }

    for(const auto &item : vecAddIndex)
    {
        strLuaScript += boost::str(boost::format(" redis.call('sadd', KEYS[%1%], '%2%') ") % nI++ % strItem );
    }

    TESLOG(INFO, "update memory: %s\n", strLuaScript.c_str());

    const sds sdsLuaScript = sdsnew(strLuaScript.c_str());
    std::vector<std::string> vecCammond;
    vecCammond.push_back("EVAL");
    vecCammond.push_back(sdsLuaScript);
    sdsfree(sdsLuaScript);
    vecCammond.push_back(std::to_string(nI-1));
	if(!mapExcludeNa.empty())
	{
    	vecCammond.push_back(strKey);
	}
	if(!mapNa.empty())
	{
		vecCammond.push_back(strKey);
	}
    vecCammond.insert(vecCammond.end(), vecDelIndex.begin(), vecDelIndex.end());
    vecCammond.insert(vecCammond.end(), vecAddIndex.begin(), vecAddIndex.end());
    
    char **pcArgv = convertToSds(vecCammond);
    size_t stArgc = vecCammond.size();
    std::unique_ptr<size_t[]> pArgvlen(new size_t[stArgc]);
    for (int nJ = 0; nJ < stArgc; nJ++)
    {
        pArgvlen[nJ] = sdslen(pcArgv[nJ]);
    }

    redisReply *pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());
    if(pReply == NULL)
    {
        TESLOG(INFO, "pReply == NULL, No Reply ,cc->type[%d], cc->errstr[%s], retry:\n", 
                     _context->err, _context->errstr);
        if (REDIS_ERR_EOF == _context->err){
            pReply = (redisReply *)redisClusterCommandArgv(_context, stArgc, (const char **)pcArgv, pArgvlen.get());
            if (pReply == NULL ){
                sdsfreesplitres(pcArgv, stArgc);
                TESLOG(ERROR, "pReply == NULL, No Reply ,cc->type[%d], cc->errstr[%s]\n", 
                                _context->err, _context->errstr);
                return false;
            }
        }else{
            sdsfreesplitres(pcArgv, stArgc);	
            return false;
        }
    }
    sdsfreesplitres(pcArgv, stArgc);	
    freeReplyObject(pReply);
    return true;
}

bool updateMemory(const std::string &strRobot, const std::map<std::string, std::string> &mapCondition,
    const std::map<std::string, std::string> &mapContent, redisClusterContext* _context)
{
    
    TESLOG(INFO, "updateMemory, strRobot = %s\n", strRobot.c_str());
    for(const auto &item : mapCondition)
    {
        TESLOG(INFO, "mapCondition, key:%s ,value:%s\n", item.first.c_str(), item.second.c_str());
    }
    for(const auto &item : mapContent)
    {
        TESLOG(INFO, "mapContent, key:%s ,value:%s\n", item.first.c_str(), item.second.c_str());
    }

    std::set<std::string> setTemp;
    findIntersection(strRobot, mapCondition, setTemp, _context);
    if(setTemp.empty())
    {
        TESLOG(INFO, "query condition is inexistence\n");
        return false;
    }
    
    std::string strKey;
    std::map<std::string, std::string> mapHashMemory;
    std::vector<std::string> vecDelIndex;
    std::vector<std::string> vecAddIndex;
    for(const auto& item : setTemp)
    {
        vecDelIndex.clear();
        vecAddIndex.clear();
        strKey = g_kstrPrimaryTableHeader + item;
        mapHashMemory.clear();
        if(hgetall(strKey.c_str(), mapHashMemory, _context))
        {
            for(const auto &ite : mapContent)
            {
                auto iter = mapHashMemory.find(ite.first);
                if(iter != mapHashMemory.end())
                {
                    if(ite.second != iter->second)
                    {
                        std::string strDel = g_kstrIndexTableHeader + "{" + strRobot + "}_" + iter->first + "_" + iter->second;
                        vecDelIndex.push_back(strDel);
                    }
                    else
                    {
                        continue;
                    }
                }
                std::string strAdd = g_kstrIndexTableHeader + "{" + strRobot + "}_" + ite.first + "_" + ite.second;
                vecAddIndex.push_back(strAdd);
            }
        }
        else
        {
            TESLOG(ERROR, "hgetall %s failed\n", strKey.c_str());
            return false;
        }
        if(!runLuaScript(strKey, mapContent, item, vecDelIndex, vecAddIndex, _context))
        {
            return false;
        }
    }
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
    sleep(120);
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


std::map<std::string, std::string> makeRDocument(std::map<std::string, std::string> &memory, int type, std::string robotid){
    std::map<std::string, std::string> out;
    for (const auto &item: memory){
        if(type == 1 && item.first != "主语"){
            out.emplace(std::make_pair(item.first, item.second + "--update"));
            continue;
        }else {
            out.emplace(std::make_pair(item.first, item.second));
        }
    }
    out.emplace(std::make_pair("主语", robotid));
    return out;
}

std::multimap<std::string, std::pair<std::string, std::string>> makeDelDocument(std::map<std::string, std::string> &memory, int type, std::string robotid){
    std::multimap<std::string, std::pair<std::string, std::string>> mapCondition;
    for (const auto &item: memory){
        if(type == 1 && item.first != "主语"){
            mapCondition.emplace(std::make_pair("=",std::make_pair(item.first, item.second + "--update")));
            continue;
        }else {
            mapCondition.emplace(std::make_pair("=",std::make_pair(item.first, item.second)));
        }
    }
    mapCondition.emplace(std::make_pair("=",std::make_pair("主语", robotid)));
    return mapCondition;
}

redisClusterContext* getContext(){
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
    return _context;
}

void add(int i,const char *file, redisClusterContext* _context, int types){
    char line[MAXLINE] = {0};
    int nowline = 0;
    std::vector<int> errline;
    std::map<std::string, std::string> mapMemory;
    FILE *fp = fopen(file,"rb");
    std::string robotid = "00000000000000000RI0000" + std::to_string(i*10+i);
    while(fp != nullptr && fgets(line, MAXLINE, fp)){
        nowline++;
        if(parseLine(line, mapMemory, SEPOR1, SEPOR2) == false){
            errline.push_back(nowline);
            continue;
        }else{
            TIMEBEGIN(0);
            addMemoryAchieve(robotid,  makeRDocument(mapMemory, types, robotid), _context);
            TIMEEND(0);
            mapMemory.clear();
        }
    }
    for(auto i: errline){
        TESLOG(INFO, "出错行:%d", i); 
    }
    TESLOG(INFO, "\n"); 
    fclose(fp);
}

void find(int i,const char *file, redisClusterContext* _context, int types){
    char line[MAXLINE] = {0};
    int nowline = 0;
    std::vector<int> errline;
    std::map<std::string, std::string> mapMemory;
    FILE *fp = fopen(file,"rb");
    std::string robotid = "00000000000000000RI0000" + std::to_string(i*10+i);
    std::vector<int> setTemp;
    while(fp != nullptr && fgets(line, MAXLINE, fp)){
        nowline++;
        if(parseLine(line, mapMemory, SEPOR1, SEPOR2) == false){
            errline.push_back(nowline);
            continue;
        }else{
            TIMEBEGIN(0);
            getMemory(robotid, makeRDocument(mapMemory, types, robotid), false, setTemp, _context);
            std::string key = "mp_{" + robotid + "}_";
            std::map<std::string, std::string> mapOut;
            for (auto &iter: setTemp){
                TESLOG(INFO, "%d\n", iter);
                key += std::to_string(iter);
                if(hgetall(key.c_str(), mapOut, _context))
                {
                    for(const auto &item : mapOut)
                    {
                        TESLOG(INFO, "%s %s\n", item.first.c_str(), item.second.c_str());
                    }
                }
                key = "mp_{"+robotid+"}_";
            }
            TIMEEND(0);
            mapMemory.clear();
        }
    }
    for(auto i: errline){
        TESLOG(INFO, "出错行:%d", i); 
    }
    TESLOG(INFO, "\n"); 
    fclose(fp);
}

void update(int i,const char *file, redisClusterContext* _context, int types){
    std::vector<int> errline;
    char line[MAXLINE] = {0};
    int nowline = 0;
    std::map<std::string, std::string> mapMemory;
    FILE *fp = fopen(file,"rb");
    std::string robotid = "00000000000000000RI0000" + std::to_string(i*10+i);
    while(fp != nullptr && fgets(line, MAXLINE, fp)){
        nowline++;
        if(parseLine(line, mapMemory, SEPOR1, SEPOR2) == false){
            errline.push_back(nowline);
            continue;
        }else{
            TIMEBEGIN(0);
            updateMemory(robotid,  makeRDocument(mapMemory, types, robotid), 
                        makeRDocument(mapMemory, 1, robotid), _context);
            TESLOG(INFO, "\n");
            TIMEEND(0);
            mapMemory.clear();
        }
    }
    for(auto i: errline){
        TESLOG(INFO, "出错行:%d", i); 
    }
    TESLOG(INFO, "\n"); 
    fclose(fp);
}

void delet(int i,const char *file, redisClusterContext* _context, int types){
    std::vector<int> errline;
    char line[MAXLINE] = {0};
    int nowline = 0;
    std::map<std::string, std::string> mapMemory;
    FILE *fp = fopen(file,"rb");
    std::string robotid = "00000000000000000RI0000" + std::to_string(i*10+i);
    while(fp != nullptr && fgets(line, MAXLINE, fp)){
        nowline++;
        if(parseLine(line, mapMemory, SEPOR1, SEPOR2) == false){
            errline.push_back(nowline);
            continue;
        }else{
            TIMEBEGIN(0);
            delMemoryAchieve(robotid,  makeDelDocument(mapMemory, types, robotid), true, _context);
            TIMEEND(0);
            mapMemory.clear();
        }
    }
    for(auto i: errline){
        TESLOG(INFO, "出错行:%d", i); 
    }
    TESLOG(INFO, "\n"); 
    fclose(fp);
}

void redis_test(){
    int preManu = -1;
    int count = 1000;
    char *file = new char(1024);
    char line[MAXLINE] = {0};
    redisClusterContext* 	_context = getContext();
    std::string robotid = "1812111752213087b6fe75a7RI003706";
    std::map<std::string, std::string> mapMemory;
    int manu = 0;
    int types = 0;
    int typel = 0;
    int nowline = 0;
    std::vector<int> errline;
    FILE *fp = nullptr;
    while ((manu = getChar(&preManu, file)) >= 0){
        switch(manu){
            case 0:{
                TESLOG(INFO, "\n");
                count = getChar(&preManu, file);
                if(count == -2){
                    add( 0, file,  _context, types);
                }else {
                    for (int i = 0; i < count; i++){
                        
                        mapMemory.insert(make_pair("习惯行为","下班"));
                        mapMemory.insert(make_pair("主语",robotid));
                        mapMemory.insert(make_pair("日期","100"));
                        mapMemory.insert(make_pair("分钟",std::to_string(i)));
                        TIMEBEGIN(0);
                        addMemoryAchieve(robotid,  makeRDocument(mapMemory, types, robotid), _context);
                        TIMEEND(0);                        
                        mapMemory.clear();
                    }
                    TESLOG(INFO, "\n");
                }
                types = 0;
                break;
            }
            case 1:{
                TESLOG(INFO, "\n");
                std::vector<int> setTemp;
                count = getChar(&preManu, file);
                if(count == -2){
                    find( 0, file,  _context, types);
                }else {
                    for (int i = 0; i < count; i++){
                        TESLOG(INFO, "\n");
                        mapMemory.insert(make_pair("习惯行为","下班"));
                        mapMemory.insert(make_pair("主语",robotid));
                        mapMemory.insert(make_pair("日期","100"));
                        mapMemory.insert(make_pair("分钟",std::to_string(i)));
                        TIMEBEGIN(0);
                        getMemory(robotid, makeRDocument(mapMemory, types, robotid), false, setTemp, _context);
                        std::string key = "mp_{"+robotid+"}_";
                        std::map<std::string, std::string> mapOut;
                        for (auto &iter: setTemp){
                            TESLOG(INFO, "%d\n", iter);
                            key += std::to_string(iter);
                            if(hgetall(key.c_str(), mapOut, _context))
                            {
                                for(const auto &item : mapOut)
                                {
                                    TESLOG(INFO, "%s %s\n", item.first.c_str(), item.second.c_str());
                                }
                            }
                            key = "mp_{"+robotid+"}_";
                        }
                        TESLOG(INFO, "\n");
                        TIMEEND(0);
                        setTemp.clear();
                        mapMemory.clear();
                    }
                }
                break;
            }
            case 2:{
                count = getChar(&preManu, file);
                if(count == -2){
                    update(0, file, _context, types);
                    
                }else {
                    std::map<std::string, std::string> mapCondition;
                    for (int i = 0; i < count; i++){
                        TESLOG(INFO, "\n");
                        mapCondition.emplace(std::make_pair("习惯行为","下班"));
                        mapCondition.emplace(std::make_pair("主语",robotid));
                        mapCondition.emplace(std::make_pair("日期","100"));
                        mapCondition.emplace(std::make_pair("分钟",std::to_string(i)));
                        TIMEBEGIN(0);
                        updateMemory(robotid,  makeRDocument(mapCondition, types, robotid), 
                                        makeRDocument(mapCondition, 1, robotid), _context);
                        TESLOG(INFO, "\n");
                        TIMEEND(0);
                        mapCondition.clear();

                    }
                }
                types = 1;
                break;
            }
            case 3:{
                count = getChar(&preManu, file);
                if(count == -2){
                    delet( 0, file, _context, types);
                    
                }else {
                    
                    std::map<std::string, std::string> mapCondition;
                    for (int i = 0; i < count; i++){
                        TESLOG(INFO, "\n");
                        
                        mapCondition.emplace(std::make_pair("习惯行为","下班"));
                        mapCondition.emplace(std::make_pair("主语",robotid));
                        mapCondition.emplace(std::make_pair("日期","100"));
                        mapCondition.emplace(std::make_pair("分钟",std::to_string(i)));
                        TIMEBEGIN(0);
                        delMemoryAchieve(robotid,  makeDelDocument(mapCondition, types, robotid), true, _context);
                        TESLOG(INFO, "\n");
                        TIMEEND(0);
                        mapCondition.clear();

                    }
                }
                types = 0;
                break;
            }
            
            case 4:{
                TESLOG(INFO, "\n");
                
                mapMemory.clear();
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语",robotid));
                mapMemory.insert(make_pair("日期","100"));
                mapMemory.insert(make_pair("分钟","1001"));
                TIMEBEGIN(0);
                addMemoryAchieve(robotid,  makeRDocument(mapMemory, typel, robotid), _context);
                TESLOG(INFO, "\n");
                TIMEEND(0);
                mapMemory.clear();
                
                typel = 0;
                break;
            }
            case 5:{
                std::vector<int> setTemp;
                TESLOG(INFO, "\n");
                
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语",robotid));
                mapMemory.insert(make_pair("日期","100"));
                mapMemory.insert(make_pair("分钟","1001"));
                TIMEBEGIN(0);
                getMemory(robotid, makeRDocument(mapMemory, typel, robotid), false, setTemp, _context);
                std::string key = "mp_{"+robotid+"}_";
                std::map<std::string, std::string> mapOut;
                for (auto &iter: setTemp){
                    TESLOG(INFO, "%d\n", iter);
                    key += std::to_string(iter);
                    if(hgetall(key.c_str(), mapOut, _context))
                    {
                        for(const auto &item : mapOut)
                        {
                            TESLOG(INFO, "%s %s\n", item.first.c_str(), item.second.c_str());
                        }
                    }
                    key = "mp_{"+robotid+"}_";
                }
                TESLOG(INFO, "\n");
                TIMEEND(0);
                mapMemory.clear();
                break;
            }
            case 6:{
                TESLOG(INFO, "\n");
                TIMEBEGIN(0);
                mapMemory.emplace(std::make_pair("习惯行为","下班"));
                mapMemory.emplace(std::make_pair("主语",robotid));
                mapMemory.emplace(std::make_pair("日期","100"));
                mapMemory.emplace(std::make_pair("分钟","1001"));
                updateMemory(robotid,   makeRDocument(mapMemory, typel, robotid), makeRDocument(mapMemory, 1, robotid), _context);
                
                TESLOG(INFO, "\n");
                TIMEEND(0);
                mapMemory.clear();
                typel = 1;
                break;
            }
            case 7:{
                TESLOG(INFO, "\n");
                TIMEBEGIN(0);
                std::map<std::string, std::string> mapCondition;
                mapCondition.emplace(std::make_pair("习惯行为","下班"));
                mapCondition.emplace(std::make_pair("主语",robotid));
                mapCondition.emplace(std::make_pair("日期","100"));
                mapCondition.emplace(std::make_pair("分钟","1001"));
                delMemoryAchieve(robotid,  makeDelDocument(mapCondition, typel, robotid), true, _context);
                
                TESLOG(INFO, "\n");
                TIMEEND(0);
                mapCondition.clear();
                typel = 0;
                break;
            }
            case 9:{ // 退出
                return;
            }
        }
    
    }


    

#if 0
    std::multimap<std::string, std::pair<std::string, std::string>> mapCondition;
    mapCondition.emplace(std::make_pair("=",std::make_pair("姓名","江冠斌")));
    mapCondition.emplace(std::make_pair("=",std::make_pair("年龄","23")));
    delMemoryAchieve("000000000000000000000000RI000000", mapCondition, true, _context);
#endif
#if 0
    thread_run(1);

    const char *keys[2];
    std::string key1 = "1.00002_{1901241905133087b6fe75a7RI000002}_rrrrrrr21";
    keys[0] = key1.c_str();
    std::string key2 = "1.00002_{1901241905133087b6fe75a7RI000002}_rrrrrrr19";
    keys[1] = key2.c_str();
    std::string out = "";
    zinterstore("1901241905133087b6fe75a7RI000002", keys, 2, out, _context);
    TESLOG(ERROR, "{1901241905133087b6fe75a7RI000002}:out:test  : %s\n", out.c_str());
    if(_context)
    {
        TESLOG(ERROR, "Disconnect From Redis Server[%s]\n", _addr.c_str());
        redisClusterFree(_context);
    }
    #endif
}

void alltest(int tid,const char *file, int choice){
    redisClusterContext* 	_context = getContext();
    
    switch(choice){
        case 0:
            TESLOG(INFO, "+add+++++++++++++++++++++++++++++++++++!\n");
            add( tid, file, _context, 0);
            break;
        case 1:
            TESLOG(INFO, "+find+++++++++++++++++++++++++++++++++++!\n");
            find( tid, file, _context, 0);
            break;
        case 2:
            TESLOG(INFO, "+update+++++++++++++++++++++++++++++++++++!\n");
            update( tid, file, _context, 0);
            break;
        case 3:
            TESLOG(INFO, "+delet+++++++++++++++++++++++++++++++++++!\n");
            delet( tid, file, _context, 1);
            break;
    }

}

bool redis_thread(int num,const char *file, int choice){

    std::thread *p[num];
    for(int i = 0; i < num; i++){
        p[i] = new std::thread(alltest, i, file, choice);
        if (p[i] == nullptr) {
            TESLOG(INFO, "create pthread error!\n");
            return -1;
        }
        
    }
    for(int i = 0; i < num; i++){
        p[i]->join();
    }
}

void redis_all_slots(){
    redisClusterContext* 	_context = getContext();
    int nodes = 0;
    do{
        TESLOG(INFO, "cluster end: %d\n", nodes);
        nodes = ((cluster_slot *)(_context->table[nodes]->slots->head->value))->end + 1;
    }while(nodes != REDIS_CLUSTER_SLOTS);
}

bool redis_base_test(){
    std::string out = "";
    redisClusterContext* 	_context = getContext();
    clusterSlots(out, _context);

}

#endif