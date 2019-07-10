#ifndef __COMMON_H
#define __COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/classification.hpp>
#define MAXLINE 10240 
#define SEPOR1 ","
#define SEPOR2 "|"

#define TESLOG(level,...) printf(__VA_ARGS__)

#define TIMEBEGIN(num) clock_t  start##num = clock();

#define TIMEEND(num)  clock_t  end##num = clock(); { double seconds  =(double)(end##num - start##num)/CLOCKS_PER_SEC;\
                      fprintf(stderr, "Use time is: %.6f\n", seconds);}

int getChar(int *preManu, char *file){
    int input = 0;
    char *choice = "选择过程: 0:批量插入 1:批量查询 2:批量更新　3:批量删除　\n          4:单条插入 5:单条查询 6:单条更新 7:单条删除   9:退出:";
    char fileTmp[1024] = {0}; 
    switch(*preManu){
        case -1:
        case 4:
        case 5:
        case 6:
        case 7:
            std::cout << choice ; 
            std::cin >> input;
            *preManu = input;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
            std::cout << "输入批量操作方式 0:文件　1:默认数据　:" ; 
            std::cin >> input;
            switch(input){
                case 0: 
                    std::cout << "输入文件绝对路径" ;
                    if(strlen(file) > 0){
                        std::cout << file << "[Y]";
                    } 
                    std::cout << " :";
                    std::cin >> fileTmp;
                    if (fileTmp[0] != '\n' && fileTmp[0] != 'Y' && fileTmp[0] != 'y'){
                        sprintf(file, "%s", fileTmp);
                    }
                    input = -2;
                    break;
                case 1:
                    std::cout << "输入数量:" ; 
                    std::cin >> input;
                    break;
                default:
                    break;
            }
            
            *preManu = -1;
            break;
        default:
            std::cout << "输入数字!\n" << choice ; 
            std::cin >> input;
            *preManu = input;
            break;
    }
    return input;
}
std::string ltrim(std::string str)
{
	int i = 0;
	int len = str.length();
	while (i < len)
	{
		if (str[i] == 32 || str[i] == 9 ) /*32:空格,9:横向制表符,10:换行符*/
		    i++;
        else
            break;
	}
	
	return str.substr(i, len - i);
	
}

/*删除str最后一个非空白字符后面的所有空白字符(空格符和横向制表符)*/
std::string rtrim(std::string str)
{
	int i = str.length() - 1;
	while (i >= 0)
	{
		if (str[i] == 32 || str[i] == 9 || str[i] == 10) /*32:空格,9:横向制表符,10:换行符*/
		    i--;
        else
            break;
        
	}
	return str.substr(0, i + 1);
}
std::string trim(std::string source){
    return ltrim(rtrim(source));
}

bool parseLine(const std::string content, std::map<std::string, std::string> &mapMemory
                , const std::string separator1, const std::string separator2){
    mapMemory.clear();
	std::vector <std::string> fields;
	std::vector <std::string> condt;
	std::string key;
    boost::split( fields, content, boost::is_any_of( separator1.c_str() ), boost::token_compress_on );
	for(auto field: fields) {
		condt.clear();
        boost::split( condt, field, boost::is_any_of( separator2.c_str() ), boost::token_compress_on );
		if(condt.size() == 2){
			mapMemory.insert(std::make_pair(trim(condt[0]), trim(condt[1])));
		}else{
            return false;
        }
	}
    if(mapMemory.size() > 0) {
        return true;
    }else {
        return false;
    }
}


#endif