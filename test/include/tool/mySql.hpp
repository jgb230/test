#ifndef _MYMYSQL_H_
#define _MYMYSQL_H_

#include "DBTimer.h"
#include "DBMySql.h"
using namespace mts_timer;

void testAdd(){
    // {
    // DBMySql *mysql = new DBMySql();
    // mysql->initialize("172.16.0.21", "root", "root", "tesdb001");
    // DBTimer *timer = new DBTimer(mysql);
    // TaskInfo taskInfo;
    // taskInfo.m_nUid = 999009;
    // taskInfo.m_strAppid = "999009";
    // taskInfo.m_nExpTime = 1500202222;

    // TaskTime tt = {2019, 8, 19, 0, 15, 39, 0, ""};
    // taskInfo.m_taskTime = tt;

    // TaskData td;
    // td.m_nActive = 0;
    // td.m_strClass = "";
    // td.m_strMsgType = "M";
    // td.m_strContents = "测试数据";
    // taskInfo.m_taskData = td;

    // taskInfo.m_strTimerId = "201908191544001";

    // timer->storeTaskInfo(taskInfo);
    // std::cout << std::endl;
    // }

    // {
    // DBMySql *mysql = new DBMySql();
    // mysql->initialize("172.16.0.21", "root", "root", "tesdb001");
    // DBTimer *timer = new DBTimer(mysql);
    // TaskInfo taskInfo;
    // taskInfo.m_nUid = 999009;
    // taskInfo.m_strAppid = "999009";
    // taskInfo.m_nExpTime = 1500202222;

    // TaskTime tt = {2019, 8, 0, 2, 15, 0, 0, ""};
    // taskInfo.m_taskTime = tt;

    // TaskData td;
    // td.m_nActive = 0;
    // td.m_strClass = "";
    // td.m_strMsgType = "M";
    // td.m_strContents = "测试数据";
    // taskInfo.m_taskData = td;

    // taskInfo.m_strTimerId = "201908191544002";

    // timer->storeTaskInfo(taskInfo);
    // std::cout << std::endl;
    // }
    {
    DBMySql *mysql = new DBMySql();
    mysql->initialize("172.16.0.21", "root", "root", "tesdb001");
    DBTimer *timer = new DBTimer(mysql);
    TaskInfo taskInfo;
    for (int i = 999010; i < 999050; ++i){
        taskInfo.m_nUid = i;
        taskInfo.m_strAppid = std::to_string(i);
        taskInfo.m_nExpTime = 1500202222 + i;

        TaskTime tt = {2019, 8, 1, 0, 15, 39, 0, ""};
        taskInfo.m_taskTime = tt;

        TaskData td;
        td.m_nActive = 0;
        td.m_strClass = "";
        td.m_strMsgType = "L";
        td.m_strContents = "[{\"key\":\"key1\",\"value\":\"value1\"}, {\"key\":\"key2\",\"value\":\"value2\"}, {\"key\":\"key3\",\"value\":\"value3\"}]";
        taskInfo.m_taskData = td;

        taskInfo.m_strTimerId = "201908191" + std::to_string(i);

        timer->storeTaskInfo(taskInfo);
        std::cout << std::endl;
    }
    
    }

    // {
    // DBMySql *mysql = new DBMySql();
    // mysql->initialize("172.16.0.21", "root", "root", "tesdb001");
    // DBTimer *timer = new DBTimer(mysql);
    // TaskInfo taskInfo;
    // taskInfo.m_nUid = 999010;
    // taskInfo.m_strAppid = std::to_string(999010);
    // taskInfo.m_nExpTime = 1500202222 + 999010;

    // TaskTime tt = {2019, 8, 1, 0, 15, 39, 0, ""};
    // taskInfo.m_taskTime = tt;

    // TaskData td;
    // td.m_nActive = 0;
    // td.m_strClass = "";
    // td.m_strMsgType = "L";
    // td.m_strContents = "[{\"key\":\"key1\",\"value\":\"value1\"}, {\"key\":\"key2\",\"value\":\"value2\"}, {\"key\":\"key3\",\"value\":\"value3\"}]";

    // taskInfo.m_taskData = td;

    // taskInfo.m_strTimerId = "201908191" + std::to_string(999010);

    // timer->storeTaskInfo(taskInfo);
    // std::cout << std::endl;
    
    // }
}

