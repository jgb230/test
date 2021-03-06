#ifndef _MYTIME_H_
#define _MYTIME_H_

#include "calendar.h"
using namespace mts_timer;

void testcheckV(){
    Calendar *cl = new Calendar();bool bss = true;
    int nextTm[6] = {2019, 8, 15, 0, 0, 0};
    std::cout << " set year 2019 " << cl->checkValue(YEAR, 2019) << std::endl;
    std::cout << " set year 0 " << cl->checkValue(YEAR, 0) << std::endl;
    std::cout << " set year -1 " << cl->checkValue(YEAR, -1) << std::endl;

    std::cout << " set month 13 " << cl->checkValue(MONTH, 13)<< std::endl;
    std::cout << " set month 5 " << cl->checkValue(MONTH, 5)<< std::endl;
    std::cout << " set month 0 " << cl->checkValue(MONTH, 0)<< std::endl;
    std::cout << " set month -1 " << cl->checkValue(MONTH, -1)<< std::endl;

    cl->m_values[YEAR] = 2000; cl->m_values[MONTH] = 2;
    std::cout << " set DAY 31 " << cl->checkValue(DAY, 30)<< std::endl;
    std::cout << " set DAY 5 " << cl->checkValue(DAY, 5)<< std::endl;
    std::cout << " set DAY 0 " << cl->checkValue(DAY, 0)<< std::endl;
    std::cout << " set DAY -1 " << cl->checkValue(DAY, -1)<< std::endl;

    cl->m_values[YEAR] = 2019; cl->m_values[MONTH] = 7;
    std::cout << " set DAY 31 " << cl->checkValue(DAY, 30)<< std::endl;
    std::cout << " set DAY 5 " << cl->checkValue(DAY, 5)<< std::endl;
    std::cout << " set DAY 0 " << cl->checkValue(DAY, 0)<< std::endl;
    std::cout << " set DAY -1 " << cl->checkValue(DAY, -1)<< std::endl;

    std::cout << " set HOUR 31 " << cl->checkValue(HOUR, 30)<< std::endl;
    std::cout << " set HOUR 5 " << cl->checkValue(HOUR, 5)<< std::endl;
    std::cout << " set HOUR 0 " << cl->checkValue(HOUR, 0)<< std::endl;
    std::cout << " set HOUR -1 " << cl->checkValue(HOUR, -1)<< std::endl;

    std::cout << " set MINUTE 77 " << cl->checkValue(MINUTE, 77)<< std::endl;
    std::cout << " set MINUTE 5 " << cl->checkValue(MINUTE, 5)<< std::endl;
    std::cout << " set MINUTE 0 " << cl->checkValue(MINUTE, 0)<< std::endl;
    std::cout << " set MINUTE -1 " << cl->checkValue(MINUTE, -1)<< std::endl;

    std::cout << " set SECOND 77 " << cl->checkValue(SECOND, 77)<< std::endl;
    std::cout << " set SECOND 5 " << cl->checkValue(SECOND, 5)<< std::endl;
    std::cout << " set SECOND 0 " << cl->checkValue(SECOND, 0)<< std::endl;
    std::cout << " set SECOND -1 " << cl->checkValue(SECOND, -1)<< std::endl;
}

