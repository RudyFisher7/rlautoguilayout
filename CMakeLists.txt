# MIT License
#
# Copyright (c) 2025 Rudy Fisher (kiyasui-hito)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.10)
project(rlautolayout)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Enable unit tests" ON)
option(ENABLE_EXAMPLES "Enable examples" ON)

if(NOT DEFINED RLAUTO_LAYOUT_TREE_CAPACITY)
    set(RLAUTO_LAYOUT_TREE_CAPACITY 256)
    message(STATUS "Configuring without -DRLAUTO_LAYOUT_TREE_CAPACITY=<int>. Setting to ${RLAUTO_LAYOUT_TREE_CAPACITY} as default.")
endif()

if(NOT DEFINED RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE)
    set(RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE 0)
    message(STATUS "Configuring without -DRLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE=<1 or 0>. Setting to ${RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE} as default.")
endif()

if(ENABLE_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()


if(ENABLE_EXAMPLES)
    add_subdirectory(example)
endif()


Include(FetchContent)

if(NOT raylib_POPULATED)
    message(STATUS "${PROJECT_NAME}: fetching raylib...")
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 5.5
            GIT_PROGRESS TRUE
            GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(raylib)
    FetchContent_GetProperties(raylib)
else()
    message(STATUS "${PROJECT_NAME}: raylib already available.")
endif()


if(NOT raygui_POPULATED)
    message(STATUS "${PROJECT_NAME}: fetching raygui...")
    FetchContent_Declare(
            raygui
            GIT_REPOSITORY https://github.com/raysan5/raygui.git
            GIT_TAG 4.0
            GIT_PROGRESS TRUE
            GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(raygui)
    FetchContent_GetProperties(raygui)

    # the above commands don't seem to work with raygui for some reason.
    # add the CMake project directory directly for now.
    add_subdirectory("${CMAKE_BINARY_DIR}/_deps/raygui-src/projects/CMake")
    set(RAYGUI_STYLES_DIR "${CMAKE_BINARY_DIR}/_deps/raygui-src/examples/styles")
else()
    message(STATUS "${PROJECT_NAME}: raygui already available.")
endif()


set(LIBS
        raylib
        raygui
)

set(SOURCES
        source/tree.c
        source/raylib_draw_functions.c
        source/raygui_draw_functions.c
)

set(HEADERS
        include/rlauto_layout/tree.h
        include/rlauto_layout/type_defs.h
        include/rlauto_layout/raylib_draw_functions.h
        include/rlauto_layout/raygui_draw_functions.h
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        ${RAYGUI_STYLES_DIR}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        RLAUTO_LAYOUT_TREE_CAPACITY=${RLAUTO_LAYOUT_TREE_CAPACITY}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${LIBS}
)
