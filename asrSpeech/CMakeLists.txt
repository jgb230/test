cmake_minimum_required(VERSION 3.5)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "/usr/bin/c++")

set(RCXX_FLAGS "$ENV{CXXFLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${RCXX_FLAGS} -o0 -g -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${RCXX_FLAGS} -o3 ")

set(CMAKE_BUILD_TYPE Debug)

project(speechAsr)
include_directories(include
					/home/jgb/mts_outer/boost_1_64_0/)

link_directories(../
				/home/jgb/mts_outer/boost_1_64_0/stage/lib)


aux_source_directory(src DIR_SRCS)

set(LIBS boost_thread
	     boost_system
	     boost_filesystem)

set(SRC ${DIR_SRCS})
#add_library(${PROJECT_NAME} SHARED ${SRC})
add_executable(${PROJECT_NAME} ${SRC})
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGs "-fPIC")
target_link_libraries(${PROJECT_NAME} ${LIBS})
