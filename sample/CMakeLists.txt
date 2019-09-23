#project name
CMAKE_MINIMUM_REQUIRED(VERSION 2.8) #cmake最低版本需求，不加入此行会受到警告信息
PROJECT(Sample) #项目名称　　

#INCLUDE_DIRECTORIES(system ${PROJECT_SOURCE_DIR}/../lib)
#
## lib directories
LINK_DIRECTORIES(

  #系统目录
  /usr/local/lib
  /use/lib
)
INCLUDE_DIRECTORIES(after  "../include/")

# 源文件搜索
FILE(GLOB_RECURSE SourceFiles 
        "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/*.cc") 
        
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -pthread")
ADD_EXECUTABLE(hello ${SourceFiles}) 

