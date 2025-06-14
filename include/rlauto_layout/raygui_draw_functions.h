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


#ifndef RLAUTO_LAYOUT_RAYGUI_DRAW_FUNCTIONS_H
#define RLAUTO_LAYOUT_RAYGUI_DRAW_FUNCTIONS_H


#include "rlauto_layout/raylib_draw_functions.h"

#include "rlauto_layout/type_defs.h"

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct gui_state_args_t
{
    int set;
    int restore;
    int value;
} GuiStateArgs;

typedef struct gui_style_args_t
{
    int setStyle;
    int restoreStyle;
    int styleControl;
    int styleProperty;
    int styleValue;
} GuiStyleArgs;


typedef struct gui_args_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    int returnedValue;
    int outValue;
    const char* text;
} GuiArgs;

typedef struct gui_grid_args_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    Vector2 mouseCell;
    int returnedValue;
    float spacing;
    int subdivs;
    const char* text;
} GuiGridArgs;

typedef struct gui_color_args_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    int returnedValue;
    Color outValue;
    const char* text;
} GuiColorArgs;

typedef struct gui_out_values_args_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    int returnedValue;
    int outValues[2];
    const char* text;
} GuiOutValuesArgs;

typedef struct gui_out_values_args_ex_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    int returnedValue;
    int outValues[3];
    int itemCount;
    const char** text;
} GuiOutValuesArgsEx;

typedef struct gui_text_args_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    int returnedValue;
    int textSize;
    bool editMode;
    char* text;
} GuiTextArgs;


typedef struct gui_edit_mode_args_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    int returnedValue;
    int outValue;
    bool editMode;
    const char* text;
} GuiEditModeArgs;

typedef struct gui_all_args_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    int returnedValue;
    int outValue;
    int minValue;
    int maxValue;
    bool editMode;
    const char* text;
    const char* textRight;
} GuiAllArgs;


typedef struct gui_all_args_f_t
{
    GuiStyleArgs styleArgs;
    GuiStateArgs stateArgs;
    int returnedValue;
    float outValue;
    float minValue;
    float maxValue;
    bool editMode;
    float textWidth;
    float rightTextWidth;
    const char* text;
    const char* rightText;
} GuiAllArgsF;


void rlautoGuiControl(Layout *layout, void *args);
void rlautoGuiGroupBox(Layout *layout, void *args);
void rlautoGuiPanel(Layout *layout, void *args);
void rlautoGuiButton(Layout *layout, void *args);
void rlautoGuiToggleGroup(Layout *layout, void *args);
void rlautoGuiToggleSlider(Layout *layout, void *args);
void rlautoGuiCheckBox(Layout *layout, void *args);
void rlautoGuiComboBox(Layout *layout, void *args);
void rlautoGuiDropdownBox(Layout *layout, void *args);
void rlautoGuiSpinner(Layout *layout, void *args);
void rlautoGuiValueBox(Layout *layout, void *args);
void rlautoGuiTextBox(Layout *layout, void *args);
void rlautoGuiSlider(Layout *layout, void *args);
void rlautoGuiSliderBar(Layout *layout, void *args);
void rlautoGuiProgressBar(Layout *layout, void *args);
void rlautoGuiGrid(Layout *layout, void *args);
void rlautoGuiScrollPanel(Layout *layout, void *args);
void rlautoGuiListView(Layout *layout, void *args);
void rlautoGuiListViewEx(Layout *layout, void *args);
void rlautoGuiColorPicker(Layout *layout, void *args);


#ifdef __cplusplus
};
#endif


#endif //RLAUTO_LAYOUT_RAYGUI_DRAW_FUNCTIONS_H
