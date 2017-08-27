project(TEST)

add_library(basic basic.c)
add_library(search search.c)
add_library(statistics statistics.c)

add_executable(wsserver wsserver.c)

target_link_libraries(wsserver basic search statistics crypto)