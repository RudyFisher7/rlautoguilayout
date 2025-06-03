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
    message(STATUS "${PROJECT_NAME}: fetch raylib.")
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 5.5
            GIT_PROGRESS TRUE
            GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(raylib)
    FetchContent_GetProperties(raylib)
    message(STATUS "${CMAKE_MODULE_PATH}")
else()
    message(STATUS "${PROJECT_NAME}: raylib already available.")
endif()


set(SOURCES
        source/tree.c
        source/draw_functions.c
)

set(HEADERS
        include/rlauto_layout/tree.h
        include/rlauto_layout/type_defs.h
        include/rlauto_layout/draw_functions.h
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        raylib
)
