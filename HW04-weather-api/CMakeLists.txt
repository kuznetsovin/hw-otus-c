cmake_minimum_required(VERSION 3.13)

project(hw-04)

link_directories(../assets/cJSON)
add_compile_options(-Wall -Wextra -Wpedantic)
add_executable(weather "src/main.c" "../assets/cJSON/cJSON.c")
target_link_libraries(weather curl)