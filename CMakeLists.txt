cmake_minimum_required(VERSION 3.26)
project(video_to_ascii)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV CONFIG REQUIRED)
find_package(portaudio CONFIG REQUIRED)

include_directories(.)
include_directories(VideoToAscii)

add_executable(video_to_ascii
    cmdutils.cpp
    cmdutils.hpp
    colorUtil.cpp
    colorUtil.hpp
    main.cpp
    Structs.h
)

target_link_libraries(video_to_ascii PRIVATE opencv_ml opencv_dnn opencv_core opencv_flann)
