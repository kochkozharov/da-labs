add_executable(lab1 src/main.cpp)
target_include_directories(lab1 PRIVATE include)

add_executable(lab1_benchmark src/benchmark.cpp)
target_include_directories(lab1_benchmark PRIVATE include)
