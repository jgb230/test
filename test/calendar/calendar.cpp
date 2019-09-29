#include "calendar.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <cmath>
#include <string>

using namespace mts_timer;

static         int    _MPDAYS[13]  = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 365}; //非闰年每个月天数
static         int    _MLDAYS[13]  = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 366}; //  闰年每个月天数
static         int    _TIMEMAX[7]  = {2199, 12, 31, 23, 59, 59, 7};                    //分别对应年、月、日、时、分、秒、星期 对应可设置最大值
static         int    _TIMEMIN[7]  = {1970,  1,  0,  0,  0,  0, 0};                    //分别对应年、月、日、时、分、秒、星期 对应可设置最小值
static std::string _TIMEFIELDS[7]  = {"年", "月", "日", "时", "分", "秒", "星期"};        //分别对应年、月、日、时、分、秒、星期 对应中文，日志输出用

//分别对应一年(365天)、一月(30天)、一日、一时、一分、一秒、一星期 对应秒数 计算偏移量用
static         int    _TIMEHEX[7]  = {31536000,  2592000,  86400,  3600,  60,  1, 604800}; 

Calendar::Calendar(){
    // 初始化时间参数为最小值
    for (int i = 0; i < FIELDMAX; ++i){
        m_values[i]   = _TIMEMIN[i];
    }
    m_offset = 0;
    m_time = 0;
    m_offValue = 0;
    m_offType = SECOND;
    m_reset = true;
}

bool Calendar::isLeap(const int year){
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return true;
    else 
        return false;
}

bool Calendar::checkDay(const int nValue, int *time){
    bool bLeap = false;
    int  monDays = _TIMEMAX[DAY];
    if(time[YEAR] > 0 && time[MONTH] > 0){
        bLeap = isLeap(time[YEAR]);
        monDays = bLeap ? _MLDAYS[time[MONTH] - 1] : _MPDAYS[time[MONTH] - 1];
    }else if (time[YEAR] < 0 && time[MONTH] > 0){
        monDays = _MLDAYS[time[MONTH] - 1];
    }else if (time[YEAR] < 0 && time[MONTH] < 0){
        monDays = _MLDAYS[0];
    }
    if(nValue >= 0 && (nValue < _TIMEMIN[DAY] || nValue > monDays)){
        std::cout << time[YEAR] << "年" << time[MONTH] << "月最多有" << monDays 
                                    << "天，但是 " << _TIMEFIELDS[DAY] << " 参数为" <<  nValue << std::endl;
        return false;
    }
    return true;
}

bool Calendar::checkValue(FIELD field, const int nValue){
    /*  支持参数 负数表示重复,正数表示具体 如: 年 -1:每年 -2:每两年 2019:2019年 其他FILED如是; 
    *   日和星期不能同时为0，必须有一个不为0; 
    *   星期总是大于等于0
    */
    switch(field){
        case YEAR:
        case MONTH:
        case HOUR:
        case MINUTE:
        case SECOND:
        case WEEK:
        {
            if(nValue >= 0 && (nValue < _TIMEMIN[field] || nValue > _TIMEMAX[field])){
                std::cout << " " << _TIMEFIELDS[field] << " 参数为" << nValue << "不在" 
                          << _TIMEMIN[field] << "到" << _TIMEMAX[field] << " 之间!" << std::endl;
                return false;
            }else if(field == WEEK && nValue < 0){
                std::cout << " " << _TIMEFIELDS[field] << " 参数为" << nValue << std::endl;
                return false;
            }
            return true;
        }
        case DAY:
        {
            return checkDay(nValue, m_values);
        }
        default:
        {
            std::cout << "checkValue field error!" << std::endl;
            return false;
        }
    }
}



bool Calendar::set(FIELD field, const int nValue){

    m_reset = true; // 需要重新计算时间
    if(!checkValue(field, nValue)){
        return false;
    }
    m_values[field] = nValue;
    std::cout << "设置参数 " << _TIMEFIELDS[field] << " " << nValue << std::endl;
    return true;
}

bool Calendar::add(FIELD field, int nValue){
    // 一年＝365天 一月＝30天 
    m_reset = true; // 需要重新计算时间
    if(field >= FIELDMAX){
        std::cout << "field 参数错误! field:" << field << std::endl;
        return false;
    }
    m_offset += nValue * _TIMEHEX[field];
    std::cout << "设置参数 " << _TIMEFIELDS[field] << " " << nValue << "为:" <<  m_offset<< std::endl;
    return true;
}

