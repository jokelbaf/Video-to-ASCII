cmake_minimum_required(VERSION 3.26)
project(video_to_ascii)

set(CMAKE_CXX_STANDARD 23)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")
set(VCPKG_HOST_TRIPLET "x64-windows-static")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    message(${CMAKE_CURRENT_SOURCE_DIR}/windows/app.rc)
    set (WIN_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/windows/app.rc)
endif()

find_package(OpenSSL REQUIRED)
find_package(FFmpeg REQUIRED)
find_package(httplib REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED)
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

        ${WIN_RESOURCES}
)

if (MSVC)
    target_compile_options(video_to_ascii PRIVATE /MT)
endif ()

target_link_libraries(video_to_ascii PRIVATE
        ${FFMPEG_LIBRARIES}
        sfml-audio
        httplib::httplib
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        OpenSSL::Crypto
        opencv_videoio
)
