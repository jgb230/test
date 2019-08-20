#include "DBTimer.h"
#include "rapidjson/rapidjson_helper.h"

using namespace mts_timer;

#define SELECT    " select "
#define DELETE    " delete "
#define UPDATE    " update "
#define INSERT    " insert "
#define FROM      " from "
#define WHERE     " where "
#define VALUES    " values "
#define SET       " set "
#define IN        " in "
#define AND       " and "
#define ORDER     " order by "
#define LIMIT     " limit "
#define TIMERINFO " tb_timer_info "

#define KEY       "key"
#define VALUE     "value"

static std::string FIELDS[_FIELD_MAX]  = {"tm_id", "tm_uid", "tm_appid", "tm_timerid", 
                                          "tm_year", "tm_month", "tm_day", "tm_weekly", "tm_hour", "tm_min", "tm_sec",
                                          "tm_class", "tm_type", "tm_content", "tm_active", "tm_expire_time", "tm_offset"};


int my_atoi(const char *str)
{
    if (str == NULL)
        return 0;
    else
        return atoi(str);
}

std::string my_char2string(const char *str)
{
    if (str == NULL)
        return "";
    else
        return std::string(str);
}

DBTimer::DBTimer(DBMySql *ptrMysql){
    m_pMySqlConn = ptrMysql;
}

Condition DBTimer::TaskInfo2Condition(const TaskInfo &taskInfo){
    Condition cond;
    cond.m_nUid = taskInfo.m_nUid;
    cond.m_strAppid = taskInfo.m_strAppid;
    cond.m_taskData = taskInfo.m_taskData;
    cond.m_taskTime = taskInfo.m_taskTime;
    return cond;
}

bool DBTimer::insterTaskInfo(const TaskInfo &taskInfo){
    std::string sql = INSERT + std::string(TIMERINFO);
    // 组装添加字段
    sql += "(" + FIELDS[_TM_UID];
    for (int i = 2; i < _FIELD_MAX; ++i){
        sql += "," + FIELDS[i];
    }
    sql += ")" ;

    sql += VALUES;

    // 组装添加内容
    sql += "(" + packageInt(taskInfo.m_nUid);
    sql += "," + packageString(taskInfo.m_strAppid);
    sql += "," + packageString(taskInfo.m_strTimerId);
    sql += "," + packageInt(taskInfo.m_taskTime.m_nYear);
    sql += "," + packageInt(taskInfo.m_taskTime.m_nMonth);
    sql += "," + packageInt(taskInfo.m_taskTime.m_nDay);
    sql += "," + packageInt(taskInfo.m_taskTime.m_nWeekly);
    sql += "," + packageInt(taskInfo.m_taskTime.m_nHour);
    sql += "," + packageInt(taskInfo.m_taskTime.m_nMin);
    sql += "," + packageInt(taskInfo.m_taskTime.m_nSec);
    sql += "," + packageString(taskInfo.m_taskData.m_strClass);
    sql += "," + packageString(taskInfo.m_taskData.m_strMsgType);
    sql += "," + packageString(taskInfo.m_taskData.m_strContents);
    sql += "," + packageInt(taskInfo.m_taskData.m_nActive);
    sql += ",FROM_UNIXTIME(" + packageTimet(taskInfo.m_nExpTime) + ")";
    sql += "," + packageString(taskInfo.m_taskTime.m_strOffsetTime);
    sql += ")";

    int ret = m_pMySqlConn->Insert(sql);
    if (ret == 0)
    {
        std::cout <<"添加成功!"<< std::endl;
        return true;
    }

    return false;
}

bool DBTimer::hasSameTaskInfo(const TaskInfo &taskInfo){
    Condition cond = TaskInfo2Condition(taskInfo);
    std::vector<TaskInfo> vecTaskInfos;
    //　完全匹配查找
    selectTaskInfo(cond, vecTaskInfos, true);
    if(vecTaskInfos.empty()){
        //根据条件没有查找到定时器任务，返回false
        return false; 
    }else{
        std::cout << "存在一模一样的数据!" << std::endl;
        return true;
    }
}

