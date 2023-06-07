cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)

project(SO2_monkey_cooks)

# TU_DODAWANE_PLIKI_NAGLOWKOWE
include_directories(include)
include_directories(include/core)

add_subdirectory(src)
add_executable(SO2_monkey_cooks main.cpp)

target_link_libraries(SO2_monkey_cooks SO2)

