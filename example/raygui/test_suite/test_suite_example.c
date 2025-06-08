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
#include "rlauto_layout/raygui_draw_functions.h"
#include <raylib.h>

//#ifndef RAYGUI_IMPLEMENTATION
//#define RAYGUI_IMPLEMENTATION
//#endif

#include <raygui.h>

// raygui embedded styles
#include <style_cyber.h>
#include <style_jungle.h>
#include <style_lavanda.h>
#include <style_dark.h>
#include <style_bluish.h>
#include <style_terminal.h>

#include <stdio.h>


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    const int screenWidth = 960;
    const int screenHeight = 560;

    InitWindow(screenWidth, screenHeight, "raygui - controls test suite");
    SetExitKey(0);

    GuiEditModeArgs dropdownBoxArgs0 = {{1, 1, COMBOBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER}, 0, 0, 0, "ONE;TWO;THREE"};
    GuiEditModeArgs dropdownBoxArgs1 = {{0}, 0, 0, 0, "#01#ONE;#02#TWO;#03#THREE;#04#FOUR"};
    GuiTextArgs checkboxArgs0 = {{0}, 0, 0, "FORCE CHECK!"};
    GuiAllArgs spinnerArgs0 = {{0}, 0, 0, -3, 100, 0, NULL};
    GuiAllArgs valueBoxArgs0 = {{0}, 0, 0, -100, 100, 0, NULL};

    GUI_ROOT {
        SetPaddingAll(25.0f);
        GUI_HBOX {
            SetSizeFlagsBoth(SIZE_FLAGS_GROW);
//            SetMaxSize((Vector2){500.0f, 500.0f});
            GUI_VBOX {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                SetChildSpacing(16.0f);

                GUI {
                    SetSizeFlagX(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiDropdownBox, (void*)&dropdownBoxArgs0});
                }

                GUI {
                    SetSizeFlagX(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiDropdownBox, (void*)&dropdownBoxArgs1});
                }

                GUI {
                    SetMinSize((Vector2){25.0f, 25.0f});
                    SetDraw((DrawFunc){&rlautoGuiCheckBox, (void*)&checkboxArgs0});
                }

                GUI {
                    SetSizeFlagX(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiSpinner, (void*)&spinnerArgs0});
                }

                GUI {
                    SetSizeFlagX(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiValueBox, (void*)&valueBoxArgs0});
                }
            }
        }
    }

    float progressValue = 0.1f;
    int visualStyleActive = 0;
    int prevVisualStyleActive = 0;
    bool exitWindow = false;
    bool showMessageBox = false;
    bool showTextInputBox = false;
    float alpha = 1.0f;

    SetTargetFPS(60);

    while (!exitWindow)
    {
        exitWindow = WindowShouldClose();

        if (IsKeyPressed(KEY_ESCAPE)) showMessageBox = !showMessageBox;

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) showTextInputBox = true;

        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();

            if ((droppedFiles.count > 0) && IsFileExtension(droppedFiles.paths[0], ".rgs")) GuiLoadStyle(droppedFiles.paths[0]);

            UnloadDroppedFiles(droppedFiles);    // Clear internal buffers
        }

        //alpha -= 0.002f;
        if (alpha < 0.0f) alpha = 0.0f;
        if (IsKeyPressed(KEY_SPACE)) alpha = 1.0f;

        GuiSetAlpha(alpha);

        //progressValue += 0.002f;
        if (IsKeyPressed(KEY_LEFT)) progressValue -= 0.1f;
        else if (IsKeyPressed(KEY_RIGHT)) progressValue += 0.1f;
        if (progressValue > 1.0f) progressValue = 1.0f;
        else if (progressValue < 0.0f) progressValue = 0.0f;

        if (visualStyleActive != prevVisualStyleActive)
        {
            GuiLoadStyleDefault();

            switch (visualStyleActive)
            {
                case 0: break;      // Default style
                case 1: GuiLoadStyleJungle(); break;
                case 2: GuiLoadStyleLavanda(); break;
                case 3: GuiLoadStyleDark(); break;
                case 4: GuiLoadStyleBluish(); break;
                case 5: GuiLoadStyleCyber(); break;
                case 6: GuiLoadStyleTerminal(); break;
                default: break;
            }

            GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

            prevVisualStyleActive = visualStyleActive;
        }

        if (dropdownBoxArgs0.returnedValue)
        {
            dropdownBoxArgs0.editMode = !dropdownBoxArgs0.editMode;
        }

        if (dropdownBoxArgs1.returnedValue)
        {
            dropdownBoxArgs1.editMode = !dropdownBoxArgs1.editMode;
        }

        if (spinnerArgs0.returnedValue)
        {
            spinnerArgs0.editMode = !spinnerArgs0.editMode;
        }

        if (valueBoxArgs0.returnedValue)
        {
            valueBoxArgs0.editMode = !valueBoxArgs0.editMode;
        }

        UpdateLayout();

        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}