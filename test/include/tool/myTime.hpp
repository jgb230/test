#ifndef _MYTIME_H_
#define _MYTIME_H_

#include "calendar.h"
using namespace mts_timer;

void testcheckV(){
    Calendar *cl = new Calendar();
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
    Calendar *cl = new Calendar();

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
    Calendar *cl = new Calendar();
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
    Calendar *cl = new Calendar();
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
    Calendar *cl = new Calendar();
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
    Calendar *cl = new Calendar();
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
    Calendar *cl = new Calendar();
    {
        int checkV[7] = {2019, -1, 5, 15, -1, -2, 0}; // 2019年每月5号15点每分每2秒
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 1);

        int nowTm[7] = {2019, 9, 5, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }
    {
        int checkV[7] = {2019, -1, 5, 15, -1, -2, 0}; // 2019年每月5号15点每分每2秒
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, -10);

        int nowTm[7] = {2019, 9, 5, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }
    {
        int checkV[7] = {2020, -1, 5, 15, -1, -2, 0}; // 2020年每月5号15点每分每2秒
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, -10);

        int nowTm[7] = {2019, 9, 5, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }

    {
        int checkV[7] = {2019, -1, 0, 15, -1, -2, 2}; // 2019年每月周二15点每分每2秒
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 15, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }

    {
        int checkV[7] = {2019, 9, 20, 15, 10, 0, 0}; // 2019年9月20号15点１０分０秒
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 13, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }
    {
        int checkV[7] = {2019, 9, 20, 15, 10, 0, 0}; // 2019年9月20号15点１０分０秒
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 13, 14, 50, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, false);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }
    {
        int checkV[7] = {2019, 8, 22, 10, -1, 0, 0}; // 2019年9月20号15点１０分０秒
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 22, 10, 43, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }

    {
        int checkV[7] = {2019, 8, 22, 10, -1, 0, 0}; // 2019年9月20号15点１０分０秒
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        int nowTm[7] = {2019, 8, 22, 10, 43, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }

    {
        cl->set(YEAR, 2019);
        cl->set(MONTH, 8);
        cl->set(HOUR, 11);
        cl->set(MINUTE, -1);
        cl->set(SECOND, 20);
        cl->set(DAY, 22);
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);
        // int nowTm[7] = {2019, 8, 22, 10, 43, 0, 0};
        // time_t t = cl->getTimeByValuesA(nowTm);
        time_t ret = cl->getTime(false);
        int tmpTm1[6] = {-1, -1, -1, -1, -1, -1};
        std::cout << "m_time:" << ret << std::endl;
        cl->getValuesByTime(ret, tmpTm1);
        std::cout << std::endl << ret << std::endl << tmpTm1[YEAR] << "年" << tmpTm1[MONTH] << "月" << tmpTm1[DAY] << "日" 
                << tmpTm1[HOUR] << "时" << tmpTm1[MINUTE] << "分" << tmpTm1[SECOND] << "秒" << std::endl;
    
        cl->set(YEAR, 2019);
        cl->set(MONTH, 8);
        cl->set(HOUR, 11);
        cl->set(MINUTE, -1);
        cl->set(SECOND, 20);
        cl->set(DAY, 22);
        cl->printAllTimeValue(cl->m_values);
        ret = cl->getTime(true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        std::cout << "m_time:" << ret << std::endl;
        cl->getValuesByTime(ret, tmpTm);
        std::cout << std::endl << ret << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
        cl->set(YEAR, 2019);
        cl->set(MONTH, 8);
        cl->set(HOUR, 11);
        cl->set(MINUTE, -1);
        cl->set(SECOND, 21);
        cl->set(DAY, 22);
        cl->printAllTimeValue(cl->m_values);
        ret = cl->getTime(false);
        int tmpTm2[6] = {-1, -1, -1, -1, -1, -1};
        std::cout << "m_time:" << ret << std::endl;
        cl->getValuesByTime(ret, tmpTm2);
        std::cout << std::endl << ret << std::endl << tmpTm2[YEAR] << "年" << tmpTm2[MONTH] << "月" << tmpTm2[DAY] << "日" 
                << tmpTm2[HOUR] << "时" << tmpTm2[MINUTE] << "分" << tmpTm2[SECOND] << "秒" << std::endl;

    }

    {
        int checkV[7] = {2019, 9, 5, 15, -4, 0, 0}; // 2019年9月5号15点每4分
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);

        int nowTm[7] = {2019, 9, 5, 15, 58, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }
    {
        int checkV[7] = {2019, 9, -1, 15, -4, 0, 0}; // 2019年9月每天15点每４分
        for (int i = 0; i < FIELDMAX; ++i){
            cl->m_values[i]   = checkV[i];
        }
        cl->printAllTimeValue(cl->m_values);
        std::cout << std::endl;
        cl->add(MINUTE, 0);

        int nowTm[7] = {2019, 9, 5, 15, 58, 0, 0};
        time_t t = cl->getTimeByValuesA(nowTm);
        cl->calulateTime(t, true);
        int tmpTm[6] = {-1, -1, -1, -1, -1, -1};
        cl->getValuesByTime(cl->m_time, tmpTm);
        std::cout << std::endl << cl->m_time << std::endl << tmpTm[YEAR] << "年" << tmpTm[MONTH] << "月" << tmpTm[DAY] << "日" 
                << tmpTm[HOUR] << "时" << tmpTm[MINUTE] << "分" << tmpTm[SECOND] << "秒" << std::endl;
    
    }
}

void timeTest(){
    testCalulate();

}

#endif