bool DBTimer::storeTaskInfo(const TaskInfo &taskInfo){
    bool ret = false;
    if(!hasSameTaskInfo(taskInfo)){
        ret = insterTaskInfo(taskInfo);
    }
    return ret;
}

bool DBTimer::delTaskInfoByCdt(const std::vector<Condition> &vecConds){
    std::vector<TaskInfo> vecTaskInfos;
    std::vector<std::string> vecTimerId;
    // content部分匹配查找
    selectTIByCdts(vecConds, vecTaskInfos, false);
    if(vecTaskInfos.empty()){
       return false; 
    }
    for(auto taskInfo: vecTaskInfos){
        // 添加符合条件的任务　timerId　到待删除集合中
        vecTimerId.push_back(taskInfo.m_strTimerId);
    }
    //　根据timerId删除任务
    delTaskInfoByTimerId(vecTimerId);
    return true;
}

bool DBTimer::delTaskInfoByTimerId(const std::vector<std::string> &vecTimerId){
    //　根据timerId删除任务
    if(vecTimerId.size() == 0){
        std::cout << "vecTimerId 是空的" << std::endl;
        return false;
    }
    // 组装ｓｑｌ
    std::string sql = DELETE + std::string(FROM) + TIMERINFO + std::string(WHERE);
    sql += FIELDS[_TM_TIMERID] + IN + std::string("(") + packageString(vecTimerId[0]);
    for (unsigned int i = 1; i < vecTimerId.size(); ++i){
        sql += "," + packageString(vecTimerId[i]);
    }    
    sql += ")";
    // 执行
    int ret = m_pMySqlConn->execute(sql);
    if (ret == 0)
    {
        std::cout <<"删除成功!"<< std::endl;
        return true;
    }

    return false;
}

void DBTimer::alterTaskInfo(const TaskInfo &srcTaskInfo, TaskInfo &desTaskInfo){
    if(srcTaskInfo.m_taskTime.m_nYear != NOTIMEVALUE){
        desTaskInfo.m_taskTime.m_nYear = srcTaskInfo.m_taskTime.m_nYear;
    }
    if(srcTaskInfo.m_taskTime.m_nMonth != NOTIMEVALUE){
        desTaskInfo.m_taskTime.m_nMonth = srcTaskInfo.m_taskTime.m_nMonth;
    }
    if(srcTaskInfo.m_taskTime.m_nDay != NOTIMEVALUE){
        desTaskInfo.m_taskTime.m_nDay = srcTaskInfo.m_taskTime.m_nDay;
    }
    if(srcTaskInfo.m_taskTime.m_nWeekly != NOTIMEVALUE){
        desTaskInfo.m_taskTime.m_nWeekly = srcTaskInfo.m_taskTime.m_nWeekly;
    }
    if(srcTaskInfo.m_taskTime.m_nHour != NOTIMEVALUE){
        desTaskInfo.m_taskTime.m_nHour = srcTaskInfo.m_taskTime.m_nHour;
    }
    if(srcTaskInfo.m_taskTime.m_nMin != NOTIMEVALUE){
        desTaskInfo.m_taskTime.m_nMin = srcTaskInfo.m_taskTime.m_nMin;
    }
    if(srcTaskInfo.m_taskTime.m_nSec != NOTIMEVALUE){
        desTaskInfo.m_taskTime.m_nSec = srcTaskInfo.m_taskTime.m_nSec;
    }
    if(!srcTaskInfo.m_taskData.m_strClass.empty()){
        desTaskInfo.m_taskData.m_strClass = srcTaskInfo.m_taskData.m_strClass;
    }
    if(!srcTaskInfo.m_taskData.m_strMsgType.empty()){
        desTaskInfo.m_taskData.m_strMsgType = srcTaskInfo.m_taskData.m_strMsgType;
    }
    if(srcTaskInfo.m_taskData.m_nActive != NOACTIVE){
        desTaskInfo.m_taskData.m_nActive = srcTaskInfo.m_taskData.m_nActive;
    }
    if(!srcTaskInfo.m_taskData.m_strContents.empty()){
        desTaskInfo.m_taskData.m_strContents = srcTaskInfo.m_taskData.m_strContents;
    }
}

