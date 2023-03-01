cmake_minimum_required(VERSION 3.19.0)

project(n_body)

add_compile_options(-O3 -std=c11)

add_executable(n_body main.c n_body.c)
target_link_libraries(n_body m)
