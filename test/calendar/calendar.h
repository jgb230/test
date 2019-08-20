#ifndef _TIMER_CALENDAR_H_
#define _TIMER_CALENDAR_H_

#include <string>

namespace mts_timer
{
    enum FIELD{
        YEAR = 0,
        MONTH,
        DAY,
        HOUR,
        MINUTE,
        SECOND,
        WEEK,
        FIELDMAX
    };
   
    class Calendar{
        public:

            Calendar();
            ~Calendar();
            // 设置对应位置时间
            bool set(FIELD field, const int nValue);
            // 设置偏移时间
            bool add(FIELD field, int nValue);
            //　获取当前配置时间戳
            time_t getTime(bool bNextTime);
        public:
            //　根据nTm(分别为　年月日时分秒　对应值)获取时间戳
            time_t getTimeByValuesD(const int nTm[6]);
            time_t getTimeByValuesA(const int nTm[6]);
            //　根据时间戳 获取　nTm(分别为　年月日时分秒　对应值)
            void getValuesByTime(time_t tm_,  int *nTm);
            // 对应单位加1
            void stepTime(FIELD field, int step, int *nTm);
            // 比较两个时间大小，1:nextTm>nowTm 1:nextTm=nowTm  -1:nextTm<nowTm
            int cmpTime(int *nextTm, int *nowTm);
            // 获取最近符合　星期　参数的日期
            void findNearestWeekDay(int *nextTm, int *nowTm);
            //　判断闰年
            bool isLeap(const int year);
            //　格式化输出所有时间参数
            void printAllTimeValue(int *nTm);
            // 检查单个参数正确性
            bool checkValue(FIELD field, const int nValue);
            // 检查所有参数正确性
            bool checkAllValue();
            // 计算时间,设置m_time
            void calulateTime(time_t now, bool bNextTime);
        public:
            int    m_values[7];
            FIELD  m_offType;
            int    m_offValue;
            time_t m_time, m_offset;
            bool   m_reset;
    };
}

#endif