bool DBTimer::hasTimeChange(const TaskInfo &taskInfo){
    bool ret = false;
    // 任何一个时间改变返回 true
    if( taskInfo.m_taskTime.m_nYear != NOTIMEVALUE ||
        taskInfo.m_taskTime.m_nMonth != NOTIMEVALUE ||
        taskInfo.m_taskTime.m_nDay != NOTIMEVALUE ||
        taskInfo.m_taskTime.m_nWeekly != NOTIMEVALUE ||
        taskInfo.m_taskTime.m_nHour != NOTIMEVALUE ||
        taskInfo.m_taskTime.m_nMin != NOTIMEVALUE ||
        taskInfo.m_taskTime.m_nSec != NOTIMEVALUE
    ) ret = true;
    return ret;
}

std::string DBTimer::packageSet(const TaskInfo &taskInfo){
    std::string set = SET;
    if(taskInfo.m_taskTime.m_nYear != NOTIMEVALUE){
        set +=  FIELDS[_TM_YEAR] + "=" + packageInt(taskInfo.m_taskTime.m_nYear) + " , ";
    }
    if(taskInfo.m_taskTime.m_nMonth != NOTIMEVALUE){
        set +=  FIELDS[_TM_MONTH] + "=" + packageInt(taskInfo.m_taskTime.m_nMonth) + " , ";
    }
    if(taskInfo.m_taskTime.m_nDay != NOTIMEVALUE){
        set +=  FIELDS[_TM_DAY] + "=" + packageInt(taskInfo.m_taskTime.m_nDay) + " , ";
    }
    if(taskInfo.m_taskTime.m_nWeekly != NOTIMEVALUE){
        set +=  FIELDS[_TM_WEEKLY] + "=" + packageInt(taskInfo.m_taskTime.m_nWeekly) + " , ";
    }
    if(taskInfo.m_taskTime.m_nHour != NOTIMEVALUE){
        set +=  FIELDS[_TM_HOUR] + "=" + packageInt(taskInfo.m_taskTime.m_nHour) + " , ";
    }
    if(taskInfo.m_taskTime.m_nMin != NOTIMEVALUE){
        set +=  FIELDS[_TM_MIN] + "=" + packageInt(taskInfo.m_taskTime.m_nMin) + " , ";
    }
    if(taskInfo.m_taskTime.m_nSec != NOTIMEVALUE){
        set +=  FIELDS[_TM_SEC] + "=" + packageInt(taskInfo.m_taskTime.m_nSec) + " , ";
    }
    if(!taskInfo.m_taskData.m_strClass.empty()){
        set +=  FIELDS[_TM_CLASS] + "=" + packageString(taskInfo.m_taskData.m_strClass) + " , ";
    }
    if(!taskInfo.m_taskData.m_strMsgType.empty()){
        set +=  FIELDS[_TM_TYPE] + "=" + packageString(taskInfo.m_taskData.m_strMsgType) + " , ";
    }
    if(taskInfo.m_taskData.m_nActive != NOACTIVE){
        set +=  FIELDS[_TM_ACTIVE] + "=" + packageInt(taskInfo.m_taskData.m_nActive) + " , ";
    }
    if(!taskInfo.m_taskData.m_strContents.empty()){
        set +=  FIELDS[_TM_CONTENT] + "=" + packageString(taskInfo.m_taskData.m_strContents) + " , ";
    }
    return set.substr(0, set.length() - 2);// 去掉最后一个,逗号
}

