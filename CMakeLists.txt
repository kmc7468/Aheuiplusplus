cmake_minimum_required(VERSION 3.8.0)
project(Aheuiplusplus CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(INCLUDE_DIR "./include")
set(SOURCE_DIR "./src")
set(OUTPUT_DIR "./bin")

set(COMPILE_TARGET "실행 파일" CACHE STRING "아희++을 어떤 형태로 컴파일 할 지 설정합니다.")

set_property(CACHE COMPILE_TARGET PROPERTY STRINGS "실행 파일" "정적 라이브러리")

include_directories(${INCLUDE_DIR})
file(GLOB SOURCE_LIST ${SOURCE_DIR}/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-D__STDC_LIMIT_MACROS)

if(${COMPILE_TARGET} STREQUAL "실행 파일")
	add_definitions(-DAHEUIPLUSPLUS_TARGET=1)

	add_executable(${PROJECT_NAME} ${SOURCE_LIST})
elseif(${COMPILE_TARGET} STREQUAL "정적 라이브러리")
	add_definitions(-DAHEUIPLUSPLUS_TARGET=2)

	add_library(${PROJECT_NAME} STATIC ${SOURCE_LIST})
endif(${COMPILE_TARGET} STREQUAL "실행 파일")


if(${COMPILE_TARGET} STREQUAL "실행 파일")
	install(TARGETS ${PROJECT_NAME} DESTINATION bin)
elseif(${COMPILE_TARGET} STREQUAL "정적 라이브러리")
	file(GLOB HEADER_LIST ${INCLUDE_DIR}/Aheuiplusplus/*.hpp)

	install(FILES ${HEADER_LIST} DESTINATION include/Aheuiplusplus)
	install(TARGETS ${PROJECT_NAME} DESTINATION lib)
endif(${COMPILE_TARGET} STREQUAL "실행 파일")