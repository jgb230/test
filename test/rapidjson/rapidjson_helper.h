/*
 * =====================================================================================
 *
 *       Filename:  json.cpp
 *
 *    Description:  封装一层 json
 *
 *        Version:  1.0
 *        Created:  05/20/2016 10:44:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhao xiaobiao (all rights reserverd), zhaoxiaobiao@sina.cn
 *        Company:  
 *
 * =====================================================================================
 */

#pragma once
// 使用 stdstring
#define RAPIDJSON_HAS_STDSTRING 1

#include "document.h"     // rapidjson's DOM-style API
#include "prettywriter.h" // for stringify JSON
#include "writer.h"
#include "allocators.h"
#include "error/en.h"
#include "istreamwrapper.h"

#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <stdint.h>
#include <map>


//typedef std::vector<std::string> VS;
typedef rapidjson::Value Value; // json 对应的value 类型
typedef rapidjson::Document Document; // Document tree 类型

//解析字符串
#define json_parse(doc,cstr) (!doc.Parse(cstr).HasParseError())

//解析文件
#define json_parse_file(doc, path) ({ \
		std::fstream is(path);\
		rapidjson::IStreamWrapper isw(is);\
		!doc.ParseStream(isw).HasParseError();\
    })

//获取解析错误内容,返回 const char*
#define json_parse_errstr(doc) rapidjson::GetParseError_En(doc.GetParseError())


#define json_is_ok(value) (value.IsObject() || value.IsArray())
#define json_is_bool(value, strKey) (value.HasMember(strKey) && value[strKey].IsBool())
#define json_is_string(value, strKey) (value.HasMember(strKey) && value[strKey].IsString())
#define json_is_int32(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt())
#define json_is_uint32(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint())
#define json_is_int64(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt64())
#define json_is_uint64(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint64())
#define json_is_float(value, strKey) (value.HasMember(strKey) && value[strKey].IsFloat())
#define json_is_double(value, strKey) (value.HasMember(strKey) && value[strKey].IsDouble())

#define json_is_number(value, strKey) (value.HasMember(strKey) && value[strKey].IsNumber())
#define json_is_array(value, strKey) (value.HasMember(strKey) && value[strKey].IsArray())
#define json_is_object(value, strKey) (value.HasMember(strKey) && value[strKey].IsObject())

// 得到对应类型的数据，如果数据不存在则得到一个默认值
#define json_get_bool(value, strKey) (json_is_bool(value, strKey) && (value)[strKey].GetBool())
#define json_get_string(value, strKey) (json_is_string(value, strKey) ? (value)[strKey].GetString() : "")
#define json_get_int32(value, strKey) (json_is_int32(value, strKey) ? (value)[strKey].GetInt() : 0)
#define json_get_uint32(value, strKey) (json_is_uint32(value, strKey) ? (value)[strKey].GetUint() : 0)
#define json_get_int64(value, strKey) (json_is_int64(value, strKey) ? (value)[strKey].GetInt64() : 0)
#define json_get_uint64(value, strKey) (json_is_uint64(value, strKey) ? (value)[strKey].GetUint64() : 0)
#define json_get_float(value, strKey) (json_is_float(value, strKey) ? (value)[strKey].GetFloat() : 0)
#define json_get_double(value, strKey) (json_is_double(value, strKey) ? (value)[strKey].GetDouble() : 0)

#define json_get_array(value, strKey) (json_is_ok(value) && json_is_array(value, strKey) ? value[strKey] : Value(rapidjson::kArrayType).Move())
#define json_get_object(value, strKey) (json_is_ok(value) && json_is_object(value, strKey) ? value[strKey] : Value(rapidjson::kObjectType).Move())
#define json_get_value(value, strKey) (json_is_object(value, strKey) ? json_get_object(value, strKey) : json_get_array(value, strKey))


//数组操作
#define json_arr_is_ok(value) (value.IsArray())
#define json_arr_is_bool(value, index) (value[index].IsBool())
#define json_arr_is_string(value, index) (value[index].IsString())
#define json_arr_is_int32(value, index) (value[index].IsInt())
#define json_arr_is_uint32(value, index) ( value[index].IsUint())
#define json_arr_is_int64(value, index) (value[index].IsInt64())
#define json_arr_is_uint64(value, index) (value[index].IsUint64())
#define json_arr_is_float(value, index) (value[index].IsFloat())
#define json_arr_is_double(value, index) (value[index].IsDouble())

#define json_arr_is_number(value, index) (value[index].IsNumber())
#define json_arr_is_array(value, index) (value[index].IsArray())
#define json_arr_is_object(value, index) (value[index].IsObject())

// 得到对应类型的数据，如果数据不存在则得到一个默认值
#define json_arr_get_bool(value, index) (json_arr_is_bool(value, index) && (value)[index].GetBool())
#define json_arr_get_string(value, index) (json_arr_is_string(value, index) ? (value)[index].GetString() : "")
#define json_arr_get_int32(value, index) (json_arr_is_int32(value, index) ? (value)[index].GetInt() : 0)
#define json_arr_get_uint32(value, index) (json_arr_is_uint32(value, index) ? (value)[index].GetUint() : 0)
#define json_arr_get_int64(value, index) (json_arr_is_int64(value, index) ? (value)[index].GetInt64() : 0)
#define json_arr_get_uint64(value, index) (json_arr_is_uint64(value, index) ? (value)[index].GetUint64() : 0)
#define json_arr_get_float(value, index) (json_arr_is_float(value, index) ? (value)[index].GetFloat() : 0)
#define json_arr_get_double(value, index) (json_arr_is_double(value, index) ? (value)[index].GetDouble() : 0)

