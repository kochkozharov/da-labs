add_library(patricia src/patricia.cpp include/patricia.h
                     src/binary_string.cpp include/binary_string.h)
target_include_directories(patricia PUBLIC include)

add_executable(lab2 main.cpp)
target_link_libraries(lab2 patricia)

add_executable(lab2_benchmark benchmark.cpp)
target_link_libraries(lab2_benchmark patricia)

add_executable(lab2_new_benchmark new_benchmark.cpp)
target_link_libraries(lab2_new_benchmark patricia)

add_executable(lab2_tests tests/unit/binary_string_test.cpp tests/unit/patricia_test.cpp)
target_link_libraries(lab2_tests patricia gtest gtest_main)

add_executable(lab2_contest src/single_file.cpp)