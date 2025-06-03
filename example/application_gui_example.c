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
#include "rlauto_layout/draw_functions.h"
#include <raylib.h>


int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Raylib Application GUI Example");

    char format_string_buffer[48];

    Args args0 = {GREEN};
    Args args1 = {BLUE};
    Args args2 = {ORANGE};

    GUI_ROOT {
        GUI_VBOX {
            SetSizeFlagsBoth(SIZE_FLAGS_GROW);
            GUI_HBOX {
                SetSizeFlagX(SIZE_FLAGS_GROW);
                SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_CENTER, CHILD_ALIGNMENT_END});
                SetChildSpacing(16.0f);
                SetPaddingAll(4.0f);
                SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args0});

                const int navigationLinks0 = 4;
                int navLinks0[navigationLinks0] = {};
                for (int i = 0; i < navigationLinks0; ++i) {
                    GUI {
                        SetSizeFlagsBoth(SIZE_FLAGS_FIXED);
                        SetSize((Vector2){120.0f, 16.0f});
                        SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args1});
                    }
                }
            }

            GUI_HBOX {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args2});
                GUI_VBOX {
                    SetSizeFlags((Vector2UInt8){SIZE_FLAGS_FIT, SIZE_FLAGS_GROW});
                    SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_CENTER, CHILD_ALIGNMENT_END});
                    SetChildSpacing(16.0f);
                    SetPaddingAll(4.0f);
                    SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args1});

                    const int navigationLinks1 = 4;
                    int navLinks1[navigationLinks1] = {};
                    for (int i = 0; i < navigationLinks1; ++i) {
                        GUI {
                            SetSizeFlagsBoth(SIZE_FLAGS_FIXED);
                            SetSize((Vector2){120.0f, 16.0f});
                            SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args0});
                        }
                    }
                }

                GUI_VBOX {
                    SetSizeFlags((Vector2UInt8){SIZE_FLAGS_GROW, SIZE_FLAGS_GROW});
                    SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_CENTER, CHILD_ALIGNMENT_END});
                    SetChildSpacing(16.0f);
                    SetPaddingAll(4.0f);
                    SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args2});

                    const int navigationLinks2 = 4;
                    int navLinks2[navigationLinks2] = {};
                    for (int i = 0; i < navigationLinks2; ++i) {
                        GUI {
                            SetSizeFlagsBoth(SIZE_FLAGS_FIXED);
                            SetSize((Vector2){120.0f, 16.0f});
                            SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args1});
                        }
                    }
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