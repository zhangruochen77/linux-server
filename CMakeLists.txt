# 设置项目使用 cmake 版本以及项目名称
cmake_minimum_required(VERSION 3.0)
project(server)
add_compile_options(-fPIC)#添加这句编译选项

# 设置版本 c++11 以及强制绑定 c++11版本
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

# 添加依赖头文件位置 需要进行扫描的头文件位置
include_directories(.)
include_directories(/usr/local/include)

# 导入动态库所在位置
# ubuntu
# link_libraries(" /usr/local/lib/libyaml-cpp.so")

# centos
 link_libraries(" /usr/local/lib64/libyaml-cpp.so")

# 添加源文件 设定源文件目标集合位置
set(LIB_SRC 
    server/log/log.cc
    server/utils/utils.cc
    server/config/config.cc
    )

set(LIBS
    server 
    yaml-cpp
    )

# 添加共享库
add_library(server SHARED ${LIB_SRC})
# force_redefine_file_macro_for_sources(server)

# 生成可执行文件 
# add_executable( target sourceFile)
# add_dependencies( target dependeceFiles)
# target_link_libraries( target dependeceFiles)
# log_test
add_executable(test_log tests/test_log.cc)
add_dependencies(test_log server)
target_link_libraries(test_log server)

# config_test
add_executable(test_config tests/test_config.cc)
add_dependencies(test_config server libyaml-cpp.so)
target_link_libraries(test_config server libyaml-cpp.so)

# 设置库文件以及可执行文件到 /bin 目录下
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)