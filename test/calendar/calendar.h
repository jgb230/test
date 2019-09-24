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
            ~Calendar(){};
            // 设置对应位置时间
            bool set(FIELD field, const int nValue);
            // 设置偏移时间
            bool add(FIELD field, int nValue);
            //　获取当前配置时间戳
            time_t getTime(bool bNextTime);
            //　根据时间戳 获取　nTm(分别为　年月日时分秒　对应值)
            static void getValuesByTime(time_t tm_,  int *nTm);
            // 检查所有参数正确性
            bool checkAllValue();
        public:
            //　根据nTm(分别为　年月日　对应值)获取时间戳
            time_t getTimeByValuesD(const int nTm[6]);
            time_t getTimeByValuesA(const int nTm[6]);
            // 对应单位加增加指定步长，true成功 false:增加步长后超出时间
            bool stepTime(FIELD field, int step, int *nTm);
            // 比较两个时间大小，1:nextTm>nowTm 1:nextTm=nowTm  -1:nextTm<nowTm
            int cmpTime(int *nextTm, int *nowTm);
            // 获取最近符合　星期　参数的日期 true成功 false:没有找到指定时间内的日期
            bool findNearestWeekDay(int *nextTm, int *nowTm);
            //　判断闰年
            bool isLeap(const int year);
            //　格式化输出所有时间参数
            void printAllTimeValue(int *nTm);
            // 检查日合法性
            bool checkDay(const int nValue, int *time);
            // 检查单个参数正确性
            bool checkValue(FIELD field, const int nValue);
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