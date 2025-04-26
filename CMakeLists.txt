cmake_minimum_required(VERSION 3.5)

project(cplusplus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cplusplus main.cpp)

# ThirdParty
# add_subdirectory(ThirdParty/json)
add_subdirectory(ThirdParty/googletest)

#find_package(nlohmann_json 3.2.0 REQUIRED)

# add_subdirectory(sgi-stl)
# add_subdirectory(misc)
add_subdirectory(ThreadPool)
# add_subdirectory(Json)
# add_subdirectory(Test)
add_subdirectory(Exam)

