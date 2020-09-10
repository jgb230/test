cmake_minimum_required(VERSION 3.5)

set(obj aliyun)
include_directories(../)
include_directories(./)
include_directories($ENV{MTS_R_OUTER}/cjson/include/)
include_directories($ENV{PRJ_ROOT_DIR}/src/services/include/serviceImg/)
link_directories($ENV{MTS_R_OUTER}/cjson/lib/)
include_directories($ENV{MTS_R_OUTER}/curl/include/)
link_directories($ENV{MTS_R_OUTER}/curl/lib/)
include_directories($ENV{MTS_R_OUTER}/)
set(srcfiles ./CAsrTtsAliyun.cpp 
            ../CAsrTtsBase.cpp 
            ../tengxunyun/encrypt.cpp)
SET(LIBRARY_OUTPUT_PATH ../../../../../../lib/services)

# 生成链接库
#add_executable (${obj}  ${srcfiles})
add_library (${obj} SHARED ${srcfiles})
target_link_libraries(${obj}  curl crypto)
#target_link_libraries(${obj} pthread curl)