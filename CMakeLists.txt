cmake_minimum_required(VERSION 3.1)
project(UVC_Video_Capture)

set(CMAKE_C_STANDARD 99)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(UVC_Video_Capture main.c usbinfo.c)
target_link_libraries(UVC_Video_Capture -ludev -lusb-1.0 -lv4l2-hw -lv4l2-extra-hw)