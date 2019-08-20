#ifndef _TIMER_STRUCT_H_
#define _TIMER_STRUCT_H_
#include <string>
#include <time.h>

#define NOTIMEVALUE 99999
#define NOUID       -1
#define NOACTIVE    -1

namespace mts_timer
{
    struct TaskTime{
        int m_nYear;
        int m_nMonth;
        int m_nDay;
        int m_nWeekly;
        int m_nHour;
        int m_nMin;
        int m_nSec;
        std::string m_strOffsetTime;
        
    };
    struct TaskData{
        std::string m_strClass;
        std::string m_strMsgType;
        std::string m_strContents;
        int m_nActive;
    };
    struct TaskInfo{
        int64_t m_nUid; //　广播 uid = -1　
        std::string m_strAppid;
        TaskTime m_taskTime;
        TaskData m_taskData;
        time_t m_nExpTime;
        std::string m_strTimerId;
    };
    struct Condition{
        int64_t m_nUid;
        std::string m_strAppid;
        TaskTime m_taskTime;
        TaskData m_taskData;
    };
}

#endif