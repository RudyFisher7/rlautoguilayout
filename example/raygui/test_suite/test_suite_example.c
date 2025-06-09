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


/*
 * This example is inspired by Raygui's controls_test_suite example.
 */

#include "rlauto_layout/tree.h"
#include "rlauto_layout/raygui_draw_functions.h"

#include <raylib.h>
#include <raygui.h>

#include <style_cyber.h>
#include <style_jungle.h>
#include <style_lavanda.h>
#include <style_dark.h>
#include <style_bluish.h>
#include <style_terminal.h>


void UpdateSliderValueText(GuiAllArgsF *args)
{
    args->rightText = TextFormat("%2.2f", args->outValue);
}

void UpdateProgressValueText(GuiAllArgsF *args)
{
    args->rightText = TextFormat("%i%%", (int)(args->outValue * 100));
}


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    const int screenWidth = 960;
    const int screenHeight = 560;

    InitWindow(screenWidth, screenHeight, "raygui - controls test suite");
    SetExitKey(0);

    GuiEditModeArgs dropdownBoxArgs0 = {{1, 1, DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER}, {0}, 0, 0, 0, "ONE;TWO;THREE"};
    GuiEditModeArgs dropdownBoxArgs1 = {{1, 0, DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT}, {0}, 0, 0, 0, "#01#ONE;#02#TWO;#03#THREE;#04#FOUR"};
    GuiArgs checkboxArgs0 = {{0}, {0}, 0, 0, "FORCE CHECK!"};
    GuiAllArgs spinnerArgs0 = {{0}, {0}, 0, 0, -3, 100, 0, NULL, NULL};
    GuiAllArgs valueBoxArgs0 = {{0}, {0}, 0, 0, -100, 100, 0, NULL, NULL};

    char textBoxBuffer0[64] = {0};
    GuiTextArgs textBoxArgs0 = {{0}, {0}, 0, 64, false, textBoxBuffer0};
    char loremIpsum0[1024] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\nThisisastringlongerthanexpectedwithoutspacestotestcharbreaksforthosecases,checkingifworkingasexpected.\n\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    GuiArgs controlArgs1 = {{1, 0, DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP}, {0}, 0, 0, NULL};
    GuiTextArgs textBoxArgs1 = {{1, 1, DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD}, {0}, 0, 1024, false, loremIpsum0};
    GuiArgs controlArgs2 = {{1, 0, DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE}, {0}, 0, 0, NULL};
    GuiArgs buttonArgs0 = {{0}, {0}, 0, 0, GuiIconText(ICON_FILE_SAVE, "Save File")};
    GuiArgs buttonArgs1 = {{0}, {1, 0, STATE_NORMAL}, 0, 0, "NORMAL"};
    GuiArgs buttonArgs2 = {{0}, {1, 0, STATE_FOCUSED}, 0, 0, "FOCUSED"};
    GuiArgs buttonArgs3 = {{0}, {1, 0, STATE_PRESSED}, 0, 0, "#15#PRESSED"};
    GuiArgs buttonArgs4 = {{0}, {1, 0, STATE_DISABLED}, 0, 0, "DISABLED"};
    GuiArgs controlArgs0 = {{0}, {1, 0, STATE_NORMAL}, 0, 0, NULL};
    GuiArgs groupBoxArgs0 = {{0}, {0}, 0, 0, "STATES"};
    GuiArgs comboBoxArgs0 = {{0}, {0}, 0, 0, "default;Jungle;Lavanda;Dark;Bluish;Cyber;Terminal"};
    GuiOutValuesArgs listViewArgs0 = {{0}, {0}, 0, {0}, "Charmander;Bulbasaur;#18#Squirtel;Pikachu;Eevee;Pidgey"};

    const char *listViewExList[8] = { "This", "is", "a", "list view", "with", "disable", "elements", "amazing!" };
    GuiOutValuesArgsEx listViewArgs1 = {{0}, {0}, 0, {0}, 8, listViewExList};
    GuiArgs toggleGroupArgs0 = {{0}, {0}, 0, 0, "#1#ONE\n#3#TWO\n#8#THREE\n#23#"};
    GuiArgs toggleSliderArgs0 = {{0}, {0}, 0, 0, "ON;OFF"};
    GuiArgs panelArgs0 = {{0}, {0}, 0, 0, "Panel Info"};
    GuiColorArgs colorPickerArgs0 = {{0}, {0}, 0, {0}, "Panel Info"};
    GuiAllArgsF sliderArgs0 = {{0}, {0}, 0, 0.0f, -100.0f, 100.0f, 0, (float)MeasureText("TEST", GuiGetFont().baseSize), 32.0f, "TEST", ""};
    GuiAllArgsF sliderBarArgs0 = {{0}, {0}, 0, 0.0f, -100.0f, 100.0f, 0, 0.0f, 32.0f, NULL, ""};
    GuiAllArgsF progressBarArgs0 = {{0}, {0}, 0, 0.0f, 0.0f, 1.0f, 0, 0.0f, 32.0f, NULL, ""};
    GuiArgs scrollPanelArgs0 = {{0}, {0}, 0, 0, NULL};
    GuiGridArgs gridArgs0 = {{0}, {0}, {0}, 0, 20.0f, 3, "Hello, Grid!"};

    GUI_ROOT {
        SetPaddingAll(25.0f);
        GUI_HBOX {
            SetSizeFlagsBoth(SIZE_FLAGS_GROW);
            SetChildSpacing(25.0f);
            GUI_VBOX {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                SetChildSpacing(16.0f);

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiDropdownBox, (void*)&dropdownBoxArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiDropdownBox, (void*)&dropdownBoxArgs1});
                }

                GUI {
                    SetMinSize((Vector2){25.0f, 25.0f});
                    SetDraw((DrawFunc){&rlautoGuiCheckBox, (void*)&checkboxArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiSpinner, (void*)&spinnerArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiValueBox, (void*)&valueBoxArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiTextBox, (void*)&textBoxArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiButton, (void*)&buttonArgs0});
                }

                GUI_VBOX {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetPaddingAll(5.0f);
                    SetChildSpacing(5.0f);
                    SetDraw((DrawFunc){rlautoGuiGroupBox, (void*)&groupBoxArgs0});

                    GUI {
                        SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                        SetMinSize((Vector2){125.0f, 30.0f});
                        SetDraw((DrawFunc){&rlautoGuiButton, (void*)&buttonArgs1});
                    }

                    GUI {
                        SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                        SetMinSize((Vector2){125.0f, 30.0f});
                        SetDraw((DrawFunc){&rlautoGuiButton, (void*)&buttonArgs2});
                    }

                    GUI {
                        SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                        SetMinSize((Vector2){125.0f, 30.0f});
                        SetDraw((DrawFunc){&rlautoGuiButton, (void*)&buttonArgs3});
                    }

                    GUI {
                        SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                        SetMinSize((Vector2){125.0f, 30.0f});
                        SetDraw((DrawFunc){&rlautoGuiButton, (void*)&buttonArgs4});
                    }

                    GUI {
                        SetDraw((DrawFunc){&rlautoGuiControl, (void*)&controlArgs0});
                    }
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiComboBox, (void*)&comboBoxArgs0});
                }
            }

            GUI_VBOX {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                SetChildSpacing(16.0f);

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 125.0f});
                    SetDraw((DrawFunc){&rlautoGuiListView, (void*)&listViewArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 125.0f});
                    SetDraw((DrawFunc){&rlautoGuiListViewEx, (void*)&listViewArgs1});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiToggleGroup, (void*)&toggleGroupArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiToggleSlider, (void*)&toggleSliderArgs0});
                }
            }

            GUI_VBOX {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                SetChildSpacing(16.0f);

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiPanel, (void*)&panelArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiColorPicker, (void*)&colorPickerArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiSlider, (void*)&sliderArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiSliderBar, (void*)&sliderBarArgs0});
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){125.0f, 30.0f});
                    SetDraw((DrawFunc){&rlautoGuiProgressBar, (void*)&progressBarArgs0});
                }
            }

            GUI_VBOX {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                SetChildSpacing(16.0f);

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetScrollEnabled(1, 1);
                    SetDraw((DrawFunc){&rlautoGuiScrollPanel, (void*)&scrollPanelArgs0});

                    GUI {
                        SetSizeFlagsBoth(SIZE_FLAGS_FIXED);
                        SetSize((Vector2){800.0f, 800.0f});
                        SetDraw((DrawFunc){&rlautoGuiGrid, (void*)&gridArgs0});
                    }
                }

                GUI {
                    SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                    SetMinSize((Vector2){32.0f, 32.0f});
                    SetDraw((DrawFunc){&rlautoGuiGrid, (void*)&gridArgs0});
                }
            }

            GUI {
                SetDraw((DrawFunc){&rlautoGuiControl, (void*)&controlArgs1});
            }

            GUI {
                SetSizeFlagsBoth(SIZE_FLAGS_GROW);
                SetMinSize((Vector2){125.0f, 30.0f});
                SetDraw((DrawFunc){&rlautoGuiTextBox, (void*)&textBoxArgs1});
            }

            GUI {
                SetDraw((DrawFunc){&rlautoGuiControl, (void*)&controlArgs2});
            }
        }
    }

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
        if (IsKeyPressed(KEY_LEFT)) progressBarArgs0.outValue -= 0.1f;
        else if (IsKeyPressed(KEY_RIGHT)) progressBarArgs0.outValue += 0.1f;
        if (progressBarArgs0.outValue > 1.0f) progressBarArgs0.outValue = 1.0f;
        else if (progressBarArgs0.outValue < 0.0f) progressBarArgs0.outValue = 0.0f;

        if (comboBoxArgs0.outValue != prevVisualStyleActive)
        {
            GuiLoadStyleDefault();

            switch (comboBoxArgs0.outValue)
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

            prevVisualStyleActive = comboBoxArgs0.outValue;
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

        if (textBoxArgs0.returnedValue)
        {
            textBoxArgs0.editMode = !textBoxArgs0.editMode;
        }

        UpdateSliderValueText(&sliderArgs0);
        UpdateSliderValueText(&sliderBarArgs0);
        UpdateProgressValueText(&progressBarArgs0);

        UpdateLayout();

        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}