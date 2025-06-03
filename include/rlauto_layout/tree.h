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


extern void BeginRoot(void);
extern void EndRoot(void);

extern void Begin(void);
extern void BeginHBox(void);
extern void BeginVBox(void);
extern void End(void);

extern void SetBounds(Rectangle value);
extern void SetPosition(Vector2 value);
extern void SetSize(Vector2 value);
extern void SetMargins(Vector4 value);
extern void SetMarginsAll(float value);
extern void SetMarginsX(Vector2 value);
extern void SetMarginsY(Vector2 value);
extern void SetPadding(Vector4 value);
extern void SetPaddingAll(float value);
extern void SetPaddingX(Vector2 value);
extern void SetPaddingY(Vector2 value);
extern void SetMinSize(Vector2 value);
extern void SetMinSizeX(float value);
extern void SetMinSizeY(float value);
extern void SetMaxSize(Vector2 value);
extern void SetMaxSizeX(float value);
extern void SetMaxSizeY(float value);
extern void SetChildSpacing(float value);
extern void SetSizeFlags(Vector2UInt8 value);
extern void SetSizeFlagsBoth(uint8_t value);
extern void SetSizeFlagX(uint8_t value);
extern void SetSizeFlagY(uint8_t value);
extern void SetChildAlignment(Vector2UInt8 value);
extern void SetChildLayoutAxis(ChildLayoutAxis value);
extern void SetText(const char* value, int textLength, float fontSize, float lineSpacing);
extern void SetDraw(DrawFunc value);
extern void UpdateLayout(void);

extern void Draw(void);


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
