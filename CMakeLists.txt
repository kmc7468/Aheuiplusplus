cmake_minimum_required(VERSION 3.8.0)
project(Aheuiplusplus CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR "./include")
set(SOURCE_DIR "./src")
set(OUTPUT_DIR "./bin")

include_directories(${INCLUDE_DIR})
file(GLOB SOURCE_LIST ${SOURCE_DIR}/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-D__STDC_LIMIT_MACROS)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)