bool DBTimer::updateTaskInfoByCdt(const std::vector<Condition> &vecConds, const TaskInfo &taskInfo
                                    , std::vector<TaskInfo> &vecTaskInfos){
    std::vector<TaskInfo> vecInfo;
    std::vector<TaskInfo> vecInfotmp;
    std::vector<std::string> vecTimerId; // 需要更新的timerId
    selectTIByCdts(vecConds, vecInfo, false); // 找出所有符合条件的定时器任务
    for(auto info: vecInfo){
        alterTaskInfo(taskInfo, info);
        std::cout << " taskInfo:" << TaskInfo2string(taskInfo) << std::endl;
        std::cout << " taskInfo:" << TaskInfo2string(info) << std::endl;
        selectTaskInfo(TaskInfo2Condition(info), vecInfotmp, true);
        if(vecInfotmp.empty()){
            std::cout << " m_strTimerId:" << info.m_strTimerId << std::endl;
            vecTimerId.push_back(info.m_strTimerId);
            vecTaskInfos.push_back(info);
        }else{
            std::cout << "更新后存在一模一样的数据!" << std::endl;
        }
    }
    if(!hasTimeChange(taskInfo)){
        // 没有改变时间，不返回任务
        vecTaskInfos.clear();
    }

    //　是否有需要更新的任务
    if(vecTimerId.size() == 0){
        std::cout << "没有需要更新的任务!" << std::endl;
        return false;
    }
    // 组装ｓｑｌ
    std::string sql = UPDATE +  std::string(TIMERINFO);
    sql += packageSet(taskInfo);
    sql += WHERE;
    sql += FIELDS[_TM_TIMERID] + IN + std::string("(") + vecTimerId[0];
    for (unsigned int i = 1; i < vecTimerId.size(); ++i){
        sql += "," + vecTimerId[i];
    }    
    sql += ")";
    // 执行
    int ret = m_pMySqlConn->Update(sql);
    if (ret == 0)
    {
        std::cout <<"更新成功!"<< std::endl;
        return true;
    }

    return false;
}

bool DBTimer::updateExpTime(const std::string &strTimerId, time_t nExpTime){
    // 组装ｓｑｌ
    std::string sql = UPDATE +  std::string(TIMERINFO);
    sql += SET + FIELDS[_TM_EXPIRE_TIME] + "=FROM_UNIXTIME(" + packageTimet(nExpTime) + ")";
    sql += WHERE + FIELDS[_TM_TIMERID] + packageString(strTimerId);
    // 执行
    int ret = m_pMySqlConn->Update(sql);
    if (ret == 0)
    {
        std::cout <<"更新成功!"<< std::endl;
        return true;
    }

    return false;
}

std::string DBTimer::packageInt(const int nVal){
    std::string ret = std::to_string(nVal);
    return ret;
}

std::string DBTimer::packageTimet(const time_t nVal){
    std::string ret = std::to_string(nVal);
    return ret;
}

std::string DBTimer::packageString(const std::string strVal){
    std::string ret = "'" + strVal + "'";
    return ret;
}

