cmake_minimum_required(VERSION 3.5)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "/usr/bin/c++")

set(RCXX_FLAGS "$ENV{CXXFLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${RCXX_FLAGS} -o0 -g -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${RCXX_FLAGS} -o3 ")

add_compile_options(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)

project(jgbtest)
include_directories(./
					./include
					./include/tool/
					./redis/include
					../boost_1_64_0
					./src/SDK/include
					./src/SDK/include/rapidjson
					../rapidjson
					./src/mongodb
					
					/usr/local/include/bsoncxx/v_noabi/
					/usr/local/include/mongocxx/v_noabi/
					/home/jgb/mts_outer/cjson/include/
					./calendar/
					./mysql/
					/home/jgb/mts_outer/mysql/include
					./src/leetCode/)

link_directories(../
				./build/src/SDK/Debug/
				./redis/lib
				../boost_1_64_0/stage/lib
				/home/jgb/mts_outer/cjson/lib
				/home/jgb/mts_outer/mysql/lib/
				)


#aux_source_directory(src DIR_SRCS)

#add_subdirectory(mem)
add_subdirectory(./src/SDK)

set(LIBS curl
		clientAPI
		boost_program_options
		boost_system
		hiredis_vip
		ssl
		crypto
		mongocxx
		bsoncxx
		cjson
		mysqlclient
		)


################################# jgbtest

MESSAGE(${PROJECT_SOURCE_DIR})

set(SRC main.cpp
		./calendar/calendar.cpp
		./mysql/DBTimer.cpp
		./mysql/DBMySql.cpp
		./src/common/md5/my_md5.cpp
		./src/leetCode/leetCodeEasy.cpp)

#add_library(${PROJECT_NAME} SHARED ${SRC})
add_executable(${PROJECT_NAME} ${SRC})
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGs "-fPIC")
IF (WIN32)
	target_link_libraries(${PROJECT_NAME} ${LIBS} )
ELSEIF (UNIX)
	target_link_libraries(${PROJECT_NAME} ${LIBS} -lpthread)
ENDIF ()