void Calendar::printAllTimeValue(int *nTm){
    std::cout << "-------时间--------"<< std::endl;
    for(int i = 0; i < FIELDMAX; ++i){
        std::cout << _TIMEFIELDS[i] << " : " << nTm[i] << ""<< std::endl;
    }
    std::cout << "-------时间--------"<< std::endl;
}

/*
*1、日、星期不能全为0，或全不为0
*2、当一个单位值小于-1时，粒度比当前小的单位的值不能小于0，大的单位的值不能小于-1
*/
bool Calendar::checkAllValue(){

    // 日、星期不能全为0，或全不为0
    if(0 == m_values[DAY] && 0 == m_values[WEEK]){
        std::cout << "日、星期参数全为0！"<< std::endl;
        return false;
    }
    if(0 != m_values[DAY] && 0 != m_values[WEEK]){
        std::cout << "日、星期参数全不为0！"<< std::endl;
        return false;
    }

    // 当一个单位值小于-1时，粒度比当前小的单位的值不能小于0，大的单位的值不能小于-1
    int nMinT = WEEK; // 值最小的时间单位 ,星期对应的值总是大于等于0的
    int nMinC = 0; // 小于-1的值的个数
    for (int i = WEEK - 1; i >= 0; --i){
        if(m_values[nMinT] > m_values[i]) {
            nMinT = i;
        }
        if(m_values[i] < -1) ++nMinC;
    }
    if(nMinC > 1){ 
        std::cout << "参数存在两个及以上值小于-1！"<< std::endl;
        printAllTimeValue(m_values);
        return false;
    }
    if(0 != m_values[WEEK] && nMinT < DAY && nMinC >= 1){
        std::cout << "星期不为0，但　" << _TIMEFIELDS[nMinT] << " 参数值:" << m_values[nMinT]<< std::endl;
        printAllTimeValue(m_values);
        return false;
    }
    int cmpValue = 0;
    if(nMinC == 1){
        for (int i = WEEK - 1; i >= 0; --i){
            if(i == nMinT){
                cmpValue = -1;
                continue;
            }
            if(m_values[i] < cmpValue) {
                std::cout << _TIMEFIELDS[nMinT] << " 参数值:" << m_values[nMinT] << ",但是 " 
                                              << _TIMEFIELDS[i] << " 参数值:" << m_values[i] << ","<< std::endl;
                return false;
            }
        }
        
    }
    

    return true;
}

time_t Calendar::getTimeByValuesD(const int nTm[6]){
    char buf[24] = {0};
    sprintf(buf, "%d-%02d-%02d", nTm[YEAR], nTm[MONTH], nTm[DAY]);
    tm tm_ = {0};
    strptime(buf, "%Y-%m-%d", &tm_); //将字符串转换为tm时间
    tm_.tm_isdst = -1;  
    time_t t_  = mktime(&tm_); //将tm时间转换为秒时间 
    return t_;
}

time_t Calendar::getTimeByValuesA(const int nTm[6]){
    char buf[24] = {0};
    sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", nTm[YEAR], nTm[MONTH], nTm[DAY], nTm[HOUR], nTm[MINUTE], nTm[SECOND]);
    tm tm_ = {0};
    strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间
    tm_.tm_isdst = -1;  
    time_t t_  = mktime(&tm_); //将tm时间转换为秒时间 
    return t_;
}


void Calendar::getValuesByTime(time_t mTm, int *nTm){
    tm *tmTmp = localtime(&mTm);
    nTm[YEAR] = tmTmp->tm_year + 1900;
    nTm[MONTH] = tmTmp->tm_mon + 1;
    nTm[DAY] = tmTmp->tm_mday;
    nTm[HOUR] = tmTmp->tm_hour;
    nTm[MINUTE] = tmTmp->tm_min;
    nTm[SECOND] = tmTmp->tm_sec;
    return ;
}