void testUpdate(){
    {
    DBMySql *mysql = new DBMySql();
    mysql->initialize("172.16.0.21", "root", "root", "tesdb001");
    DBTimer *timer = new DBTimer(mysql);

    TaskInfo taskInfo;
    TaskTime tt = {NOTIMEVALUE, NOTIMEVALUE, 19, 0, 15, 39, NOTIMEVALUE, ""};
    taskInfo.m_taskTime = tt;
    TaskData td;
    td.m_nActive = NOACTIVE;
    td.m_strClass = "";
    td.m_strMsgType = "";
    td.m_strContents = "";
    taskInfo.m_taskData = td;

    Condition cond;
    cond.m_nUid = 999009;
    cond.m_strAppid = "999009";
    TaskTime tt1 = {2019, 8, 0, 2, 15, 0, 0, ""};
    cond.m_taskTime = tt1;
    TaskData td1;
    td1.m_nActive = 0;
    td1.m_strClass = "";
    td1.m_strMsgType = "M";
    td1.m_strContents = "测试数据";
    cond.m_taskData = td1;

    std::vector<Condition> vecConds;
    vecConds.push_back(cond);

    std::vector<TaskInfo> tis;
    timer->updateTaskInfoByCdt(vecConds, taskInfo, tis);
    for (auto ti: tis){
        std::cout << " m_strTimerId:" << ti.m_strTimerId << std::endl; 
    }
    std::cout << std::endl;
    }
    {
    DBMySql *mysql = new DBMySql();
    mysql->initialize("172.16.0.21", "root", "root", "tesdb001");
    DBTimer *timer = new DBTimer(mysql);

    TaskInfo taskInfo;
    TaskTime tt = {NOTIMEVALUE, NOTIMEVALUE, NOTIMEVALUE, 0, 16, 39, NOTIMEVALUE, ""};
    taskInfo.m_taskTime = tt;
    TaskData td;
    td.m_nActive = NOACTIVE;
    td.m_strClass = "";
    td.m_strMsgType = "";
    td.m_strContents = "";
    taskInfo.m_taskData = td;

    Condition cond;
    cond.m_nUid = 999009;
    cond.m_strAppid = "999009";
    TaskTime tt1 = {2019, 8, 0, 2, 15, 0, 0, ""};
    cond.m_taskTime = tt1;
    TaskData td1;
    td1.m_nActive = 0;
    td1.m_strClass = "";
    td1.m_strMsgType = "M";
    td1.m_strContents = "测试数据";
    cond.m_taskData = td1;

    std::vector<Condition> vecConds;
    vecConds.push_back(cond);

    std::vector<TaskInfo> tis;
    timer->updateTaskInfoByCdt(vecConds, taskInfo, tis);
    for (auto ti: tis){
        std::cout << " ti:" << timer->TaskInfo2string(ti) << std::endl; 
    }
    std::cout << std::endl;
    }
}

void testdel(){
    {
    DBMySql *mysql = new DBMySql();
    mysql->initialize("172.16.0.21", "root", "root", "tesdb001");
    DBTimer *timer = new DBTimer(mysql);
    Condition cond;
    cond.m_nUid = NOUID;
    cond.m_strAppid = "";

    TaskTime tt = {NOTIMEVALUE, NOTIMEVALUE, NOTIMEVALUE, NOTIMEVALUE, 15, 39, 0, ""};
    cond.m_taskTime = tt;

    TaskData td;
    td.m_nActive = NOACTIVE;
    td.m_strClass = "";
    td.m_strMsgType = "";
    td.m_strContents = "";
    cond.m_taskData = td;
    std::vector<Condition> conds;
    conds.push_back(cond);
    timer->delTaskInfoByCdt(conds);
    std::cout << std::endl;
    
    }
}

void TestTimerDB(){
    testAdd();
    testdel();

}

#endif