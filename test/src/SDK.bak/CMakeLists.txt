cmake_minimum_required(VERSION 3.5)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "/usr/bin/c++")

set(RCXX_FLAGS "$ENV{CXXFLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${RCXX_FLAGS} -o0 -g -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${RCXX_FLAGS} -o3 ")

add_compile_options(-std=c++11)

include_directories(include
					~/mtsProject/ext/mts_outer/)

link_directories(.
				)

set(LIBS 
		)

################################ ClientApi.so

set(CSRAPI src/clientAPI.cpp
			src/clientMsg.cpp
			src/md5.cpp)
add_library(clientAPI SHARED ${CSRAPI})
target_link_libraries(clientAPI ${LIBS})