std::string DBTimer::condition2sql(const Condition &cond){
    std::string where = " 1=1 ";
    if(cond.m_nUid != NOUID){
        where += AND + FIELDS[_TM_UID] + "=" + packageInt(cond.m_nUid);
    }
    if(!cond.m_strAppid.empty()){
        where += AND + FIELDS[_TM_APPID] + "=" + packageString(cond.m_strAppid);
    }
    if(cond.m_taskTime.m_nYear != NOTIMEVALUE){
        where += AND + FIELDS[_TM_YEAR] + "=" + packageInt(cond.m_taskTime.m_nYear);
    }
    if(cond.m_taskTime.m_nMonth != NOTIMEVALUE){
        where += AND + FIELDS[_TM_MONTH] + "=" + packageInt(cond.m_taskTime.m_nMonth);
    }
    if(cond.m_taskTime.m_nDay != NOTIMEVALUE){
        where += AND + FIELDS[_TM_DAY] + "=" + packageInt(cond.m_taskTime.m_nDay);
    }
    if(cond.m_taskTime.m_nWeekly != NOTIMEVALUE){
        where += AND + FIELDS[_TM_WEEKLY] + "=" + packageInt(cond.m_taskTime.m_nWeekly);
    }
    if(cond.m_taskTime.m_nHour != NOTIMEVALUE){
        where += AND + FIELDS[_TM_HOUR] + "=" + packageInt(cond.m_taskTime.m_nHour);
    }
    if(cond.m_taskTime.m_nMin != NOTIMEVALUE){
        where += AND + FIELDS[_TM_MIN] + "=" + packageInt(cond.m_taskTime.m_nMin);
    }
    if(cond.m_taskTime.m_nSec != NOTIMEVALUE){
        where += AND + FIELDS[_TM_SEC] + "=" + packageInt(cond.m_taskTime.m_nSec);
    }

    if(!cond.m_taskData.m_strClass.empty()){
        where += AND + FIELDS[_TM_CLASS] + "=" + packageString(cond.m_taskData.m_strClass);
    }
    if(!cond.m_taskData.m_strMsgType.empty()){
        where += AND + FIELDS[_TM_TYPE] + "=" + packageString(cond.m_taskData.m_strMsgType);
    }
    if(cond.m_taskData.m_nActive != NOACTIVE){
        where += AND + FIELDS[_TM_ACTIVE] + "=" + packageInt(cond.m_taskData.m_nActive);
    }

    return where;
}

TaskInfo DBTimer::row2TaskInfo(const MYSQL_ROW &row){
    
    TaskInfo taskInfo;
    memset(&taskInfo, 0, sizeof(taskInfo));
    taskInfo.m_nUid = my_atoi(row[_TM_UID]);
    taskInfo.m_strAppid = my_char2string(row[_TM_APPID]);
    taskInfo.m_strTimerId = my_char2string(row[_TM_TIMERID]);

    taskInfo.m_taskTime.m_nYear = my_atoi(row[_TM_YEAR]);
    taskInfo.m_taskTime.m_nMonth = my_atoi(row[_TM_MONTH]);
    taskInfo.m_taskTime.m_nDay = my_atoi(row[_TM_DAY]);
    taskInfo.m_taskTime.m_nWeekly = my_atoi(row[_TM_WEEKLY]);
    taskInfo.m_taskTime.m_nHour = my_atoi(row[_TM_HOUR]);
    taskInfo.m_taskTime.m_nMin = my_atoi(row[_TM_MIN]);
    taskInfo.m_taskTime.m_nSec = my_atoi(row[_TM_SEC]);
    taskInfo.m_taskTime.m_strOffsetTime = my_char2string(row[_TM_OFFSET]);

    taskInfo.m_taskData.m_strClass = my_char2string(row[_TM_CLASS]);
    taskInfo.m_taskData.m_strMsgType = my_char2string(row[_TM_TYPE]);
    taskInfo.m_taskData.m_strContents = my_char2string(row[_TM_CONTENT]);

    taskInfo.m_taskData.m_nActive = my_atoi(row[_TM_ACTIVE]);

    return taskInfo;
}

std::string DBTimer::map2string(const std::map<std::string, std::string> &mapTc){
    std::string ret = "[";
    for (auto tc: mapTc){
        ret += KEY + std::string(":") + tc.first + "," + std::string(VALUE) + ":" + tc.second + ";";
    }
    ret += "]";
    return ret;
}

std::string DBTimer::TaskInfo2string(const TaskInfo &task){
    std::string ret = "[";

    ret += std::to_string(task.m_nUid) + " ";
    ret += task.m_strAppid + " ";
    ret += task.m_strTimerId + " ";

    ret += std::to_string(task.m_taskTime.m_nYear) + " ";
    ret += std::to_string(task.m_taskTime.m_nMonth) + " ";
    ret += std::to_string(task.m_taskTime.m_nDay) + " ";
    ret += std::to_string(task.m_taskTime.m_nWeekly) + " ";
    ret += std::to_string(task.m_taskTime.m_nHour) + " ";
    ret += std::to_string(task.m_taskTime.m_nMin) + " ";
    ret += std::to_string(task.m_taskTime.m_nSec) + " ";

    ret += task.m_taskData.m_strClass + " ";
    ret += task.m_taskData.m_strContents + " ";
    ret += task.m_taskData.m_strMsgType + " ";

    ret += "]";
    return ret;
}

