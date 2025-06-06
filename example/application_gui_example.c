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
#include "rlauto_layout/raylib_draw_functions.h"
#include "rlauto_layout/raygui_draw_functions.h"
#include <raylib.h>

#include <stdio.h>




int main() {
#ifdef RESOURCE_DIR
    const char* ResourceDir = RESOURCE_DIR;
#else
    char ResourceDir[72];
    sprintf(ResourceDir, "%s/%s", GetWorkingDirectory(), "resource");
#endif
    printf("Resource Directory: %s\n", ResourceDir);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Raylib Application GUI Example");

    char format_string_buffer[72];
    sprintf(format_string_buffer, "%s/%s", ResourceDir, "nine-patch-rect-128x128.png");
    Texture2D ninePatchRectTexture = LoadTexture(format_string_buffer);

    Args args0 = {GREEN};
    Args args1 = {BLUE};
    Args args2 = {ORANGE};
    TextArgs args3 = {RAYWHITE, 20, "Hello raylib!"};
    TextureArgs args5 = {ninePatchRectTexture, RAYWHITE};
    NPatchTextureArgs args6 = {(NPatchInfo){(Rectangle){0.0f, 0.0f, 0.0f, 0.0f}, 16, 16, 16, 16, NPATCH_NINE_PATCH}, ninePatchRectTexture, RAYWHITE};
    GuiTextArgs args4 = {0, "Hello raygui!"};

    GUI_ROOT {
        GUI_VBOX {
            SetSizeFlagsBoth(SIZE_FLAGS_GROW);
            SetMarginsAll(8.0f);

            // heading navigation
            GUI_HBOX {
                SetSizeFlags((Vector2UInt8){SIZE_FLAGS_GROW, SIZE_FLAGS_FIT});
                SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_CENTER, CHILD_ALIGNMENT_END});
                SetChildSpacing(4.0f);
                SetPaddingAll(4.0f);
                SetMarginsAll(8.0f);
                SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args0});

                const int navigationLinks0 = 4;
                int navLinks0[4];
                for (int i = 0; i < navigationLinks0; ++i) {
                    GUI {
                        SetSizeFlags((Vector2UInt8){SIZE_FLAGS_GROW, SIZE_FLAGS_FIT});
                        SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_CENTER, CHILD_ALIGNMENT_END});
                        SetDraw((DrawFunc){&rlautoDrawRectangleLines, (void*)&args1});
                        GUI {
                            SetSizeFlags((Vector2UInt8){SIZE_FLAGS_FIXED, SIZE_FLAGS_FIXED});
                            SetSize((Vector2){120.0f, 16.0f});
                            SetDraw((DrawFunc){&rlautoGuiButton, (void*)&args4});
                        }
                    }
                }
            }

            // body
            GUI_HBOX {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);

                // side panel navigation
                GUI_VBOX {
                    SetSizeFlags((Vector2UInt8){SIZE_FLAGS_FIT, SIZE_FLAGS_GROW});
                    SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_CENTER, CHILD_ALIGNMENT_BEGIN});
                    SetChildSpacing(16.0f);
                    SetPaddingAll(16.0f);
                    SetMarginsAll(8.0f);
                    SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args1});

                    const int navigationLinks1 = 4;
                    int navLinks1[4];
                    for (int i = 0; i < navigationLinks1; ++i) {
                        GUI {
                            SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                            SetMinSize((Vector2){200.0f, 24.0f});
                            SetDraw((DrawFunc){&rlautoGuiButton, (void*)&args4});
//                            SetDraw((DrawFunc){&rlautoDrawText, (void*)&args3});
                        }
                    }
                }

                // content area
                GUI_VBOX {
                    SetSizeFlags((Vector2UInt8){SIZE_FLAGS_GROW, SIZE_FLAGS_GROW});
                    SetChildAlignment((Vector2UInt8){CHILD_ALIGNMENT_BEGIN, CHILD_ALIGNMENT_BEGIN});
                    SetChildSpacing(24.0f);
                    SetScrollEnabled(1, 1);
                    SetDraw((DrawFunc){&rlautoGuiScrollPanel, (void*)&args4});

//                    GUI {
//                        SetSizeFlagsBoth(SIZE_FLAGS_GROW);
//                        SetMaxSize((Vector2){600.0f, 600.0f});
//                        SetDraw((DrawFunc){&rlautoDrawTextureNPatch, (void*)&args6});
//                    }

                    const int navigationLinks2 = 4;
                    int navLinks2[4];
                    for (int i = 0; i < navigationLinks2; ++i) {
                        GUI {
                            SetSizeFlags((Vector2UInt8){SIZE_FLAGS_GROW, SIZE_FLAGS_GROW});
                            SetMaxSize((Vector2){800.0f, 200.0f});
                            SetMarginsAll(64.0f);
                            SetDraw((DrawFunc){&rlautoDrawRectangle, (void*)&args2});
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

//        rlautoGuiScrollPanel(NULL, NULL);
        EndDrawing();
    }

    UnloadTexture(ninePatchRectTexture);
    CloseWindow();

    return 0;
}