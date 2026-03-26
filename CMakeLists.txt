cmake_minimum_required(VERSION 3.16)

project(huffman_archiver C)
enable_testing()

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_compile_options(
    -Wall
    -Wextra
    -O2
    -fsanitize=address
)

add_link_options(-fsanitize=address)

set(SRC
    src/huffman_tree.c
    src/huffman_codec.c
    src/bitio.c
)

add_library(huffman ${SRC})

add_executable(huff src/main.c)
target_link_libraries(huff huffman)


file(GLOB TEST_SRC tests/unit/test_*.c)

foreach(test_file ${TEST_SRC})
    get_filename_component(test_name ${test_file} NAME_WE)

    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name} huffman)

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

add_custom_target(run_tests
    COMMAND bash ${CMAKE_SOURCE_DIR}/tests/test.sh
    DEPENDS huff
)

add_custom_target(unit_tests
    COMMAND ${CMAKE_COMMAND} -E make_directory tests/tmp
    COMMAND ${CMAKE_COMMAND} --build . --target ${TEST_SRC}
)
