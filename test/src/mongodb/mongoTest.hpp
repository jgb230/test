#ifndef __MONGO_H
#define __MONGO_H

#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "common.hpp"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::concatenate;
using mongocxx::collection;

bsoncxx::document::value makeDocument(std::map<std::string, std::string> mapMemory, int type, std::string robotid){

    bsoncxx::builder::stream::document document{};
    document << "主语" << robotid;
    for (const auto &item: mapMemory){
        if (item.first == "主语"){
            continue;
        }
        if(type == 1 && item.first != "主语"){
            document << item.first << item.second + "--update";
        }else {
            document << item.first << item.second;
        }
    }
    return document.extract();
}

std::vector<bsoncxx::document::value> makeDocuments(std::vector<std::map<std::string, std::string>> memorys, std::string robotid){

    std::vector<bsoncxx::document::value> documents;
    for (const auto &item: memorys){
        documents.push_back(makeDocument(item, 0, robotid));
    }
    return documents;
}

bsoncxx::document::value makeUpDocuments(std::map<std::string, std::string> mapMemory, std::string robotid){

    bsoncxx::builder::stream::document document{};

    document << "$set" << open_document << concatenate(makeDocument(mapMemory, 1, robotid)) << close_document;

    return document.extract();
}


void addM(int i,const char *file, mongocxx::collection &collection , int update){
    char line[MAXLINE] = {0};
    int nowline = 0;
    std::vector<int> errline;
    std::map<std::string, std::string> mapMemory;
    FILE *fp = fopen(file,"rb");
    std::string robotid = "00000000000000000RI0000" + std::to_string(i*10+i);
    while(fp != nullptr && fgets(line, MAXLINE, fp)){
        nowline++;
        if(parseLine(line, mapMemory, SEPOR1, SEPOR2) == false){
            errline.push_back(nowline);
            continue;
        }else{
            TIMEBEGIN(0);
            collection.insert_one(makeDocument(mapMemory, update, robotid));
            TIMEEND(0);
            mapMemory.clear();
        }
    }
    for(auto i: errline){
        TESLOG(INFO, "出错行:%d", i); 
    }
    TESLOG(INFO, "\n"); 
    fclose(fp);
}

void findM(int i,const char *file, mongocxx::collection &collection, int update){
    char line[MAXLINE] = {0};
    int nowline = 0;
    std::vector<int> errline;
    std::map<std::string, std::string> mapMemory;
    FILE *fp = fopen(file,"rb");
    std::string robotid = "00000000000000000RI0000" + std::to_string(i*10+i);
    while(fp != nullptr && fgets(line, MAXLINE, fp)){
        nowline++;
        if(parseLine(line, mapMemory, SEPOR1, SEPOR2) == false){
            errline.push_back(nowline);
            continue;
        }else{
            TIMEBEGIN(0);
            mongocxx::cursor cursor = collection.find(makeDocument(mapMemory, update, robotid));
            if (cursor.begin() == cursor.end()){
                TESLOG(INFO, " =================\n"); 
            }
            for (auto&& doc : cursor) {
                TESLOG(INFO, "%s\n", bsoncxx::to_json(doc).c_str()); 
            }
            TIMEEND(0);
            mapMemory.clear();
        }
    }
    for(auto i: errline){
        TESLOG(INFO, "出错行:%d", i); 
    }
    TESLOG(INFO, "\n"); 
    fclose(fp);
}

void updateM(int i,const char *file, mongocxx::collection &collection, int update){
    std::vector<int> errline;
    char line[MAXLINE] = {0};
    int nowline = 0;
    std::map<std::string, std::string> mapMemory;
    FILE *fp = fopen(file,"rb");
    std::string robotid = "00000000000000000RI0000" + std::to_string(i*10+i);
    while(fp != nullptr && fgets(line, MAXLINE, fp)){
        nowline++;
        if(parseLine(line, mapMemory, SEPOR1, SEPOR2) == false){
            errline.push_back(nowline);
            continue;
        }else{
            TIMEBEGIN(0);
            collection.update_many(makeDocument(mapMemory, update, robotid), makeUpDocuments(mapMemory, robotid));
            TIMEEND(0);
            mapMemory.clear();
        }
    }
    for(auto i: errline){
        TESLOG(INFO, "出错行:%d", i); 
    }
    TESLOG(INFO, "\n"); 
    fclose(fp);
}

