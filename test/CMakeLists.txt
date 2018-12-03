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
					../boost_1_64_0/
					./redis/include/
					~/mtsProject/ext/mts_outer/
					src/SDK/include)

link_directories(../
				lib/
				../boost_1_64_0/stage/lib/
				./redis/lib/
				)


#aux_source_directory(src DIR_SRCS)

#add_subdirectory(mem)
add_subdirectory(src/SDK)

################################ A.so
set(ASRC src/A.cpp)
add_library(A SHARED ${ASRC})

################################ B.so
set(ALIBS A
	     )
set(BSRC src/B.cpp)
add_library(B SHARED ${BSRC})
target_link_libraries(B ${ALIBS})

################################ C.so

set(CSRC src/C.cpp)
add_library(C SHARED ${CSRC})
target_link_libraries(C ${ALIBS})

set(LIBS A
	    B
		C
		dl
		clientAPI
		boost_system 
		boost_thread
		boost_filesystem
		boost_program_options
		hiredis_vip
		crypto
		)


################################# jgbtest


MESSAGE(${PROJECT_SOURCE_DIR})

set(SRC src/main.cpp)

#add_library(${PROJECT_NAME} SHARED ${SRC})
add_executable(${PROJECT_NAME} ${SRC})
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGs "-fPIC")
target_link_libraries(${PROJECT_NAME} ${LIBS})

