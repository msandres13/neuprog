cmake_minimum_required (VERSION 2.6)

project ( neuprog )

set (CMAKE_CXX_STANDARD 11)
#find_package(Threads REQUIRED)

if ( CMAKE_COMPILER_IS_GNUCC )
#    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long")
endif()


#include_directories( /usr/include/mysql )
#add_definitions(-DDEBUGOUTFILENAME="/var/log/meter")

add_executable( ${PROJECT_NAME}
	neuprog.cpp
)

target_link_libraries (${PROJECT_NAME} ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} pigpio rt)

