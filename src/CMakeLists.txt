project(TEST)

add_library(basic basic.c)

add_executable(test test.c)

target_link_libraries(test basic)