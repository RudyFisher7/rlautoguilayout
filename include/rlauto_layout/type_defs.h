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


#ifndef RLAUTO_LAYOUT_STRUCTS_H
#define RLAUTO_LAYOUT_STRUCTS_H

#include <raylib.h>

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

typedef enum size_flags_t
{
    SIZE_FLAGS_MIN = 0,
    SIZE_FLAGS_FIT = SIZE_FLAGS_MIN,
    SIZE_FLAGS_FIXED ,
    SIZE_FLAGS_GROW,
    SIZE_FLAGS_SIZE,
} SizeFlags;


typedef enum child_alignment_t
{
    CHILD_ALIGNMENT_MIN = 0,
    CHILD_ALIGNMENT_BEGIN = CHILD_ALIGNMENT_MIN,
    CHILD_ALIGNMENT_CENTER,
    CHILD_ALIGNMENT_END,
    CHILD_ALIGNMENT_RADIAL,
    CHILD_ALIGNMENT_SIZE,
} ChildAlignment;


typedef enum child_layout_axis_t
{
    CHILD_LAYOUT_AXIS_MIN = 0,
    CHILD_LAYOUT_AXIS_X = CHILD_LAYOUT_AXIS_MIN,
    CHILD_LAYOUT_AXIS_Y,
    CHILD_LAYOUT_AXIS_SIZE,
} ChildLayoutAxis;

typedef struct vector2_uint8_t
{
    uint8_t x;
    uint8_t y;
} Vector2UInt8;

typedef struct layout_t
{
    Rectangle bounds;
    Rectangle scrollContentBounds;
    Vector4 margins; // padding is in css's order, so {x=top, y=right, z=bottom, w=left}. Vector4 is used for convenience.
    Vector4 padding; // margin is in css's order, so {x=top, y=right, z=bottom, w=left}. Vector4 is used for convenience.
    Vector2 minSize;
    Vector2 maxSize;
    float childSpacing;
    float fontSize;
    float lineSpacing;
    float wrappedTextSizeV;
    Vector2UInt8 sizeFlags;
    Vector2UInt8 childAlignment;
    ChildLayoutAxis childLayoutAxis;
    int hScrollEnabled;
    int vScrollEnabled;
} Layout;


typedef struct draw_func_t
{
    void(*draw)(Rectangle*, void*);
    void* args;
} DrawFunc;


typedef struct node_t
{
    Layout layout;
    DrawFunc drawFunc;
    struct node_t* parent;
    struct node_t* leftSibling;
    struct node_t* rightSibling;
    struct node_t* firstChild;
    struct node_t* lastChild;
} Node;


#ifdef __cplusplus
};
#endif

#endif //RLAUTO_LAYOUT_STRUCTS_H
