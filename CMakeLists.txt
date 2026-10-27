cmake_minimum_required(VERSION 3.16)
project(demo1)
set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR D:/opencv/opencv/build/x64/vc16/lib)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(demo src/project2.cpp)
target_link_libraries(demo ${OpenCV_LIBS})

# 自动把依赖的 OpenCV dll 拷贝到可执行文件所在目录
add_custom_command(TARGET demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "D:/opencv/opencv/build/x64/vc16/bin/opencv_world500d.dll"
    "D:/opencv/opencv/build/x64/vc16/bin/opencv_videoio_msmf500_64d.dll"
    $<TARGET_FILE_DIR:demo>
)
