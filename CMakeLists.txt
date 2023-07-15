# 设置项目使用 cmake 版本以及项目名称
cmake_minimum_required(VERSION 3.0)
project(server)

# 设置版本 c++11 以及强制绑定 c++11版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加依赖头文件位置 需要进行扫描的头文件位置
include_directories(.)

# 添加源文件 设定源文件目标集合位置
set(LIB_SRC 
    server/log/log.cc
    server/utils/utils.cc
    )

# 添加共享库
add_library(server SHARED ${LIB_SRC})

# 生成可执行文件 
# add_executable( target sourceFile)
# add_dependencies( target dependeceFiles)
# target_link_libraries( target dependeceFiles)
add_executable(test_log tests/test_log.cc)
add_dependencies(test_log server)
target_link_libraries(test_log server)

# 设置库文件以及可执行文件到 /bin 目录下
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)