bool Calendar::stepTime(FIELD field, int step, int *nTm){
    if(field < YEAR || field >= FIELDMAX){
        return false;
    }
    if(step == 0){
        return true;
    }
    
    // 单位为年 直接加步长
    if (YEAR == field){
        nTm[YEAR] += step;
        if(nTm[YEAR] >= _TIMEMAX[YEAR]){

            return false;
        }
        return true;
    }

    int max = _TIMEMAX[field];
    int initV = 0;

    if(DAY == field){
        // 日的最大值根据所在年月计算
        bool bLeap = isLeap(nTm[YEAR]);
        max = bLeap ? _MLDAYS[nTm[MONTH] - 1] : _MPDAYS[nTm[MONTH] - 1];
    }
    if(DAY == field || MONTH == field){
        // 月日 都从1开始
        initV = 1;
    }

    if (nTm[field] + step <= max){
        // 增加步长后没有超过最大值，直接在对应单位增加
        nTm[field] += step;
    }else{
        // 增加步长后超过最大值，设置为余数，下一个单位加1
        /*
        *1、日、星期不能全为0，或全不为0
        *2、当一个单位值小于-1时，粒度比当前小的单位的值不能小于0，大的单位的值不能小于-1
        */
        int leftStep = nTm[field] + step + initV - 1 - max;
        nTm[field] = leftStep; 
        int fieldTmp = field - 1;
        int step = 0;
        bool isWeek = false;
        //找到前一个可变单位，即小于0的
        while(fieldTmp >= 0  ){
            if(fieldTmp == DAY && m_values[WEEK] != 0){
                step = 7;
                isWeek = true;
                break;
            }else if (m_values[fieldTmp] < 0){
                step =  abs(m_values[fieldTmp]);
                break;
            }
            
            --fieldTmp;
        }  
        if(fieldTmp < 0){
            std::cout << " 增加步长后超出时间范围" << std::endl;
            return false;
        }else{
            nTm[field] = initV;//　对应最小值
            // 前一个可变单位增加步长
            if (!stepTime(FIELD(fieldTmp), step, nTm)) 
                    return false;
            // 如果相隔只有一个单位继续增加剩余步长
            if (fieldTmp + 1 == field && !isWeek){
                if (!stepTime(FIELD(field), leftStep-1, nTm)) 
                    return false;
            }
        }
        
    }
    
    return true;
}

/*
只比较已设置时间单位的时间大小
eg: next {2019, 8, -1, -1, -1, -1} now {2019, 8, 10, 14, 44, 23}  返回 0
    next {2019, 7, -1, -1, -1, -1} now {2019, 8, 10, 14, 44, 23}  返回-1
    next {2019, 9, -1, -1, -1, -1} now {2019, 8, 10, 14, 44, 23}  返回 1
*/
int Calendar::cmpTime(int *nextTm, int *nowTm){

    for (int i = YEAR; i < WEEK; ++i){
        if(nextTm[i] == nowTm[i]){
            continue;
        }
        if(nextTm[i] == -1){
            return 0;
        }
        if(nextTm[i] > nowTm[i]){
            return 1;
        }
        if(nextTm[i] < nowTm[i]){
            return -1;
        }
    }
    return 0;
}
/*
获取最近符合 星期 参数的日期
*/
bool Calendar::findNearestWeekDay(int *nextTm, int *nowTm){
    // 比较next的 年月 与当前大小,因为设置nextTm 年月 时已经保证大于等于今天 年月，所以不会小于
    int ret = cmpTime(nextTm, nowTm);
    if(ret == 0){
        // 如果相等，设置为今天 日 的值
        nextTm[DAY] = nowTm[DAY];
    }else if (ret > 0){
        // 如果next大，设置为1号
        nextTm[DAY] = 1;
    }
    // 根据nextTm 年月日 获取 星期
    time_t nextT = getTimeByValuesD(nextTm);
    tm *tmTmp = localtime(&nextT);
    int step = 0;
    step = m_values[WEEK]%7 - tmTmp->tm_wday;
    if (step < 0) step += 7;
    std::cout << " step :" << step << std::endl;
    printAllTimeValue(nextTm);
    if (!stepTime(DAY, step, nextTm)){
        return false;
    }else{
        // 星期进位可能发生跳年,日会设置会1，要重新找到跳年后第一个准确星期
        // 根据nextTm 年月日 获取 星期
        printAllTimeValue(nextTm);
        nextT = getTimeByValuesD(nextTm);
        tmTmp = localtime(&nextT);
        step = 0;
        step = m_values[WEEK]%7 - tmTmp->tm_wday;
        if (step < 0) step += 7;
        std::cout << " step :" << step << std::endl;
        return stepTime(DAY, step, nextTm);
    }
}