void deletM(int i,const char *file, mongocxx::collection &collection, int update){
    std::vector<int> errline;
    char line[MAXLINE] = {0};
    int nowline = 0;
    std::map<std::string, std::string> mapMemory;
    FILE *fp = fopen(file,"rb");
    std::string robotid = "00000000000000000RI0000" + std::to_string(i*10+i);
    while(fp != nullptr && fgets(line, MAXLINE, fp)){
        nowline++;
        if(parseLine(line, mapMemory, SEPOR1, SEPOR2) == false){
            errline.push_back(nowline);
            continue;
        }else{
            TIMEBEGIN(0);
            collection.delete_one(makeDocument(mapMemory, update, robotid));
            TIMEEND(0);
            mapMemory.clear();
        }
    }
    for(auto i: errline){
        TESLOG(INFO, "出错行:%d", i); 
    }
    TESLOG(INFO, "\n"); 
    fclose(fp);
}

void firstCMD(mongocxx::collection &collection){
    std::map<std::string, std::string> mapMemory;
    std::string robotid = "00000000000000000RI000000";
    mapMemory.insert(make_pair("习惯行为","下班"));
    mapMemory.insert(make_pair("主语",robotid));
    mapMemory.insert(make_pair("日期","200"));
    mapMemory.insert(make_pair("分钟","0"));
    mongocxx::cursor cursor = collection.find(makeDocument(mapMemory, 0, robotid));
    if (cursor.begin() == cursor.end()){
        TESLOG(INFO, " =================\n"); 
    }
    for (auto&& doc : cursor) {
        TESLOG(INFO, "%s\n", bsoncxx::to_json(doc).c_str()); 
    }
}

// mongocxx::collection getCollection(){
//     mongocxx::instance inst{};
//     mongocxx::client conn{mongocxx::uri{"mongodb://jgb:123@172.16.0.19:27071/testdb"}};

//     return conn["testdb"]["testcollection3"];
// }



