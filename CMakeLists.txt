cmake_minimum_required(VERSION 3.5)

project(cplusplus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# weird setting, but for protobuf head files found
set(protobuf_MODULE_COMPATIBLE 1)
find_package(Protobuf REQUIRED CONFIG)

if(NOT Protobuf_FOUND)
    message(FATAL_ERROR "Protobuf not found. Please install protobuf or set CMAKE_PREFIX_PATH")
endif()

message(STATUS "Using Protobuf ${Protobuf_VERSION}")
include_directories(${Protobuf_INCLUDE_DIRS})

add_executable(cplusplus main.cpp)

# ThirdParty
# add_subdirectory(ThirdParty/json)
add_subdirectory(ThirdParty/googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#find_package(nlohmann_json 3.2.0 REQUIRED)

# add_subdirectory(sgi-stl)
# add_subdirectory(misc)
add_subdirectory(ThreadPool)
# add_subdirectory(Json)
# add_subdirectory(Test)
add_subdirectory(Exam)
add_subdirectory(tests)
add_subdirectory(proto)
