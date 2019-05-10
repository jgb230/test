#ifndef __COMMON_H
#define __COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

int getChar(int *preType){
    int input = 0;
    switch(*preType){
        case -1:
        case 1:
        case 3:
        case 4:
            std::cout << "选择过程: 0:批量插入 1:单条查询　2:批量删除　3:单条插入 4:单条删除 9:退出:" ; 
            std::cin >> input;
            *preType = input;
            break;
        case 0:
        case 2:
            std::cout << "输入批量总数:" ; 
            std::cin >> input;
            *preType = -1;
            break;
        default:
            std::cout << "输入数字!\n选择过程: 0:批量插入 1:单条查询　2:批量删除　3:单条插入 4:单条删除 9:退出:" ; 
            std::cin >> input;
            *preType = input;
            break;
    }
    return input;
}
#endif