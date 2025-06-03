/*
 * MIT License
 *
 * Copyright (c) 2025 Rudy Fisher (kiyasui-hito)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include "rlauto_layout/tree.h"
#include <raylib.h>


int main() {
    char format_string_buffer[48];

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Raylib + RayGUI Example");

    GUI_ROOT {
        GUI_VBOX {
            SetSizeFlagsBoth(SIZE_FLAGS_GROW);

            GUI_HBOX {
                SetSizeFlagX(SIZE_FLAGS_GROW);
                SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_CENTER, CHILD_ALIGNMENT_END});
                SetChildSpacing(16.0f);
                SetPaddingAll(4.0f);
//                SetDrawFunc(build_raylib_draw_rectangle(&DrawRectangle, SKYBLUE));

                const int navigationLinks = 4;
                int navLinks0[navigationLinks] = {};
                for (int i = 0; i < navigationLinks; ++i) {
                    GUI {
                        SetSizeFlagsBoth(SIZE_FLAGS_FIXED);
                        SetSize((Vector2){120.0f, 16.0f});
                    }
                }
            }

            GUI_HBOX {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                GUI_VBOX {
                    SetSizeFlagX(SIZE_FLAGS_GROW);
                    SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_CENTER, CHILD_ALIGNMENT_END});
                    SetChildSpacing(16.0f);
                    SetPaddingAll(4.0f);
                }
            }
        }
    }

    while (!WindowShouldClose()) // Main loop
    {
        UpdateLayout();

        BeginDrawing();
        ClearBackground(BLACK);
        Draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}