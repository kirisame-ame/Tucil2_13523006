cmake_minimum_required(VERSION 3.16...4.0)
project(Tucil2_13523006 VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
add_subdirectory(src)

add_executable(Main src/main.cpp)
target_link_libraries(Main PRIVATE ImageCompressorLib)