#ifndef __TEST_REDIS_H
#define __TEST_REDIS_H

#include "common.hpp"
#include "redisJgb.hpp"
using namespace std;

void redisTest000(){
    redisClusterContext* 	_context = getContext();
    //redis.call('del', KEYS[1], KEYS[2])

    string strLuaScript = boost::str(boost::format("return redis.call('hmset', KEYS[1], '%1%', '%2%')") % "111" % "jgb \n jgb");
    const sds sdsLuaScript = sdsnew(strLuaScript.c_str());
    std::vector<std::string> vecCammond;
    vecCammond.push_back("EVAL");
    vecCammond.push_back(sdsLuaScript);
    sdsfree(sdsLuaScript);
    vecCammond.push_back("1");
    vecCammond.push_back("jgbtest");
    char **pcArgv = convertToSds(vecCammond);
    size_t stArgc = vecCammond.size();
    std::unique_ptr<size_t[]> pArgvlen(new size_t[stArgc]);
    for (size_t nJ = 0; nJ < stArgc; nJ++)
    {
        pArgvlen[nJ] = sdslen(pcArgv[nJ]);
        TESLOG(INFO, "－－－－－－－－－－%s\n", pcArgv[nJ]);
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
            TESLOG(INFO, "Reply[%u]\n", pReply->type);
            TESLOG(INFO, "Reply[%s]\n", pReply->str);
            break;
    }

    sdsfreesplitres(pcArgv, stArgc);	
    freeReplyObject(pReply);
}


#endif