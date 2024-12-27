add_library(pers_segtree INTERFACE)

target_include_directories(pers_segtree INTERFACE include)

add_executable(cp main.cpp)

target_link_libraries(cp PRIVATE pers_segtree)