std::string DBTimer::Condition2string(const Condition &cond){
    std::string ret = "[";
    ret += std::to_string(cond.m_nUid) + " ";
    ret += cond.m_strAppid + " ";

    ret += std::to_string(cond.m_taskTime.m_nYear) + " ";
    ret += std::to_string(cond.m_taskTime.m_nMonth) + " ";
    ret += std::to_string(cond.m_taskTime.m_nDay) + " ";
    ret += std::to_string(cond.m_taskTime.m_nWeekly) + " ";
    ret += std::to_string(cond.m_taskTime.m_nHour) + " ";
    ret += std::to_string(cond.m_taskTime.m_nMin) + " ";
    ret += std::to_string(cond.m_taskTime.m_nSec) + " ";

    ret += cond.m_taskData.m_strClass + " ";
    ret += cond.m_taskData.m_strContents + " ";
    ret += cond.m_taskData.m_strMsgType + " ";
    ret += "]";
    return ret;
}

bool DBTimer::cmpContent(const std::string &strCC, const std::string &strTC, bool bPerfect, const std::string &type){
    std::cout << " TYPE:" << type << std::endl;
    if(strCC.empty()){
        return true;
    }
    if(type == "M"){
        // M 类型直接比较
        return strCC.compare(strTC) == 0? true: false; // 相等返回true,不等返回false
    }

    // L类型解析数组分别比较每个键值对
    json_mk_array(arrObjCC);
    json_parse(arrObjCC, strCC);
    json_mk_array(arrObjTC);
    json_parse(arrObjTC, strTC);
    json_arr_size(arrObjCC);
    std::map<std::string, std::string> mapTc;
    // 把arrObjTC放入map中
    for(unsigned int i = 0; i < json_arr_size(arrObjTC); ++i){
        auto &obj = json_arr_get_object(arrObjTC, i);
        mapTc.insert(std::make_pair(json_get_string(obj, KEY), json_get_string(obj, VALUE)));
    }
    std::cout << map2string(mapTc) << std::endl;
    bool ret = true;
    std::string key = "";
    std::string value = "";
    // 存在arrObjCC中可以不在map中或者value不同，则返回false;
    for(unsigned int i = 0; i < json_arr_size(arrObjCC); ++i){
        auto &obj = json_arr_get_object(arrObjCC, i);
        key = json_get_string(obj, KEY);
        value = json_get_string(obj, VALUE);
        auto iter = mapTc.find(key);
        if(iter == mapTc.end() || iter->second != value){
            ret = false;
            std::cout << KEY << ":" << key << " "<< VALUE <<":" << value  
                                         << ", mapTc:" << map2string(mapTc) << std::endl;
            break;
        }
    }
    if(ret && bPerfect){
        // 完全匹配
        ret =  json_arr_size(arrObjTC) == json_arr_size(arrObjCC) ? true: false;
    }
    return ret;
}

bool DBTimer::selectTaskInfoByCdt(const Condition &cond, std::vector<TaskInfo> &vecTaskInfos, int nCount, int nOffset){
    return selectTaskInfo(cond, vecTaskInfos, false, nCount, nOffset);
}


