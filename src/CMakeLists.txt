cmake_minimum_required(VERSION 3.14)
project(pgw_sim)


set(BOOST_ROOT "$ENV{HOME}/boost_1_87_0") 
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_NO_BOOST_CMAKE ON)

find_package(Boost 1.87 REQUIRED)

add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_ASIO_NO_DEPRECATED)

set(RUNNABLE ${CMAKE_PROJECT_NAME})
set(OBJ_LIB "${CMAKE_PROJECT_NAME}_lib")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_library(${OBJ_LIB} OBJECT ${SOURCES})

target_include_directories(${OBJ_LIB} PUBLIC 
    ${Boost_INCLUDE_DIRS}
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_compile_options(${OBJ_LIB} PUBLIC 
    "-Werror" "-Wall" "-Wextra" "-Wpedantic" "-Wno-error=maybe-uninitialized"
)

add_executable(${RUNNABLE} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
target_link_libraries(${RUNNABLE} ${OBJ_LIB})