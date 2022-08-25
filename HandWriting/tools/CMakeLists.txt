cmake_minimum_required(VERSION 3.5.0)

project(tools)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(knntt knntt.cpp)
target_link_libraries(knntt ${OpenCV_LIBS})

add_executable(svmtt svmtt.cpp)
target_link_libraries(svmtt ${OpenCV_LIBS})