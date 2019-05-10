#ifndef __MONGO_H
#define __MONGO_H

#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "common.hpp"
#define TESLOG(level,...) printf(__VA_ARGS__)

#define TIMEBEGIN(num)  struct timeval tv##num;\
                     struct timezone tz##num;\
                     gettimeofday(&tv##num,&tz##num);\
                     long int beginTime##num =  tv##num.tv_sec*1000000 + tv##num.tv_usec;\
                     printf("微妙，beginTime: %ld\n",beginTime##num);

#define TIMEEND(num) gettimeofday(&tv##num,&tz##num);\
                     long int endTime##num =  tv##num.tv_sec*1000000 + tv##num.tv_usec;\
                     printf("微妙，endTime: %ld; 微妙，runTime:%ld\n",\
                            endTime##num, endTime##num - beginTime##num);

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

bsoncxx::builder::stream::document makeDocument(std::map<std::string, std::string> mapMemory){

    bsoncxx::builder::stream::document document{};
    for (const auto &item: mapMemory){
        document << item.first << item.second;
    }
    return document;
}

std::vector<bsoncxx::document::value> makeDocuments(std::vector<std::map<std::string, std::string>> memorys){

    std::vector<bsoncxx::document::value> documents;
    for (const auto &item: memorys){
        documents.push_back(makeDocument(item).extract());
    }
    return documents;
}

int addMemory(std::map<std::string, std::string> mapMemory, mongocxx::database db, std::string table){


}




int mongoTest() {
    int preType = -1;
    int count = 1000;
    int type =1;
    TIMEBEGIN(10);
    TIMEEND(10);
    std::map<std::string, std::string> mapMemory;
    std::vector<std::map<std::string, std::string>> vecMemorys;
    bsoncxx::builder::stream::document doc;

    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"mongodb://jgb:123@172.16.0.19:27071/testdb"}};

    auto collection = conn["testdb"]["testcollection3"];
    while ((type = getChar(&preType)) != 9){
        switch(type){
            case 0:{
                count = getChar(&preType);
                TIMEBEGIN(1);
                for (int i = 0; i < count; i++){
                    TIMEBEGIN(7);
                    mapMemory.insert(make_pair("习惯行为","下班"));
                    mapMemory.insert(make_pair("主语","1812111752213087b6fe75a7RI003706"));
                    mapMemory.insert(make_pair("日期","200"));
                    mapMemory.insert(make_pair("分钟",std::to_string(i).c_str()));
                    collection.insert_one(makeDocument(mapMemory).extract());
                    mapMemory.clear();
                    TIMEEND(7);
                }

                TIMEEND(1);
                break;
            }
            case 1:{
                TIMEBEGIN(2);
                // Create the find options with the projection
                
                mongocxx::options::find opts{};
                opts.projection(make_document(kvp("_id", 1), kvp("习惯行为", 1)));
                mapMemory.clear();
                // Execute find with options
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语","1812111752213087b6fe75a7RI003706"));
                mapMemory.insert(make_pair("分钟","500"));
                doc = makeDocument(mapMemory);
                mongocxx::cursor cursor = collection.find(doc.view());
                TIMEEND(2);
                TIMEBEGIN(3);
                if (cursor.begin() == cursor.end()){
                    TESLOG(INFO, " =================\n"); 
                }
                for (auto&& doc : cursor) {
                    TESLOG(INFO, "%s\n", bsoncxx::to_json(doc).c_str()); 
                }
                TIMEEND(3);
                break;
            }
            case 2:{
                count = getChar(&preType);
                TIMEBEGIN(6);
                for (int i = 0; i < count; i++){
                    mapMemory.insert(make_pair("习惯行为","下班"));
                    mapMemory.insert(make_pair("主语","1812111752213087b6fe75a7RI003706"));
                    mapMemory.insert(make_pair("日期","200"));
                    mapMemory.insert(make_pair("分钟",std::to_string(i).c_str()));
                    collection.delete_one(makeDocument(mapMemory).extract());
                    mapMemory.clear();
                }
                TIMEEND(6);
                break;
            }
            case 3:{
                TIMEBEGIN(4);
                mapMemory.clear();
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语","1812111752213087b6fe75a7RI003706"));
                mapMemory.insert(make_pair("日期","200"));
                mapMemory.insert(make_pair("分钟","1001"));
                collection.insert_one(makeDocument(mapMemory).extract());
                mapMemory.clear();
                TIMEEND(4);
                break;
            }
            case 4:{
                TIMEBEGIN(5);
                mapMemory.clear();
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语","1812111752213087b6fe75a7RI003706"));
                mapMemory.insert(make_pair("日期","200"));
                mapMemory.insert(make_pair("分钟","1001"));
                doc = makeDocument(mapMemory);
                collection.delete_one(doc.view());
                mapMemory.clear();
                TIMEEND(5);
                break;
            }
        }
    }
    
}

#endif