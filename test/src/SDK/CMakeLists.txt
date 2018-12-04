cmake_minimum_required(VERSION 3.5)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "/usr/bin/c++")

set(RCXX_FLAGS "$ENV{CXXFLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${RCXX_FLAGS} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${RCXX_FLAGS}  ")

add_compile_options(-std=c++11)


include_directories(include
					include/rapidjson)

link_directories(.
				./build/Debug
				)

set(LIBS 
		ClientApi
		)

################################ ClientApi.so

set(CSRAPI src/clientAPI.cpp
			src/clientMsg.cpp
			src/md5.cpp)
add_library(clientAPI SHARED ${CSRAPI})
#target_link_libraries(clientAPI ${LIBS})
