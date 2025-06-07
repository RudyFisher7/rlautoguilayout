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


#ifndef RLAUTO_LAYOUT_RAYLIB_DRAW_FUNCTIONS_H
#define RLAUTO_LAYOUT_RAYLIB_DRAW_FUNCTIONS_H


#include "rlauto_layout/type_defs.h"

#include <raylib.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct args_t
{
    Color color;
} Args;

typedef struct text_args_t
{
    Color color;
    int fontSize;
    const char* text;
} TextArgs;

typedef struct texture_args_t
{
    Texture2D texture;
    Color tint;
} TextureArgs;

typedef struct n_patch_texture_args_t
{
    NPatchInfo nPatchInfo;
    Texture2D texture;
    Color tint;
} NPatchTextureArgs;


/* Primitive Drawing */
void rlautoDrawLine(Layout *layout, void *args);
void rlautoDrawRectangle(Layout *layout, void *args);
void rlautoDrawRectangleLines(Layout *layout, void *args);

/* Text Drawing */
void rlautoDrawText(Layout *layout, void *args);

/**
 * Draws wrapped text.
 * @note The algorithm trims trailing horizontal white space, but preserves leading horizontal white space.
 * @param layout
 * @param args
 */
void rlautoDrawWrappedText(Layout *layout, void *args);

/* Texture Drawing */
void rlautoDrawTexture(Layout *layout, void *args);
void rlautoDrawTextureNPatch(Layout *layout, void *args);


#ifdef __cplusplus
};
#endif

#endif //RLAUTO_LAYOUT_RAYLIB_DRAW_FUNCTIONS_H