void testCheckAll(){
    Calendar *cl = new Calendar();bool bss = true;

    {
        int checkV[7] = {2019, 8, 15, 0, 0, 0, 2}; // false
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check 2019, 8, 15, 0, 0, 0, 2 " << cl->checkAllValue(); 
        std::cout << std::endl;
    }
    {
        int checkV[7] = {2019, 8, 0, 0, 0, 0, 0}; // false
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check 2019, 8, 0, 0, 0, 0, 0 " <<cl->checkAllValue();
        std::cout << std::endl;
    }
    {
        int checkV[7] = {-1, -1, -2, 0, 0, 0, 2}; // false
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check -1, -1, -2, 0, 0, 0, 2 " <<cl->checkAllValue();
        std::cout << std::endl;
    }{
        int checkV[7] = {-1, -1, 15, 0, 0, 0, 0}; // true
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check -1, -1, 15, 0, 0, 0, 0 " <<cl->checkAllValue();
        std::cout << std::endl;
    }{
        int checkV[7] = {-1, -1, -2, 0, 0, 0, 0}; // true
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check -1, -1, -2, 0, 0, 0, 0 " <<cl->checkAllValue();
        std::cout << std::endl;
    }{
        int checkV[7] = {-1, -1, -2, -1, 0, 0, 0}; // false
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check -1, -1, -2, -1, 0, 0, 0 " <<cl->checkAllValue();
        std::cout << std::endl;
    }{
        int checkV[7] = {-1, -1, -2, 15, 0, 0, 0}; // true
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check -1, -1, -2, 15, 0, 0, 0 " <<cl->checkAllValue();
        std::cout << std::endl;
    }{
        int checkV[7] = {-1, -1, 0, 15, 0, 0, 3}; // true
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check -1, -1, 0, 15, 0, 0, 3 " <<cl->checkAllValue();
        std::cout << std::endl;
    }{
        int checkV[7] = {-1, -1, 5, 15, -1, -2, 0}; // true
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check -1, -1, 5, 15, -1, -2, 0 " <<cl->checkAllValue();
        std::cout << std::endl;
    }{
        int checkV[7] = {2020, -1, 5, 15, -1, -2, 0}; // true
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check 2020, -1, 5, 15, -1, -2, 0 " <<cl->checkAllValue();
        std::cout << std::endl;
    }{
        int checkV[7] = {2019, -1, 5, 15, -1, -2, 0}; // true
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        std::cout << " check 2019, -1, 5, 15, -1, -2, 0 " <<cl->checkAllValue();
        std::cout << std::endl;
    }
}   