bool DBTimer::selectTaskInfo(const Condition &cond, std::vector<TaskInfo> &vecTaskInfos,
                             bool perfect, int nCount, int nOffset){
    std::string sql = "";
    TaskInfo info;
    vecTaskInfos.clear();
    // 组装每个condition　sql,根据uid排序
    std::cout << " cond:" << Condition2string(cond) << std::endl;
    sql = SELECT + FIELDS[0];
    for (int i = 1; i < _FIELD_MAX; ++i){
        if(i == _TM_EXPIRE_TIME){
            sql += ",UNIX_TIMESTAMP(" + FIELDS[i] + ") as " + FIELDS[i];
        }else{
            sql += "," + FIELDS[i];
        }
        
    }
    sql += FROM + std::string(TIMERINFO) + 
            WHERE + condition2sql(cond) + 
            ORDER + std::string(FIELDS[_TM_ID]);
    // 分页返回
    if(nCount != -1){
        sql += LIMIT + std::to_string(nOffset) + "," + std::to_string(nOffset);
    }
    // 执行查询
    MYSQL_RES* result = m_pMySqlConn->Query(sql);
    if (result == NULL)
    {
        std::cout <<"result == NULL"<< std::endl;
        return false;
    }
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        // 判断content内容是否匹配,不匹配则继续下一条记录，否则放入返回集中
        if(!cmpContent(cond.m_taskData.m_strContents, my_char2string(row[_TM_CONTENT]), 
                        perfect, my_char2string(row[_TM_TYPE]))){
            continue;
        }
        vecTaskInfos.push_back(row2TaskInfo(row));
    }
    mysql_free_result(result);
    
    return true;
}

bool DBTimer::selectTIByCdts(const std::vector<Condition> &vecConds, std::vector<TaskInfo> &vecTaskInfos,
                             bool perfect){
    std::string sql = "";
    TaskInfo info;
    vecTaskInfos.clear();
    for(auto cond: vecConds){
        // 组装每个condition　sql,根据uid排序
        std::cout << " cond:" << Condition2string(cond) << std::endl;
        sql = SELECT + FIELDS[0];
        for (int i = 1; i < _FIELD_MAX; ++i){
            if(i == _TM_EXPIRE_TIME){
                sql += ",UNIX_TIMESTAMP(" + FIELDS[i] + ") as " + FIELDS[i];
            }else{
                sql += "," + FIELDS[i];
            }
        }
        sql += FROM + std::string(TIMERINFO) + 
                WHERE + condition2sql(cond) + 
                ORDER + std::string(FIELDS[_TM_ID]);

        // 执行查询
        MYSQL_RES* result = m_pMySqlConn->Query(sql);
        if (result == NULL)
        {
            std::cout <<"result == NULL"<< std::endl;
            continue;
        }
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)))
        {
            // 判断content内容是否匹配,不匹配则继续下一条记录，否则放入返回集中
            std::cout << "_TM_CONTENT " << my_char2string(row[_TM_CONTENT]) << std::endl;
            if(!cmpContent(cond.m_taskData.m_strContents, my_char2string(row[_TM_CONTENT]), 
                            perfect, my_char2string(row[_TM_TYPE]))){
                continue;
            }
            vecTaskInfos.push_back(row2TaskInfo(row));
        }
        mysql_free_result(result);
    }
    
    
    return true;
}

bool DBTimer::selectTaskInfoByTime(time_t nBeginTime, time_t nEndTime, std::vector<TaskInfo> &vecTaskInfo){
    
    // 查询下一次执行时间查询在nBeginTime与nEndTime之间的所有任务
    std::string sql = "";
    TaskInfo info;
    // 组装sql 
    sql = SELECT + FIELDS[0];
    for (int i = 1; i < _FIELD_MAX; ++i){
        if(i == _TM_EXPIRE_TIME){
            sql += ",UNIX_TIMESTAMP(" + FIELDS[i] + ") as " + FIELDS[i];
        }else{
            sql += "," + FIELDS[i];
        }
    }
    sql += FROM + std::string(TIMERINFO) + 
            WHERE + FIELDS[_TM_EXPIRE_TIME] + ">=" + std::to_string(nBeginTime) +
            AND + FIELDS[_TM_EXPIRE_TIME] + "<" + std::to_string(nEndTime) +
            ORDER + std::string(FIELDS[_TM_ID]);
    // 执行查询
    MYSQL_RES* result = m_pMySqlConn->Query(sql);
    if (result == NULL)
    {
        std::cout <<"result == NULL"<< std::endl;
        return false;
    }
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        vecTaskInfo.push_back(row2TaskInfo(row));
    }
    mysql_free_result(result);

    return true;
}

