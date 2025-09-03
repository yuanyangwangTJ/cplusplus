cmake_minimum_required(VERSION 3.10)

project(cplusplus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cplusplus main.cpp)

# ThirdParty
# add_subdirectory(ThirdParty/json)
add_subdirectory(ThirdParty/googletest)
add_subdirectory(ThirdParty/benchmark)

set(BOOST_INCLUDE_LIBRARIES filesystem regex container
    coroutine2 fiber)
add_subdirectory(ThirdParty/boost-1.88.0 EXCLUDE_FROM_ALL)

enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "Force gtest to use shared CRT" FORCE)
include(GoogleTest)

#find_package(nlohmann_json 3.2.0 REQUIRED)

# add_subdirectory(sgi-stl)
# add_subdirectory(misc)
# add_subdirectory(ThreadPool)
# add_subdirectory(Json)
add_subdirectory(Test)
# add_subdirectory(Exam)
add_subdirectory(Boost-Learn)
add_subdirectory(MyStructure)
add_subdirectory(CppTool-Learn)
add_subdirectory(Cpp-Advance)
add_subdirectory(Exam-Autumn)
add_subdirectory(Algorithm)