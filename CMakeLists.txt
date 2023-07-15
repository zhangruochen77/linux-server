cmake_minimum_required(VERSION 3.0)
project(server)

# 设置 C++ 编译选项（可选）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

# 添加源文件
set(LIB_SRC 
    server/log/log.cc
    server/utils/utils.cc
    )

add_library(server SHARED ${LIB_SRC})

# 生成可执行文件
add_executable(test_log tests/test_log.cc)
add_dependencies(test_log server)
target_link_libraries(test_log server)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)