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

#ifndef RLAUTO_LAYOUT_TREE_H
#define RLAUTO_LAYOUT_TREE_H

#include "rlauto_layout/type_defs.h"

#ifdef __cplusplus
extern "C" {
#endif


void BeginRoot(void);
void EndRoot(void);

void Begin(void);
void BeginHBox(void);
void BeginVBox(void);
void End(void);

void SetBounds(Rectangle value);
void SetPosition(Vector2 value);
void SetSize(Vector2 value);
void SetMargins(Vector4 value);
void SetMarginsAll(float value);
void SetMarginsX(Vector2 value);
void SetMarginsY(Vector2 value);
void SetPadding(Vector4 value);
void SetPaddingAll(float value);
void SetPaddingX(Vector2 value);
void SetPaddingY(Vector2 value);
void SetMinSize(Vector2 value);
void SetMinSizeX(float value);
void SetMinSizeY(float value);
void SetMaxSize(Vector2 value);
void SetMaxSizeX(float value);
void SetMaxSizeY(float value);
void SetChildSpacing(float value);
void SetSizeFlags(Vector2UInt8 value);
void SetSizeFlagsBoth(uint8_t value);
void SetSizeFlagX(uint8_t value);
void SetSizeFlagY(uint8_t value);
void SetChildAlignment(Vector2UInt8 value);
void SetChildLayoutAxis(ChildLayoutAxis value);
void SetText(const char* value, int textLength, float fontSize, float lineSpacing);
void SetScrollEnabled(int x, int y);
void SetDraw(DrawFunc value);

void UpdateLayout(void);

void Draw(void);

/**
 * A draw function that just passes through (draws nothing and computes nothing).
 * Parameters can be NULL.
 * @param bounds Unused. Can be NULL.
 * @param args Unused. Can be NULL.
 */
void PassThroughDraw(Layout *layout, void *args);


#define GUI_ROOT static int rlautoMacroIndex = 0;\
    BeginRoot();\
    for (rlautoMacroIndex = 0; rlautoMacroIndex < 1; ++rlautoMacroIndex, EndRoot())

#define GUI Begin();\
    for (rlautoMacroIndex = 0; rlautoMacroIndex < 1; ++rlautoMacroIndex, End())

#define GUI_HBOX BeginHBox();\
    for (rlautoMacroIndex = 0; rlautoMacroIndex < 1; ++rlautoMacroIndex, End())

#define GUI_VBOX BeginVBox();\
    for (rlautoMacroIndex = 0; rlautoMacroIndex < 1; ++rlautoMacroIndex, End())


#ifdef __cplusplus
};
#endif

#endif //RLAUTO_LAYOUT_TREE_H
