cmake_minimum_required(VERSION 3.26)
project(video_to_ascii)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(SFML COMPONENTS audio REQUIRED)

include_directories(.)
include_directories(VideoToAscii)

add_executable(video_to_ascii
    src/utility/cmd.cpp
    src/utility/cmd.h
    src/utility/colors.cpp
    src/utility/colors.h
    src/utility/youtube.h
    src/utility/youtube.cpp
    src/structs.h
    src/main.cpp

    src/modules/audiorw/audiorw.h
    src/modules/audiorw/read.cpp
    src/modules/audiorw/write.cpp
)

target_link_libraries(video_to_ascii PRIVATE opencv_ml opencv_dnn opencv_core opencv_flann sfml-audio libssl libcrypto)
