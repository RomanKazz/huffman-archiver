cmake_minimum_required(VERSION 3.16)

project(huffman_archiver C)

enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" ON)

function(enable_project_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4)
        target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -O2)
    endif()
endfunction()

function(enable_project_sanitizers target)
    if (ENABLE_SANITIZERS AND CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()

add_library(huffman
    src/huffman_tree.c
    src/huffman_codec.c
    src/bitio.c
)

target_include_directories(huffman PUBLIC include)
enable_project_warnings(huffman)
enable_project_sanitizers(huffman)

add_executable(huff src/main.c)
target_link_libraries(huff PRIVATE huffman)
enable_project_warnings(huff)
enable_project_sanitizers(huff)

file(GLOB TEST_SRC CONFIGURE_DEPENDS tests/unit/test_*.c)

set(TEST_TARGETS)
foreach(test_file ${TEST_SRC})
    get_filename_component(test_name ${test_file} NAME_WE)

    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PRIVATE huffman)
    enable_project_warnings(${test_name})
    enable_project_sanitizers(${test_name})

    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    list(APPEND TEST_TARGETS ${test_name})
endforeach()

add_custom_target(unit_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ${TEST_TARGETS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(run_tests
    COMMAND bash ${CMAKE_SOURCE_DIR}/tests/test.sh
    DEPENDS huff
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
