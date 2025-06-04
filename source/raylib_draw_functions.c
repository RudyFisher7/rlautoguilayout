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


#include "rlauto_layout/raylib_draw_functions.h"


void rlautoDrawLine(Rectangle *bounds, void *args)
{
    Args *argData = (Args*)args;
    DrawLine(
            (int)bounds->x,
            (int)bounds->y,
            (int)bounds->x + (int)bounds->width,
            (int)bounds->y,
            argData->color
    );
}

void rlautoDrawRectangle(Rectangle *bounds, void *args)
{
    Args *argData = (Args*)args;
    DrawRectangleRec(*bounds, argData->color);
}

void rlautoDrawRectangleLines(Rectangle *bounds, void *args)
{
    Args *argData = (Args*)args;
    DrawRectangleLines((int)bounds->x, (int)bounds->y, (int)bounds->width, (int)bounds->height, argData->color);
}

void rlautoDrawText(Rectangle *bounds, void *args)
{
    TextArgs *argData = (TextArgs*)args;
    DrawText(argData->text, bounds->x, bounds->y, argData->fontSize, argData->color);
}

void rlautoDrawWrappedText(Rectangle *bounds, void *args)
{
    TextArgs *argData = (TextArgs*)args;
    DrawText(argData->text, bounds->x, bounds->y, argData->fontSize, argData->color);
}