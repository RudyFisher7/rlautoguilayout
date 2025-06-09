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


#include "rlauto_layout/raygui_draw_functions.h"

#ifndef RAYGUI_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#endif

#include <raygui.h>


inline static void HandleSetGuiState(GuiStateArgs *args, int *outPreviousValue)
{
    if (args->set)
    {
        if (args->restore)
        {
            *outPreviousValue = GuiGetState();
        }

        GuiSetState(args->value);
    }
}

inline static void HandleRestoreGuiState(GuiStateArgs *args, int previousValue)
{
    if (args->restore)
    {
        GuiSetState(previousValue);
    }
}

#define HANDLE_SET_STATE(args) int prevStateValue = 0; HandleSetGuiState(args, &prevStateValue)
#define HANDLE_RESTORE_STATE(args) HandleRestoreGuiState(args, prevStateValue)

inline static void HandleSetGuiStyle(GuiStyleArgs *args, int *outPreviousValue)
{
    if (args->setStyle)
    {
        if (args->restoreStyle)
        {
            *outPreviousValue = GuiGetStyle(args->styleControl, args->styleProperty);
        }

        GuiSetStyle(args->styleControl, args->styleProperty, args->styleValue);
    }
}

inline static void HandleRestoreGuiStyle(GuiStyleArgs *args, int previousValue)
{
    if (args->restoreStyle)
    {
        GuiSetStyle(args->styleControl, args->styleProperty, previousValue);
    }
}


#define HANDLE_SET_STYLE(args) int prevStyleValue = 0; HandleSetGuiStyle(args, &prevStyleValue)
#define HANDLE_RESTORE_STYLE(args) HandleRestoreGuiStyle(args, prevStyleValue)


void rlautoGuiControl(Layout *layout, void *args)
{
    (void)layout;

    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiGroupBox(Layout *layout, void *args)
{
    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiGroupBox(layout->bounds, argData->text);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiPanel(Layout *layout, void *args)
{
    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiPanel(layout->bounds, argData->text);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiButton(Layout *layout, void *args)
{
    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiButton(layout->bounds, argData->text);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiToggleGroup(Layout *layout, void *args)
{
    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    const char* t = argData->text;
    int itemCount = 1;
    while (*t)
    {
        if (*t == '\n')
        {
            ++itemCount;
        }
        ++t;
    }

    Rectangle bounds = layout->bounds;
    bounds.height = bounds.height / (float)itemCount;
    argData->returnedValue = GuiToggleGroup(bounds, argData->text, &argData->outValue);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiToggleSlider(Layout *layout, void *args)
{
    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    const char* t = argData->text;
    int itemCount = 1;
    while (*t)
    {
        if (*t == ';')
        {
            ++itemCount;
        }
        ++t;
    }

    Rectangle bounds = layout->bounds;
    bounds.width = bounds.width / (float)itemCount;
    argData->returnedValue = GuiToggleGroup(bounds, argData->text, &argData->outValue);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiCheckBox(Layout *layout, void *args)
{
    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    bool forceChecked = argData->outValue;

    argData->returnedValue = GuiCheckBox(layout->bounds, argData->text, &forceChecked);

    argData->outValue = forceChecked;

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiComboBox(Layout *layout, void *args)
{
    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiComboBox(layout->bounds, argData->text, &argData->outValue);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiDropdownBox(Layout *layout, void *args)
{
    GuiEditModeArgs *argData = (GuiEditModeArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    if (layout->isExpanded)
    {
        GuiUnlock();
    }

    argData->returnedValue = GuiDropdownBox(layout->bounds, argData->text, &argData->outValue, argData->editMode);
    layout->isExpanded = argData->editMode;

    if (layout->isExpanded)
    {
        GuiLock();
    }

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiSpinner(Layout *layout, void *args)
{
    GuiAllArgs *argData = (GuiAllArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiSpinner(layout->bounds, argData->text, &argData->outValue, argData->minValue, argData->maxValue, argData->editMode);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiValueBox(Layout *layout, void *args)
{
    GuiAllArgs *argData = (GuiAllArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiValueBox(layout->bounds, argData->text, &argData->outValue, argData->minValue, argData->maxValue, argData->editMode);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiTextBox(Layout *layout, void *args)
{
    GuiTextArgs *argData = (GuiTextArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiTextBox(layout->bounds, argData->text, argData->textSize, argData->editMode);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiSlider(Layout *layout, void *args)
{
    GuiAllArgsF *argData = (GuiAllArgsF*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    Rectangle bounds = layout->bounds;
    bounds.x += argData->textWidth;
    bounds.width -= (argData->textWidth + argData->rightTextWidth);
    argData->returnedValue = GuiSlider(bounds, argData->text, argData->rightText, &argData->outValue, argData->minValue, argData->maxValue);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiSliderBar(Layout *layout, void *args)
{
    GuiAllArgsF *argData = (GuiAllArgsF*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    Rectangle bounds = layout->bounds;
    bounds.x += argData->textWidth;
    bounds.width -= (argData->textWidth + argData->rightTextWidth);
    argData->returnedValue = GuiSliderBar(bounds, argData->text, argData->rightText, &argData->outValue, argData->minValue, argData->maxValue);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiProgressBar(Layout *layout, void *args)
{
    GuiAllArgsF *argData = (GuiAllArgsF*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    Rectangle bounds = layout->bounds;
    bounds.x += argData->textWidth;
    bounds.width -= (argData->textWidth + argData->rightTextWidth);
    argData->returnedValue = GuiProgressBar(bounds, argData->text, argData->rightText, &argData->outValue, argData->minValue, argData->maxValue);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiGrid(Layout *layout, void *args)
{
    GuiGridArgs *argData = (GuiGridArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiGrid(layout->bounds, argData->text, argData->spacing, argData->subdivs, &argData->mouseCell);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiScrollPanel(Layout *layout, void *args)
{
    GuiArgs *argData = (GuiArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiScrollPanel(layout->bounds, argData->text, layout->scrollContentBounds, &layout->scroll, &layout->scrollView);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiListView(Layout *layout, void *args)
{
    GuiOutValuesArgs *argData = (GuiOutValuesArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiListView(layout->bounds, argData->text, &argData->outValues[0], &argData->outValues[1]);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiListViewEx(Layout *layout, void *args)
{
    GuiOutValuesArgsEx *argData = (GuiOutValuesArgsEx*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiListViewEx(layout->bounds, argData->text, argData->itemCount, &argData->outValues[0], &argData->outValues[1], &argData->outValues[2]);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}

void rlautoGuiColorPicker(Layout *layout, void *args)
{
    GuiColorArgs *argData = (GuiColorArgs*)args;

    HANDLE_SET_STATE(&argData->stateArgs);
    HANDLE_SET_STYLE(&argData->styleArgs);

    argData->returnedValue = GuiColorPicker(layout->bounds, argData->text, &argData->outValue);

    HANDLE_RESTORE_STYLE(&argData->styleArgs);
    HANDLE_RESTORE_STATE(&argData->stateArgs);
}


#undef HANDLE_SET_STATE
#undef HANDLE_RESTORE_STATE

#undef HANDLE_SET_STYLE
#undef HANDLE_RESTORE_STYLE
