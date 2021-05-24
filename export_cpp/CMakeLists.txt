# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (export_cpp_cfg CXX)
SET(PRJ_NAME "export_cpp_cfg")

configure_file("version.h.in" "${PROJECT_SOURCE_DIR}/version.h")

# include directories
INCLUDE_DIRECTORIES(after  ${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(after  "../com/")


# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
file(GLOB_RECURSE SRCS  ${PROJECT_SOURCE_DIR} "*.cc")
file(GLOB_RECURSE SRCS2  ${PROJECT_SOURCE_DIR} "*.cpp"
"../com/SimpleCfg.cpp"
)

# 指定生成目标
ADD_EXECUTABLE(${PRJ_NAME} ${SRCS} ${SRCS2})

#设置依赖
ADD_DEPENDENCIES(${PRJ_NAME} lib_prj)

#增加系统依赖库
ADD_library(pthread SHARED IMPORTED)
ADD_library(crypto SHARED IMPORTED)

# 设置链接库
target_link_libraries(${PRJ_NAME} dl)

#cotire设置gcc的预编译头相关			
#set_target_properties(${PRJ_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
#cotire(${PRJ_NAME})