int mongoTest() {
    int preManu = -1;
    int count = 1000;
    int manu = 0;
    char *file = new char(1024);  
    std::map<std::string, std::string> mapMemory;
    std::string robotid = "1812111752213087b6fe75a7RI003706";
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"mongodb://jgb:123@172.16.0.19:27071/testdb"}};
    mongocxx::collection collection = conn["testdb"]["testcollection3"];
    int update = 0;
    int typel = 0;

    while ((manu = getChar(&preManu, file)) >= 0){
        switch(manu){
            case 0:{//批量插入
                count = getChar(&preManu, file);
                if(count == -2){
                    addM(0, file, collection, update);
                    
                }else {
                    for (int i = 0; i < count; i++){
                        mapMemory.insert(make_pair("习惯行为","下班"));
                        mapMemory.insert(make_pair("主语",robotid));
                        mapMemory.insert(make_pair("日期","200"));
                        mapMemory.insert(make_pair("分钟",std::to_string(i).c_str()));
                        TIMEBEGIN(0);
                        collection.insert_one(makeDocument(mapMemory, update, robotid));
                        TIMEEND(0);
                        mapMemory.clear();
                    }
                }
                update = 0;
                break;
            }
            case 1:{// 批量查询
                count = getChar(&preManu, file);
                if(count == -2){
                    findM(0, file, collection, update);
                    
                }else {
                    for (int i = 0; i < count; i++){
                        
                        mapMemory.insert(make_pair("习惯行为","下班"));
                        mapMemory.insert(make_pair("主语",robotid));
                        mapMemory.insert(make_pair("日期","200"));
                        mapMemory.insert(make_pair("分钟",std::to_string(i).c_str()));
                        TIMEBEGIN(0);
                        mongocxx::cursor cursor = collection.find(makeDocument(mapMemory, update, robotid));
                        if (cursor.begin() == cursor.end()){
                            TESLOG(INFO, " =================\n"); 
                        }
                        for (auto&& doc : cursor) {
                            TESLOG(INFO, "%s\n", bsoncxx::to_json(doc).c_str()); 
                        }
                        TIMEEND(0);

                        mapMemory.clear();
                    }
                }
                
                break;
            }
            case 2:{//批量更新
                count = getChar(&preManu, file);
                if(count == -2){
                    updateM( 0, file, collection, update);
                }else {
                    for (int i = 0; i < count; i++){
                        mapMemory.insert(make_pair("习惯行为","下班"));
                        mapMemory.insert(make_pair("主语",robotid));
                        mapMemory.insert(make_pair("日期","200"));
                        mapMemory.insert(make_pair("分钟",std::to_string(i).c_str()));
                        TIMEBEGIN(0);
                        collection.update_many(makeDocument(mapMemory, update, robotid), makeUpDocuments(mapMemory, robotid));
                        TIMEEND(0);
                        mapMemory.clear();
                        
                    }
                }
                update = 1;
                break;
            }
            case 3:{//批量删除
                
                count = getChar(&preManu, file);
                if(count == -2){
                    deletM(0, file, collection, update);
                }else {
                    for (int i = 0; i < count; i++){
                        
                        mapMemory.insert(make_pair("习惯行为","下班"));
                        mapMemory.insert(make_pair("主语",robotid));
                        mapMemory.insert(make_pair("日期","200"));
                        mapMemory.insert(make_pair("分钟",std::to_string(i).c_str()));
                        TIMEBEGIN(0);
                        collection.delete_one(makeDocument(mapMemory, update, robotid));
                        TIMEEND(0);                        
                        mapMemory.clear();
                    }
                }
                update = 0;
                break;
            }
            case 5:{// 单条查询
                TIMEBEGIN(2);
                // Create the find options with the projection
                
                mongocxx::options::find opts{};
                opts.projection(make_document(kvp("_id", 1), kvp("习惯行为", 1)));
                mapMemory.clear();
                // Execute find with options
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语",robotid));
                mapMemory.insert(make_pair("分钟","1001"));
                mongocxx::cursor cursor = collection.find(makeDocument(mapMemory, typel, robotid));
                TIMEEND(2);
                TIMEBEGIN(3);
                if (cursor.begin() == cursor.end()){
                    TESLOG(INFO, " =================\n"); 
                }
                for (auto&& doc : cursor) {
                    TESLOG(INFO, "%s\n", bsoncxx::to_json(doc).c_str()); 
                }
                mapMemory.clear();
                TIMEEND(3);
                typel = 0;
                break;
            }
            case 4:{// 单条插入
                TIMEBEGIN(4);
                mapMemory.clear();
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语",robotid));
                mapMemory.insert(make_pair("日期","200"));
                mapMemory.insert(make_pair("分钟","1001"));
                collection.insert_one(makeDocument(mapMemory, typel, robotid));
                mapMemory.clear();
                TIMEEND(4);
                break;
            }
            case 6:{ // 单条更新
                TIMEBEGIN(5);
                mapMemory.clear();
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语",robotid));
                mapMemory.insert(make_pair("日期","200"));
                mapMemory.insert(make_pair("分钟","1001"));
                collection.update_many(makeDocument(mapMemory, typel, robotid), makeUpDocuments(mapMemory, robotid));
                mapMemory.clear();
                TIMEEND(5);
                typel = 1;
                break;
            }
            case 7:{ // 单条删除
                TIMEBEGIN(5);
                mapMemory.clear();
                mapMemory.insert(make_pair("习惯行为","下班"));
                mapMemory.insert(make_pair("主语",robotid));
                mapMemory.insert(make_pair("日期","200"));
                mapMemory.insert(make_pair("分钟","1001"));
                collection.delete_one(makeDocument(mapMemory, typel, robotid));
                mapMemory.clear();
                TIMEEND(5);
                typel = 0;
                break;
            }
            case 9:{ // 退出
                return 0;
            }
        }
    }
    
}


void alltestM(int tid, const char *file, int choice){
    mongocxx::client conn{mongocxx::uri{"mongodb://jgb:123@172.16.0.19:27071/testdb"}};
    mongocxx::collection collection = conn["testdb"]["testcollection3"];
    firstCMD(collection);
    switch(choice){
        case 0:
            TESLOG(INFO, "+addM+++++++++++++++++++++++++++++++++++!\n");
            addM( tid, file,  collection, 0);
            break;
        case 1:
            TESLOG(INFO, "+findM+++++++++++++++++++++++++++++++++++!\n");
            findM( tid, file,  collection, 0);
            break;
        case 2:
            TESLOG(INFO, "+updateM+++++++++++++++++++++++++++++++++++!\n");
            updateM( tid, file,  collection, 0);
            break;
        case 3:
            TESLOG(INFO, "+deletM+++++++++++++++++++++++++++++++++++!\n");
            deletM( tid, file,  collection, 1);
            break;
    }
    
}

bool mongo_thread(int num, const char *file, int choice){
    mongocxx::instance inst{};

    std::thread *p[num];
    for(int i = 0; i < num; i++){
        p[i] = new std::thread(alltestM, i, file, choice);
        if (p[i] == nullptr) {
            TESLOG(INFO, "create pthread error!\n");
            return -1;
        }
        
    }
    for(int i = 0; i < num; i++){
        p[i]->join();
    }
}

#endif