#define json_arr_get_array(value, index) (json_arr_is_array(value, index) ? value[index] : Value(rapidjson::kArrayType).Move())
#define json_arr_get_object(value, index) (json_arr_is_object(value, index) ? value[index] : Value(rapidjson::kObjectType).Move())
#define json_arr_get_value(value, index) (json_arr_is_object(value, index) ? json_arr_get_object(value, index) : json_arr_get_array(value, index))
//获取素组元素个数.
#define json_arr_size(arr) (arr.IsArray() ? arr.Size() : -1)


// 添加
#define json_mk_document(d) rapidjson::Document d(rapidjson::kObjectType);
#define json_mk_array(a)  rapidjson::Document a(rapidjson::kArrayType);
#define json_mk_obj(o) rapidjson::Document o(rapidjson::kObjectType);
/*
* 下面两个创建对象的宏是为了,把创建的arr或者obj对象嵌入到document里面去
* 例如:
* json_mk_document(doc);
* json_mk_embed_array(arr, doc);
* json_mk_embed_obj(obj1, doc);
* son_add_member(obj1, "key", "value");
* json_push_back(arr, obj1);
* json_add_member(doc, "content", arr);
* 为了保证统一的内存分配器,否则程序会崩溃
*/
#define json_mk_embed_array(arr,doc)  rapidjson::Document arr(rapidjson::kArrayType,&doc.GetAllocator());
#define json_mk_embed_obj(obj,doc) rapidjson::Document obj(rapidjson::kObjectType, &doc.GetAllocator());

#define json_push_back(arr, value) { \
    (arr).PushBack(value, arr.GetAllocator());  \
}

#define json_push_back_string(arr, stdStr){\
	json_push_back(arr, Value(stdStr.c_str(), stdStr.size(),arr.GetAllocator()));\
}



#define json_addstring_member(obj, strKey, strValue) {\
    obj.AddMember(Value(strKey, obj.GetAllocator()).Move(), Value(strValue, obj.GetAllocator()).Move(), obj.GetAllocator());  \
}

#define json_add_member(obj, strKey, value){\
    obj.AddMember(Value(strKey, obj.GetAllocator()).Move(), value, obj.GetAllocator());  \
}

// vector => array
#define json_vector_array(v, vec) ({ \
    for(size_t i = 0; i < vec.size(); i++) { \
        json_push_back(v, Value(vec[i], obj.GetAllocator()).Move());  \
    } \
})

// map => obj

template<typename T>
void json_map_obj(Document &obj, std::map<std::string, T> m) {
	typename std::map<std::string, T>::iterator beg = m.begin();
	typename std::map<std::string, T>::iterator end = m.end();
	for (; beg != end; ++beg) {
		json_addstring_member(obj, beg->first, beg->second);
	}
	return;
}

// 删除
// 删除对象键值为strkv的元素
// 删除数组值为strkv(重载 op == 的对象) 的第一个元素
#define json_arr_erase(value, strKV) {\
    Value::ConstValueIterator beg = value.Begin(); \
    Value::ConstValueIterator end = value.End(); \
    for(; beg != end; ++beg) { \
        if(*beg == strKV) { \
            value.Erase(beg); \
            break; \
        } \
    } \
}

#define json_obj_erase(value, strKV) {\
    if(value.IsObject()) {\
        value.EraseMember(strKV); \
    }\
}

// 删除所有相同值得元素
#define json_arr_erase_all(value, strKV) {\
   if(value.IsArray()) { \
       Value::ConstValueIterator beg = value.Begin(); \
       Value::ConstValueIterator end = value.End(); \
       for(; beg != value.End(); ) { \
           if(*beg == strKV) { \
               value.Erase(beg); \
           } else { \
               beg++; \
           } \
       } \
   }\
}

template<typename T>
void json_erase(Value &v, T strKV) {
	if (v.IsObject()) {
		json_obj_erase(v, strKV);
	}
	else {
		json_arr_erase_all(v, strKV);
	}
}

// 生成json
// 生成格式化json
#define json_encode_pretty(value) \
        ({ \
         rapidjson::StringBuffer sb; \
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb); \
        value.Accept(writer);  \
        std::string(sb.GetString(),sb.GetSize()); \
        }) 
#define json_encode_pretty_ref(value, str) \
        ({ \
         rapidjson::StringBuffer sb; \
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb); \
        value.Accept(writer);  \
        str.append(sb.GetString(),sb.GetSize()); \
        }) 



// 生成序列化json
#define json_encode(value) \
        ({ \
        rapidjson::StringBuffer sb; \
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb); \
        value.Accept(writer);  \
        std::string(sb.GetString(),sb.GetSize()); \
        }) 
#define json_encode_ref(value, str) \
        ({ \
        rapidjson::StringBuffer sb; \
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb); \
        value.Accept(writer);  \
        str.append(sb.GetString(),sb.GetSize()); \
        })   
