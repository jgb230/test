#ifndef _PUBLIC_MEMORY_H_
#define _PUBLIC_MEMORY_H_
#include <string.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>
#include <boost/format.hpp>
#include <stdarg.h>
#include <thread>
namespace PublicMemory{

#define TIMEBEGIN(num) clock_t  start##num = clock();

#define TIMEEND(num)  clock_t  end##num = clock(); { double seconds  =(double)(end##num - start##num)/CLOCKS_PER_SEC;\
                      fprintf(stderr, "Use time is: %.6f\n", seconds);}
/*
多个有序集合取交集
1、找出所有集合当前位置最大值
2、所有集合前进直到等于或大于最大值,存在一个集合已经遍历完,退出
3、所有集合当前位置相等，则加入交集结果中,并且全部前进一个位置,存在一个集合已经遍历完,退出
4、跳到1

过程 
0、 三个有序集合　(1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
　　　　　　　　　　^            ^   ^    ^   ^      ^
                iter        end iter  end iter   end
1、　定位最大　
　　(1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
                               ^
                             maxId
2、　所有集合前进直到等于或大于最大值
　　　(1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
    　　　　^        ^    ^   ^  ^       ^
        　iter           iter  iter
3、  不相等，不加入结果集
4、　定位最大
　　　(1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
                    　　　^
                        maxId
５、　所有集合前进直到等于或大于最大值
　　　(1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
    　　　　　^      ^    ^   ^  　　^    ^
        　　iter      　　iter   　iter
６、相等，加入结果集，并前进一步
   (1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
    　　　　　^     ^      ^ ^  　  ^   ^
        　　iter      　　iter    iter
7、定位最大
　　　(1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
                    　　　　　^
                        　　maxId
８、　所有集合前进直到等于或大于最大值
　　　(1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
    　　　　　　　　　^^    　　^^  　　　　^^
        　　　　　　iter      iter   　　iter
９、相等，加入结果集，并前进一步
　　　(1,2,3,5,6,8,10) (2,5,10) (3,5,9,10)
    　　　　　　　　　^    　　^  　　　　　^
        　　　　　　iter      iter   　　iter
10、存在一个集合已经遍历完,退出
*/
void intersect(const std::vector<std::set<std::string>> &keys, std::set<std::string> &res){
    TIMEBEGIN(0);
    if(keys.size() == 1){
        for(auto it: keys[0]){
            res.insert(it);
        }
        TIMEEND(0);
        return;
    }
    std::set<std::string>::iterator it[keys.size()];
    std::set<std::string>::iterator end[keys.size()];
    bool anyEnd = false;
    std::string maxId = "";

    bool eque = true;

    for(int i = 0; i < keys.size(); ++i){
        it[i] = keys.at(i).begin();
        end[i] = keys.at(i).end();
        if(it[i] == end[i]){
            //　存在一个集合为空,退出
            TIMEEND(0);
            return;
        }
        // 找出所有集合当前位置最大值
        if(i == 0){
            maxId = *it[i];
        }else{
            if(maxId < *it[i]){
                maxId = *it[i];
            }
        }
    }

    while(1){
        for(int i = 0; i < keys.size(); ++i){
            //　所有集合前进直到等于或大于最大值
            while(  *it[i] < maxId) {
                ++(it[i]);
                if(it[i] == end[i]){
                    //　存在一个集合已经遍历完,退出
                    TIMEEND(0);
                    return;
                }
            }
            // 判断所有集合当前位置是否相等
            if(*it[i] > maxId){
                maxId = *it[i];
                eque = false;
            }
        }

        //　相等则加入交集结果中,并且前进第一集合一个位置
        if(eque){
            res.insert(maxId);
            ++(it[0]);
            if(it[0] == end[0]){
                //　第一个集合已经遍历完,退出
                TIMEEND(0);
                return;
            }
            maxId = *it[0];
        }
        eque = true;
    }

}

void intersect1(const std::vector<std::set<std::string>> &keys, std::set<std::string> &res){
    TIMEBEGIN(0);
    std::set<std::string> tmp1;
    std::set<std::string> tmp;
    if(keys.size() == 1){
        for(auto it: keys[0]){
            res.insert(it);
        }
        TIMEEND(0);
        return;
    }
    std::set_intersection(keys[0].begin(), keys[0].end(), keys[1].begin(), keys[1].end(), std::insert_iterator<std::set<std::string>>(tmp,tmp.begin()));
    if(tmp.size() == 0){
        TIMEEND(0);
        return;
    }
    for(auto it: tmp){
        tmp1.insert(it);
    }
    tmp.clear();
    for(int i = 2; i < keys.size(); i++){
        std::set_intersection(tmp1.begin(), tmp1.end(), keys[i].begin(), keys[i].end(), std::insert_iterator<std::set<std::string>>(tmp,tmp.begin()));
        if(tmp.size() == 0){
            TIMEEND(0);
            return;
        }
        tmp1.clear();
        for(auto it: tmp){
            tmp1.insert(it);
        }
        tmp.clear();
    }
    for(auto it: tmp1){
        res.insert(it);
    }
    TIMEEND(0);
}




}
#endif