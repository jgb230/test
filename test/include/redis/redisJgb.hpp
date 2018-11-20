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

char** convertToSds(const std::vector<std::string>& vecCammond)
{
    int nJ;
    char **pSds = (char**)malloc(sizeof(char*)*vecCammond.size());
    for(nJ = 0; nJ < vecCammond.size(); nJ++)
    {
        pSds[nJ] = sdsnew((vecCammond[nJ]).c_str());
    }
    return pSds;
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
    for (int nJ = 0; nJ < stArgc; nJ++)
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
    for (int nJ = 0; nJ < stArgc; nJ++)
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



void redis_test(){
    redisClusterContext* 	_context;
    std::string 			_addr = "172.16.0.21:7000";
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

    std::map<std::string, std::string> mapMemory;
// mi_{181026033809ffdc196c965aRI000004}_需提醒_是
// mi_{181026033809ffdc196c965aRI000004}_程度_剧烈
// mi_{181026033809ffdc196c965aRI000004}_类型_刺痛
// mi_{181026033809ffdc196c965aRI000004}_症状_咳嗽

    mapMemory.insert(make_pair("需提醒","是"));
    mapMemory.insert(make_pair("程度","剧烈"));
    //mapMemory.insert(make_pair("类型","刺痛"));

    std::set<std::string> setTemp;

    TESLOG(INFO, "fun\n");
    findIntersection("181026033809ffdc196c965aRI000004", mapMemory, setTemp, _context);
    for (auto &iter: setTemp){
        TESLOG(INFO, "%s\n", iter.c_str());
    }
    TESLOG(INFO, "fun1\n");
    findIntersection1("181026033809ffdc196c965aRI000004", mapMemory, setTemp, _context);
    for (auto &iter: setTemp){
        TESLOG(INFO, "%s\n", iter.c_str());
    }
    // TESLOG(INFO, "test\n");
    // test(_context);
    // TESLOG(INFO, "test1\n");
    // test1(_context);


    if(_context)
    {
        TESLOG(ERROR, "Disconnect From Redis Server[%s]\n", _addr.c_str());
        redisClusterFree(_context);
    }
}