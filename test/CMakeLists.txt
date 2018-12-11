cmake_minimum_required(VERSION 3.5)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "/usr/bin/c++")

set(RCXX_FLAGS "$ENV{CXXFLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${RCXX_FLAGS} -o0 -g -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${RCXX_FLAGS} -o3 ")

add_compile_options(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)

project(jgbtest)
include_directories(include
					./src/SDK/include
					./src/SDK/include/rapidjson)

link_directories(../
				./build/src/SDK/Debug/
				)


#aux_source_directory(src DIR_SRCS)

#add_subdirectory(mem)
add_subdirectory(./src/SDK)

set(LIBS
		clientAPI
		)


################################# jgbtest

MESSAGE(${PROJECT_SOURCE_DIR})

set(SRC main.cpp)

#add_library(${PROJECT_NAME} SHARED ${SRC})
add_executable(${PROJECT_NAME} ${SRC})
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGs "-fPIC")
IF (WIN32)
	target_link_libraries(${PROJECT_NAME} ${LIBS} )
ELSEIF (UNIX)
	target_link_libraries(${PROJECT_NAME} ${LIBS} -lpthread)
ENDIF ()