void testgetTimeByValuesD(){
    Calendar *cl = new Calendar();bool bss = true;
    {
    int nextTm[6] = {2019, 8, 15, 0, 0, 0};
    int tmpTm[6] = {-1};
    time_t t = cl->getTimeByValuesD(nextTm);
    tm *tmTmp = localtime(&t);
    std::cout << tmTmp->tm_year + 1900 << "年" << tmTmp->tm_mon + 1 << "月" << tmTmp->tm_mday << "日" 
                << tmTmp->tm_hour << "时" << tmTmp->tm_min << "分" << tmTmp->tm_sec << "秒" 
                << "　周" << tmTmp-> tm_wday << std::endl;
    t = cl->getTimeByValuesA(nextTm);
    tmTmp = localtime(&t);
    std::cout << tmTmp->tm_year + 1900 << "年" << tmTmp->tm_mon + 1 << "月" << tmTmp->tm_mday << "日" 
                << tmTmp->tm_hour << "时" << tmTmp->tm_min << "分" << tmTmp->tm_sec << "秒" 
                << "　周" << tmTmp-> tm_wday << std::endl;
    cl->getValuesByTime(t, tmpTm);
    std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }

    {
    int nextTm[6] = {2020, 8, 15, 20, 12, 12};
    int tmpTm[6] = {-1};
    time_t t = cl->getTimeByValuesD(nextTm);
    tm *tmTmp = localtime(&t);
    std::cout << tmTmp->tm_year + 1900 << "年" << tmTmp->tm_mon + 1 << "月" << tmTmp->tm_mday << "日" 
                << tmTmp->tm_hour << "时" << tmTmp->tm_min << "分" << tmTmp->tm_sec << "秒 周" 
                << tmTmp-> tm_wday << std::endl;
    t = cl->getTimeByValuesA(nextTm);
    tmTmp = localtime(&t);
    std::cout << tmTmp->tm_year + 1900 << "年" << tmTmp->tm_mon + 1 << "月" << tmTmp->tm_mday << "日" 
                << tmTmp->tm_hour << "时" << tmTmp->tm_min << "分" << tmTmp->tm_sec << "秒" 
                << "　周" << tmTmp-> tm_wday << std::endl;
    cl->getValuesByTime(t, tmpTm);
    std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    
}

void testStemp(){
    Calendar *cl = new Calendar();bool bss = true;
    {
        int tmpTm[6] = {2000, 2, 28, 23, 59, 59};
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
        std::cout << "秒加1" << std::endl;
        cl->stepTime(SECOND, 1, tmpTm);
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        int tmpTm[6] = {2000, 2, 29, 23, 59, 59};
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
        std::cout << "秒加1" << std::endl;
        cl->stepTime(SECOND, 1, tmpTm);
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        int tmpTm[6] = {2019, 2, 28, 23, 59, 59};
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
        std::cout << "秒加1" << std::endl;
        cl->stepTime(SECOND, 1, tmpTm);
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        int tmpTm[6] = {2019, 12, 31, 23, 59, 59};
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
        std::cout << "秒加1" << std::endl;
        cl->stepTime(SECOND, 1, tmpTm);
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        int tmpTm[6] = {2019, 12, 31, 23, 59, 59};
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
        std::cout << "时加８" << std::endl;
        cl->stepTime(HOUR, 8, tmpTm);
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        int tmpTm[6] = {2019, 12, 31, 23, 59, 59};
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
        std::cout << "分加８" << std::endl;
        cl->stepTime(MINUTE, 8, tmpTm);
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        int tmpTm[6] = {2019, 12, 28, 23, 59, 59};
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
        std::cout << "日加7" << std::endl;
        cl->stepTime(DAY, 7, tmpTm);
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        int tmpTm[6] = {2019, 12, 28, 23, 59, 59};
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
        std::cout << "月加7" << std::endl;
        cl->stepTime(MONTH, 7, tmpTm);
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
}

void testcmp(){
    Calendar *cl = new Calendar();bool bss = true;
    {
        int tmpTm1[6] = {2019, 8, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 10, 14, 44, 23};
        std::cout << "cmp {2019, 8, -1, -1, -1, -1} {2019, 8, 10, 14, 44, 23} " << cl->cmpTime(tmpTm1, tmpTm2) << std::endl;
    }
    {
        int tmpTm1[6] = {2019, 7, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 10, 14, 44, 23};
        std::cout << "cmp {2019, 7, -1, -1, -1, -1} {2019, 8, 10, 14, 44, 23} " << cl->cmpTime(tmpTm1, tmpTm2) << std::endl;
    }
    {
        int tmpTm1[6] = {2019, 9, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 10, 14, 44, 23};
        std::cout << "cmp {2019, 9, -1, -1, -1, -1} {2019, 8, 10, 14, 44, 23} " << cl->cmpTime(tmpTm1, tmpTm2) << std::endl;
    }
    
}

void testfindNearestWeekDay(){
    Calendar *cl = new Calendar();bool bss = true;
    {
        cl->m_values[WEEK] = 7;
        int tmpTm[6] = {2019, 8, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 15, 14, 44, 23};
        cl->findNearestWeekDay(tmpTm, tmpTm2);
        std::cout << "findNearestWeekDay {2019, 8, -1, -1, -1, -1} {2019, 8, 15, 14, 44, 23} 周" << cl->m_values[WEEK] << std::endl;
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        cl->m_values[WEEK] = 4;
        int tmpTm[6] = {2019, 8, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 15, 14, 44, 23};
        cl->findNearestWeekDay(tmpTm, tmpTm2);
        std::cout << "findNearestWeekDay {2019, 8, -1, -1, -1, -1} {2019, 8, 15, 14, 44, 23} 周" << cl->m_values[WEEK] << std::endl;
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        cl->m_values[WEEK] = 6;
        int tmpTm[6] = {2020, 8, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 15, 14, 44, 23};
        cl->findNearestWeekDay(tmpTm, tmpTm2);
        std::cout << "findNearestWeekDay {2019, 8, -1, -1, -1, -1} {2019, 8, 15, 14, 44, 23} 周" << cl->m_values[WEEK] << std::endl;
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        cl->m_values[WEEK] = 7;
        int tmpTm[6] = {2020, 8, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 15, 14, 44, 23};
        cl->findNearestWeekDay(tmpTm, tmpTm2);
        std::cout << "findNearestWeekDay {2019, 8, -1, -1, -1, -1} {2019, 8, 15, 14, 44, 23} 周" << cl->m_values[WEEK] << std::endl;
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        cl->m_values[WEEK] = 4;
        int tmpTm[6] = {2020, 8, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 15, 14, 44, 23};
        cl->findNearestWeekDay(tmpTm, tmpTm2);
        std::cout << "findNearestWeekDay {2019, 8, -1, -1, -1, -1} {2019, 8, 15, 14, 44, 23} 周" << cl->m_values[WEEK] << std::endl;
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
    {
        cl->m_values[WEEK] = 2;
        int tmpTm[6] = {2019, 8, -1, -1, -1, -1};
        int tmpTm2[6] = {2019, 8, 15, 14, 44, 23};
        cl->findNearestWeekDay(tmpTm, tmpTm2);
        std::cout << "findNearestWeekDay {2019, 8, -1, -1, -1, -1} {2019, 8, 15, 14, 44, 23} 周" << cl->m_values[WEEK] << std::endl;
        std::cout << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    }
}

void testCalulate(){
    
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, -1, 5, 15, -1, -2, 0}; // 2019年每月5号15点每分每2秒
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 1);

        int nowTm[7] = {2019, 9, 5, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年9月5日15时1分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "1计算结果: " << res << std::endl;
            std::cout << "1正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------1结果正确-------" << std::endl;
        }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, -1, 5, 15, -1, -2, 0}; // 2019年每月5号15点每分每2秒
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, -10);

        int nowTm[7] = {2019, 9, 5, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年9月5日14时50分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "2计算结果: " << res << std::endl;
            std::cout << "2正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------2结果正确-------" << std::endl;
        }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2020, -1, 5, 15, -1, -2, 0}; // 2020年每月5号15点每分每2秒
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, -10);

        int nowTm[7] = {2019, 9, 5, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);

        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2020年1月5日14时50分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "3计算结果: " << res << std::endl;
            std::cout << "3正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------3结果正确-------" << std::endl;
        }
        }
    }

    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, -1, 0, 15, -1, -2, 2}; // 2019年每月周二15点每分每2秒
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 15, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年8月20日15时0分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "4计算结果: " << res << std::endl;
            std::cout << "4正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------4结果正确-------" << std::endl;
        }
        }
    }

    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 9, 20, 15, 10, 0, 0}; // 2019年9月20号15点１０分０秒
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 13, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年9月20日15时10分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "5计算结果: " << res << std::endl;
            std::cout << "5正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------5结果正确-------" << std::endl;
        }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 8, 22, 10, -1, 0, 0};
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }

        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 22, 10, 43, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年8月22日10时44分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "6计算结果: " << res << std::endl;
            std::cout << "6正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------6结果正确-------" << std::endl;
        }
        }
    }

    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 8, 22, 10, -1, 0, 0};
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 22, 10, 43, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年8月22日10时43分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "7计算结果: " << res << std::endl;
            std::cout << "7正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------7结果正确-------" << std::endl;
        }}
    }

    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 9, 5, 15, -4, 0, 0}; // 2019年9月5号15点每4分
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);

        int nowTm[7] = {2019, 9, 5, 15, 58, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "1970年1月1日8时0分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "8计算结果: " << res << std::endl;
            std::cout << "8正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------8结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 9, -1, 15, -4, 0, 0}; // 2019年9月每天15点每４分
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);

        int nowTm[7] = {2019, 9, 5, 15, 58, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年9月6日15时0分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "9计算结果: " << res << std::endl;
            std::cout << "9正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------9结果正确-------" << std::endl;
        }}
    }

    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, -1, -1, 10, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(YEAR, -1);

        int nowTm[7] = {2019, 8, 28, 17, 10, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年8月28日18时10分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "10计算结果: " << res << std::endl;
            std::cout << "10正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------10结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, -1, -1, 10, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 2);
        cl->add(SECOND, -1);

        int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年8月28日17时11分59秒";
        if(strcmp(cres, res) != 0){
            std::cout << "11计算结果: " << res << std::endl;
            std::cout << "11正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------11结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, 2, 30, -1, 0, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(SECOND, 0);
        int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年9月20日15时10分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "12计算结果: " << res << std::endl;
            std::cout << "12正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------12结果正确-------" << std::endl;
        }}
        std::cout << "12正确结果: -1年2月最多有29天，但是 日 参数为30" << std::endl;
    }   
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, 2, 29, -1, 0, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(SECOND, 0);
        int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2020年2月29日0时0分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "13计算结果: " << res << std::endl;
            std::cout << "13正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------13结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, 0, 12, 15, 0, 4}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(SECOND, 0);
        int nowTm[7] = {2019, 8, 29, 15, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年9月5日12时15分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "14计算结果: " << res << std::endl;
            std::cout << "14正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------14结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, 0, 12, 15, 0, 4}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(SECOND, 0);
        int nowTm[7] = {2019, 8, 30, 15, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年9月5日12时15分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "15计算结果: " << res << std::endl;
            std::cout << "15正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------15结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, 8, 0, 12, 15, 0, 4}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(SECOND, 0);
        int nowTm[7] = {2019, 8, 29, 15, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2020年8月6日12时15分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "16计算结果: " << res << std::endl;
            std::cout << "16正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------16结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, 8, 0, 12, 15, 0, 4}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(SECOND, 0);
        int nowTm[7] = {2019, 8, 30, 15, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2020年8月6日12时15分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "17计算结果: " << res << std::endl;
            std::cout << "17正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------17结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 8, 20, 12, 15, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(HOUR, 3);
        int nowTm[7] = {2019, 8, 20, 15, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年8月20日15时15分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "18计算结果: " << res << std::endl;
            std::cout << "18正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------18结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 8, 20, 12, -2, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(HOUR, 3);
        int nowTm[7] = {2019, 8, 20, 15, 01, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        char res[256] = {0};
        sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
        char cres[256] = "2019年8月20日15时59分0秒";
        if(strcmp(cres, res) != 0){
            std::cout << "19计算结果: " << res << std::endl;
            std::cout << "19正确结果: " << cres << std::endl;
        }else{
            std::cout << "--------19结果正确-------" << std::endl;
        }}
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, 2, 30, -1, 0, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 0);
            int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月20日15时10分0秒";
            if(strcmp(cres, res) != 0){
                std::cout << "20计算结果: " << res << std::endl;
                std::cout << "20正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------20结果正确-------" << std::endl;
            }
        }
        std::cout << "20正确结果: -1年2月最多有29天，但是 日 参数为30" << std::endl;

    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, 32, -1, 0, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 0);
            int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月20日15时10分0秒";
            if(strcmp(cres, res) != 0){
                std::cout << "21计算结果: " << res << std::endl;
                std::cout << "21正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------21结果正确-------" << std::endl;
            }
        }
        std::cout << "21正确结果: -1年-1月最多有31天，但是 日 参数为32" << std::endl;
    }
    
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, 31, 24, 0, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 0);
            int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月20日15时10分0秒";
            if(strcmp(cres, res) != 0){
                std::cout << "22计算结果: " << res << std::endl;
                std::cout << "22正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------22结果正确-------" << std::endl;
            }
        }
        std::cout << "22正确结果: 时 参数为24不在0到23 之间!" << std::endl;
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, 31, 0, 60, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 0);
            int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月20日15时10分0秒";
            if(strcmp(cres, res) != 0){
                std::cout << "23计算结果: " << res << std::endl;
                std::cout << "23正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------23结果正确-------" << std::endl;
            }
        }
        std::cout << "23正确结果: 分 参数为60不在0到59 之间!" << std::endl;
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, 31, 0, 0, 61, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 0);
            int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月20日15时10分0秒";
            if(strcmp(cres, res) != 0){
                std::cout << "25计算结果: " << res << std::endl;
                std::cout << "25正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------25结果正确-------" << std::endl;
            }
        }
        std::cout << "25正确结果: 秒 参数为61不在0到59 之间!" << std::endl;
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, 31, 0, 0, 0, 8}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 0);
            int nowTm[7] = {2019, 8, 28, 17, 01, 0, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月20日15时10分0秒";
            if(strcmp(cres, res) != 0){
                std::cout << "26计算结果: " << res << std::endl;
                std::cout << "26正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------26结果正确-------" << std::endl;
            }
        }
        std::cout << "26正确结果: 星期 参数为8不在0到7 之间!" << std::endl;
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, -1, -1, -1, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 30);
            int nowTm[7] = {2019, 9, 2, 10, 25, 30, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月2日10时26分30秒";
            if(strcmp(cres, res) != 0){
                std::cout << "27计算结果: " << res << std::endl;
                std::cout << "27正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------27结果正确-------" << std::endl;
            }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 9, 2, 10, 25, 30, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(MINUTE, 1);
            int nowTm[7] = {2019, 9, 2, 10, 26, 30, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月2日10时26分30秒";
            if(strcmp(cres, res) != 0){
                std::cout << "28计算结果: " << res << std::endl;
                std::cout << "28正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------28结果正确-------" << std::endl;
            }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, 9, 2, 10, 25, 30, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(MINUTE, 1);
            int nowTm[7] = {2019, 9, 2, 10, 26, 30, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月2日10时26分30秒";
            if(strcmp(cres, res) != 0){
                std::cout << "29计算结果: " << res << std::endl;
                std::cout << "29正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------29结果正确-------" << std::endl;
            }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {2019, -1, -1, -1, -1, 0, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(MONTH, -1);
            int nowTm[7] = {2019, 9, 24, 13, 21, 0, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, true);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月24日13时22分0秒";
            if(strcmp(cres, res) != 0){
                std::cout << "30计算结果: " << res << std::endl;
                std::cout << "30正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------30结果正确-------" << std::endl;
            }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, -1, -1, -1, -5, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 15);
            int nowTm[7] = {2019, 9, 24, 11, 16, 38, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, false);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月24日11时16分53秒";
            if(strcmp(cres, res) != 0){
                std::cout << "31计算结果: " << res << std::endl;
                std::cout << "31正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------31结果正确-------" << std::endl;
            }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, -1, -1, -1, -5, 0}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 15);
            int nowTm[7] = {2019, 9, 24, 11, 16, 38, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, true);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年9月24日11时16分43秒";
            if(strcmp(cres, res) != 0){
                std::cout << "32计算结果: " << res << std::endl;
                std::cout << "32正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------32结果正确-------" << std::endl;
            }
        }
    }
    {
        Calendar *cl = new Calendar();bool bss = true;
        int checkV[7] = {-1, -1, 0, -1, 0, 0, 3}; // 
        for (int i = 0; i < FIELDMAX; ++i){
            if(!cl->set(FIELD(i), checkV[i])){
                bss = false;
                break;
            }
        }
        if(bss){
            cl->printAllTimeValue(cl->m_values);
            std::cout << std::endl;
            cl->add(SECOND, 15);
            int nowTm[7] = {2019, 9, 25, 23, 0, 0, 0};
            time_t t = cl->getTimeByValuesA(nowTm);
            cl->calulateTime(t, true);
            int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
            cl->getValuesByTime(cl->m_time, tmpTm);
            char res[256] = {0};
            sprintf(res, "%d年%d月%d日%d时%d分%d秒", 
                    tmpTm[YEAR], tmpTm[MONTH], tmpTm[DAY], tmpTm[HOUR], tmpTm[MINUTE], tmpTm[SECOND]);
            char cres[256] = "2019年10月2日0时0分0秒";
            if(strcmp(cres, res) != 0){
                std::cout << "33计算结果: " << res << std::endl;
                std::cout << "33正确结果: " << cres << std::endl;
            }else{
                std::cout << "--------33结果正确-------" << std::endl;
            }
        }
    }
}

void timeTest(){
    testCalulate();

}

#endif