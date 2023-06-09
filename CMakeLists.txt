cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)

project(SO2_monkey_cooks)

# TU_DODAWANE_PLIKI_NAGLOWKOWE
include_directories(include)
include_directories(include/core)

add_subdirectory(src)
add_executable(SO2_monkey_cooks main.cpp include/core/MonkeUtility.hpp)

# KOPIOWANIE_FOLDER_RESOURCES (żeby nie trzeba było edytować w folderze build przy każdym debugowaniu)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources)
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
        DEPENDS ${CMAKE_SOURCE_DIR}/resources
        )
add_dependencies(SO2_monkey_cooks copy_resources)

target_link_libraries(SO2_monkey_cooks SO2)

