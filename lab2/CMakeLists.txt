add_executable(lab2 main.cpp)

add_library(patricia src/patricia.cpp include/patricia.h
            src/binary_string.cpp include/binary_string.h)
target_include_directories(patricia PUBLIC include)
target_link_libraries(lab2 patricia)