cmake_minimum_required(VERSION 3.8.0)
project(Aheuiplusplus CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(INCLUDE_DIR "./include")
set(SOURCE_DIR "./src")
set(OUTPUT_DIR "./bin")
set(EXTLIB_DIR "./extlib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Cache
set(COMPILE_TARGET "Executable" CACHE STRING "아희++을 어떤 형태로 컴파일 할 지 설정합니다.")
set_property(CACHE COMPILE_TARGET PROPERTY STRINGS "Executable" "Library")
string(TOLOWER ${COMPILE_TARGET} COMPILE_TARGET)

# Searching files
## Header files
include_directories(${INCLUDE_DIR})
include_directories(${EXTLIB_DIR}/u5e/include)

## Source files
file(GLOB SOURCE_LIST ${SOURCE_DIR}/*.cpp)
file(GLOB EXTLIB_U5E_SOURCE_LIST ${EXTLIB_DIR}/u5e/src/u5e/*.cpp ${EXTLIB_DIR}/u5e/src/u5e/props/*.cpp)

# Compilation
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-D__STDC_LIMIT_MACROS)

if(${COMPILE_TARGET} STREQUAL "executable" OR ${COMPILE_TARGET} STREQUAL "e")
	add_definitions(-DAHEUIPLUSPLUS_TARGET=1)

	add_executable(${PROJECT_NAME} ${SOURCE_LIST} ${EXTLIB_U5E_SOURCE_LIST})
elseif(${COMPILE_TARGET} STREQUAL "library" OR ${COMPILE_TARGET} STREQUAL "l")
	add_definitions(-DAHEUIPLUSPLUS_TARGET=2)

	add_library(${PROJECT_NAME} STATIC ${SOURCE_LIST} ${EXTLIB_U5E_SOURCE_LIST})
endif(${COMPILE_TARGET} STREQUAL "executable" OR ${COMPILE_TARGET} STREQUAL "e")

# Installation
if(${COMPILE_TARGET} STREQUAL "executable" OR ${COMPILE_TARGET} STREQUAL "e")
	install(TARGETS ${PROJECT_NAME} DESTINATION bin)
elseif(${COMPILE_TARGET} STREQUAL "library" OR ${COMPILE_TARGET} STREQUAL "l")
	install(TARGETS ${PROJECT_NAME} DESTINATION lib)

	## Header files
	file(GLOB HEADER_LIST ${INCLUDE_DIR}/Aheuiplusplus/*.hpp)
	file(GLOB EXTLIB_U5E_HEADER_LIST ${EXTLIB_DIR}/u5e/include/u5e/*.hpp)
	file(GLOB EXTLIB_U5E_PROPS_HEADER_LIST ${EXTLIB_DIR}/u5e/include/u5e/props/*.hpp)
	file(GLOB EXTLIB_UTF8_CPP_HEADER_LIST ${EXTLIB_DIR}/UTF8-CPP/include/utf8/*.h)

	install(FILES ${HEADER_LIST} DESTINATION include/Aheuiplusplus)
	install(FILES ${EXTLIB_U5E_HEADER_LIST} DESTINATION include/u5e)
	install(FILES ${EXTLIB_U5E_PROPS_HEADER_LIST} DESTINATION include/u5e/props)
	install(FILES ${EXTLIB_UTF8_CPP_HEADER_LIST} DESTINATION include/utf8)
endif(${COMPILE_TARGET} STREQUAL "executable" OR ${COMPILE_TARGET} STREQUAL "e")