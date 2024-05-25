add_library(zfunc src/zfunc.cpp include/zfunc.h)

target_include_directories(zfunc PUBLIC include)

add_executable(lab4 main.cpp)
target_link_libraries(lab4 zfunc)

add_executable(lab4_benchmark benchmark.cpp)
target_link_libraries(lab4_benchmark zfunc)

add_executable(lab4_contest single_file.cpp)