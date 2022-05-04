cmake_minimum_required(VERSION 3.13)

project(hw-10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(BIN_FILE "crc32")

add_compile_options(-Wall -Wextra -Wpedantic)
add_executable(${BIN_FILE} "src/main.c")
add_custom_target(valgrind_10
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./${BIN_FILE} test.txt
        )