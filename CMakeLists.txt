cmake_minimum_required(VERSION 3.24)            #cmake最低版本要求

project(ZhengLi)                                #设置项目名称（也可以先声明项目变量，然后填入变量）
aux_source_directory(. DIR_SRCS)                #将当前目录下的所有源文件的名字保存到变量DIR_SRCS
add_executable(ZhengLi ${DIR_SRCS})             #为整个项目导入Boost库

set(CMAKE_CXX_STANDARD 14)                      #设置编译版本

set(BOOST_ROOT D:/Boost)                        #设置Boost库根目录变量
set(Boost_INCLUDE_DIR D:/Boost)                 #设置头文件搜索路径变量（系统将在D:\Boost\boost中搜索）
set(Boost_LIBRARY_DIR D:/Boost/vc_14_3/lib)     #设置库文件搜索路径变量
set(Boost_USE_SHARED_LIBS ON)                   #启用动态库
set(Boost_USE_SHARED_RUNTIME ON)                #启用动态链接
set(Boost_USE_MULTITHREAD ON)                   #启用多线程

include_directories(${Boost_INCLUDE_DIR})       #头文件搜索路径
link_directories(${Boost_LIBRARY_DIR})          #库文件搜索路径
target_link_libraries(ZhengLi boost_thread boost_system})

add_executable(t1 7_6.cpp)
add_executable(t2 7_8.cpp)
add_executable(t3 mySort.cpp)
add_executable(t4 IncrementIterator.cpp)
add_executable(t5 getElapsedTime.cpp)
add_executable(t10_17_2and3 10_18.cpp)
add_executable(t10_17_4 10_10.cpp)
add_executable(t11_11 IBAM.cpp account.cpp date.cpp)
add_executable(c3 Chapter3.cpp)
add_executable(c4 Chapter4.cpp)
add_executable(c5 Chapter5.cpp)
add_executable(c6 Chapter6.cpp)
add_executable(c7 Chapter7.cpp)
add_executable(c8 Chapter8.cpp)
add_executable(c9 Chapter9.cpp)
add_executable(c9_5 9_5answer.cpp)
add_executable(c9_6 9_6answer.cpp)
add_executable(c10 Chapter10.cpp)
add_executable(c11 Chapter11.cpp)
add_executable(c12 Chapter12.cpp)