cmake_minimum_required(VERSION 3.10)
project(protobuf_demo)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加protobuf的头文件目录
include_directories(${Protobuf_INCLUDE_DIRS})

message(STATUS "Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")


# 创建可执行文件
add_executable(writer writer.cpp)
add_executable(reader reader.cpp)

# 使用新的方式生成protobuf文件
protobuf_generate(TARGET writer
    PROTOS addressbook.proto
    LANGUAGE cpp
    GENERATE_EXTENSIONS .pb.h .pb.cc
    PLUGIN "protoc-gen-cpp=${Protobuf_PROTOC_EXECUTABLE}"
)

protobuf_generate(TARGET reader
    PROTOS addressbook.proto
    LANGUAGE cpp
    GENERATE_EXTENSIONS .pb.h .pb.cc
    PLUGIN "protoc-gen-cpp=${Protobuf_PROTOC_EXECUTABLE}"
)

# 为每个目标设置include路径
target_include_directories(writer PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

target_include_directories(reader PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

message(STATUS "Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")
# 链接protobuf库
# 新版本不要使用 ${Protobuf_LIBRARIES} 而是使用 protobuf::libprotobuf
target_link_libraries(writer PRIVATE protobuf::libprotobuf)
target_link_libraries(reader PRIVATE protobuf::libprotobuf)