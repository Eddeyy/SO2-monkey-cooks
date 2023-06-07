file(GLOB_RECURSE SOURCE_FILES "*.cpp")
add_library(SO2 ${SOURCE_FILES})


# TU_DODAWANE_PLIKI_NAGLOWKOWE
target_include_directories(SO2 PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(SO2 PUBLIC ${PROJECT_SOURCE_DIR}/include/core)
