add_executable(lab9 main.cpp)
add_executable(lab9_bench benchmark.cpp)

add_library(graph INTERFACE)
target_include_directories(graph INTERFACE include)

target_link_libraries(lab9 PRIVATE graph)
target_link_libraries(lab9_bench PRIVATE graph)
