#ifndef _TIMER_DB_H_
#define _TIMER_DB_H_
#include <string>
#include <vector>
#include <map>
#include "DBMySql.h"
#include "TimerStruct.hpp"

namespace mts_timer
{
    enum    TIMER_INFO
    {
        _TM_ID = 0,
        _TM_UID,
        _TM_APPID,
        _TM_TIMERID,
        _TM_YEAR,
        _TM_MONTH,
        _TM_DAY,
        _TM_WEEKLY,
        _TM_HOUR,
        _TM_MIN,
        _TM_SEC,
        _TM_CLASS,
        _TM_TYPE,
        _TM_CONTENT,
        _TM_ACTIVE,
        _TM_EXPIRE_TIME,
        _TM_OFFSET,
        _FIELD_MAX
    };

    struct TimerInfo{
        long int m_tm_id;
        long int m_tm_uid;
        std::string m_tm_appid;
        std::string m_tm_timerid;
        int m_tm_year;
        int m_tm_month;
        int m_tm_day;
        int m_tm_weekly;
        int m_tm_hour;
        int m_tm_min;
        int m_tm_sec;
        std::string m_tm_class;
        std::string m_tm_type;
        std::string m_tm_content;
        int m_tm_actinve;
        time_t m_tm_expire_time;
        std::string m_tm_offset;

    };

    class DBTimer{
        public:
            DBTimer();
            DBTimer(DBMySql *ptrMysql);
            ~DBTimer();

            // 存储定时任务到数据库中,先查重再插入
            bool storeTaskInfo(const TaskInfo &taskInfo);

            // 根据条件删除定时任务
            bool delTaskInfoByCdt(const std::vector<Condition> &vecConds);
            // 删除指定timerId定时任务
            bool delTaskInfoByTimerId(const std::vector<std::string> &vecTimerId);

            // 根据条件更新符合指定条件的定时任务,并将所有更新时间的时间任务返回
            bool updateTaskInfoByCdt(const std::vector<Condition> &vecConds, const TaskInfo &taskInfo, 
                                     std::vector<TaskInfo> &vecTaskInfos);
            // 更新指定timerId的下次执行时间
            bool updateExpTime(const std::string &strTimerId, time_t nExpTime);

            // 查询符合条件定时任务, 支持分页　nOffset分页开始位置默认从0开始， nCount记录个数　默认-1全部
            bool selectTaskInfoByCdt(const Condition &cond, std::vector<TaskInfo> &vecTaskInfos, 
                                     int nCount = -1, int nOffset = 0);
            // 查询指定时段的定时任务
            bool selectTaskInfoByTime(time_t nBeginTime, time_t nEndTime, std::vector<TaskInfo> &vecTaskInfo);

        public:
            // 根据cond打包sql条件语句
            std::string condition2sql(const Condition &cond);
            std::string packageTimet(const time_t nVal);
            std::string packageInt(const int nVal);
            std::string packageString(const std::string strVal);
            // row打包成TaskInfo结构体
            TaskInfo row2TaskInfo(const MYSQL_ROW &row);
            // 根据taskInfo拼接更新语句中SET部分
            std::string packageSet(const TaskInfo &taskInfo);
            // 比较条件content 与　定时任务content
            bool cmpContent(const std::string &strCC, const std::string &strTC, bool bPerfect, const std::string &type);
            // 格式化ｍａｐ内容
            std::string map2string(const std::map<std::string, std::string> &mapTc);
            std::string TaskInfo2string(const TaskInfo &task);
            std::string Condition2string(const Condition &cond);
            // TaskInfo to Condition
            Condition TaskInfo2Condition(const TaskInfo &taskInfo);
            // 添加TaskInfo
            bool insterTaskInfo(const TaskInfo &taskInfo);
            // 是否有完全一样的定时器任务
            bool hasSameTaskInfo(const TaskInfo &taskInfo);

            // 查询符合条件定时任务, 支持分页　nOffset分页开始位置默认从0开始， nCount记录个数　默认-1全部 
            // perfect true content完全匹配， false content包含匹配
            bool selectTaskInfo(const Condition &cond, std::vector<TaskInfo> &vecTaskInfos, 
                                bool perfect, int nCount = -1, int nOffset = 0);
            bool selectTIByCdts(const std::vector<Condition> &vecConds, std::vector<TaskInfo> &vecTaskInfos,
                             bool perfect);
            // 根据srcTaskInfo更新desTaskInfo
            void alterTaskInfo(const TaskInfo &srcTaskInfo, TaskInfo &desTaskInfo);
            // 判断更新taskInfo中是否有时间字段更新，如果有时间字段跟新返回true,否则返回false
            bool hasTimeChange(const TaskInfo &taskInfo);
        private:
            DBMySql    m_MySqlConn;
            DBMySql*   m_pMySqlConn;
    };
}

#endif