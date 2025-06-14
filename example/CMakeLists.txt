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

cmake_minimum_required(VERSION 3.26)
project(rlautolayoutexample)


set(RESOURCE_DIR
        "${CMAKE_SOURCE_DIR}/resource"
)

add_definitions(-DRESOURCE_DIR=\"${RESOURCE_DIR}\")

set(LIBS
        rlautolayout
)


#set(CENTERED_MENU_BUTTONS_EXAMPLE rlautolayoutcenteredmenubuttonsexample)
#
#add_executable(${CENTERED_MENU_BUTTONS_EXAMPLE}
#        centered_menu_buttons_example.c
#)
#
#target_link_libraries(${CENTERED_MENU_BUTTONS_EXAMPLE} PUBLIC
#        ${LIBS}
#)


set(APPLICATION_GUI_EXAMPLE rlautolayoutapplicationguiexample)

add_executable(${APPLICATION_GUI_EXAMPLE}
        application_gui_example.c
)

target_link_libraries(${APPLICATION_GUI_EXAMPLE} PUBLIC
        ${LIBS}
)

add_subdirectory(raygui)