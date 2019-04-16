cmake_minimum_required(VERSION 3.13)
project(wb)

set(CMAKE_CXX_STANDARD 11)
find_package(Boost 1.58 COMPONENTS system regex  filesystem REQUIRED)
find_package( OpenCV REQUIRED )

add_executable(wb main.cpp usbinfo.c video_ext.h UvcH264.h libv4l2-hw.h VideoCapture.cpp)
target_link_libraries(wb usb-1.0 -ludev -pthread ${Boost_LIBRARIES})
