add_library(suffix_tree src/suffix_tree.cpp include/suffix_tree.h)

target_include_directories(suffix_tree PUBLIC include)

add_executable(lab5 main.cpp)
target_link_libraries(lab5 suffix_tree)

add_executable(lab5_contest single_file.cpp)