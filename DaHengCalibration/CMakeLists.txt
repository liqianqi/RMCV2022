# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10) #old
project(DaHengCalibration)

# 库文件的放置目录
set(OpenCV_DIR /home/liqianqi/opencv-3.4.16/build)

#set(Torch_DIR /home/yeahoo/libtorch/share/cmake/Torch)#new
set(SDK_DIR /home/liqianqi/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.2.2104.9022/Galaxy_camera/lib/x86_64/libgxiapi.so)

# C++11 标准、o3优化、多线程
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -pthread")

find_package(OpenCV REQUIRED)

#include_directories($(OpenCV_INCLUD_DIRS)) #old

include_directories("/usr/include/eigen3")#new
include_directories(${OpenCV_INCLUDE_DIRS} )

#PROJECT_SOURCE_DIR     CMAKE_CURRENT_SOURCE_DIR

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(./src SRC_FILES)

## 指定生成目标
add_executable(${PROJECT_NAME}  ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})#new
target_link_libraries(${PROJECT_NAME} ${SDK_DIR})