void Calendar::firstTime(time_t now){
    int iNowTm[6] = {0};
    getValuesByTime(now, iNowTm);
    // 是否是过期时间
    bool lastTime = false;
    if(m_values[YEAR] > 0 && m_values[YEAR] < iNowTm[YEAR]){
        std::cout << " 年 参数值:" << m_values[YEAR] << ",小于今年" << iNowTm[YEAR]<< std::endl;
        lastTime = true;
    }
    int nextTime[6] = {-1, -1, -1, -1, -1, -1};
    int preCType = -1;
    int ret = 0;
    int step = 0;
    for (int i = YEAR ; i < WEEK; ++i){
        if (i == DAY && m_values[WEEK] != 0){
            // 找到最近符合星期参数的日期
            if(findNearestWeekDay(nextTime, iNowTm)){
                preCType = i;
                step = 7;
                continue;
            }else {
                m_time = 0;
                return;
            }
        }else if(m_values[i] < 0){
            ret = cmpTime(nextTime, iNowTm);
            if(ret == 0){
                //相等，设置当前时间
                nextTime[i] = iNowTm[i];
            }else if (ret > 0){
                // 大于当前时间，设为单位最小
                nextTime[i] = _TIMEMIN[i];
                if(i == DAY){
                    nextTime[i] = 1;
                }
            }else{
                // 小于当前，设为单位最大
                nextTime[i] = _TIMEMAX[i];
            }
            preCType = i;
            step = abs(m_values[preCType]);

        }else{
            nextTime[i] = m_values[i];            
        }
        
        // 如果设置的日的值，检查合法性,并找到合法日期
        if (i == DAY ){
            if(!checkDay(nextTime[DAY], nextTime)){
                
                m_time = 0;
                std::cout << " 没有找到合法的日期！" << std::endl;
                return;
        
            }
        }
        // 过期时间不涉及进位
        // 如果nextTime小于当前时间，上一个周期单位增加周期
        if( lastTime == false &&  cmpTime(nextTime, iNowTm) < 0 ){
            if(-1 == preCType){
                std::cout << " preCType:" <<  preCType<< std::endl;
                lastTime = true;
            }else{
                
                if(!stepTime(FIELD(preCType), step, nextTime)){
                    m_time = 0;
                    return;
                }
                // 星期进位可能发生跳年，找到跳年后第一个准确星期
                if(preCType == DAY && m_values[WEEK] != 0){
                    findNearestWeekDay(nextTime, iNowTm);
                }
            }
        }
        
    }
    time_t nTime = getTimeByValuesA(nextTime);

    nTime = getTimeByValuesA(nextTime);
    if (nTime + m_offset < now){
        if(preCType != -1){
            while(nTime + m_offset < now) {
                if(!stepTime(FIELD(preCType), step, nextTime)){
                    m_time = 0;
                    return;
                }
                nTime = getTimeByValuesA(nextTime);
            }
            
        }else{
            m_time = 0;
            return;
        }
        
    }
    std::cout << " 可执行时间 nTime:" << nTime << std::endl;
    m_time = nTime + m_offset;
    std::cout << " 偏移后可执行时间　m_time:" << m_time ;
}
void Calendar::afterTime(time_t now){
    int iNowTm[6] = {-1, -1, -1, -1, -1, -1};
    getValuesByTime(now, iNowTm);
    int fieldTmp = WEEK - 1;
    int step = 0;
    bool isWeek = false;
    //找到循环任务最小间隔
    while(fieldTmp >= 0  ){
        if(fieldTmp == DAY && m_values[WEEK] != 0){
            step = 7;
            isWeek = true;
            break;
        }else if (m_values[fieldTmp] < 0){
            step =  abs(m_values[fieldTmp]);
            break;
        }
        
        --fieldTmp;
    }  
    if(fieldTmp < 0){
        std::cout << " 不是循环任务" ;
        m_time = 0;
        return;
    }else{
        // 增加步长
        if(!stepTime(FIELD(fieldTmp), step, iNowTm)){
            m_time = 0;
            return;
        } 
    }
    m_time = getTimeByValuesA(iNowTm);
}
void Calendar::calulateTime(time_t now, bool bNextTime){
    std::cout << "now:" << now << std::endl;
    std::cout << "m_offset:" << m_offset << std::endl;
    std::cout << "bNextTime:" << bNextTime << std::endl;
    printAllTimeValue(m_values);

    if(!checkAllValue()) {
        // 参数检查不通过
        m_time = 0;
        return;
    }
    if(!bNextTime){
        firstTime(now);
    }else{
        afterTime(now);
    }
    
    
}

time_t Calendar::getTime(bool bNextTime){
    if(m_reset == true){
        m_reset = false;
        // 重新计算时间
        calulateTime(time(0), bNextTime);
    }
    return m_time;
}