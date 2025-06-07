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

#include <string.h>


void rlautoDrawLine(Layout *layout, void *args)
{
    Args *argData = (Args*)args;
    DrawLine((int)layout->bounds.x, (int)layout->bounds.y, (int)layout->bounds.x + (int)layout->bounds.width, (int)layout->bounds.y, argData->color);
}

void rlautoDrawRectangle(Layout *layout, void *args)
{
    Args *argData = (Args*)args;
    DrawRectangleRec(layout->bounds, argData->color);
}

void rlautoDrawRectangleLines(Layout *layout, void *args)
{
    Args *argData = (Args*)args;
    DrawRectangleLines((int)layout->bounds.x, (int)layout->bounds.y, (int)layout->bounds.width, (int)layout->bounds.height, argData->color);
}

void rlautoDrawText(Layout *layout, void *args)
{
    TextArgs *argData = (TextArgs*)args;
    DrawText(argData->text, (int)layout->bounds.x, (int)layout->bounds.y, argData->fontSize, argData->color);
}

void rlautoDrawWrappedText(Layout *layout, void *args)
{
    TextArgs *argData = (TextArgs*)args;
    Font font = GetFontDefault();//fixme:: put this in layout
    float charSpacing = 8.0f;//fixme:: put this in layout

    int codepointCount = 0;
    int *codepoints = LoadCodepoints(layout->text, &codepointCount);
    int *currentCodepoints = codepoints;
    int currentIndex = 0;
    float width = 0.0f;

    Vector2 position = {layout->bounds.x, layout->bounds.y};
    for (int i = 0; i < codepointCount; ++i)
    {
        if (codepoints[i] == '\n')
        {
            width = 0.0f;
            DrawTextCodepoints(font, currentCodepoints, (i - currentIndex), position, layout->fontSize, layout->lineSpacing, argData->color);

            position.y += layout->fontSize + layout->lineSpacing;
            currentCodepoints = (codepoints + i + 1);
            currentIndex = i;

            ++i;
        }

        int glyphIndex = GetGlyphIndex(font, codepoints[i]);

        float nextWidth = 0.0f;
        if ((i + 1) < codepointCount && (codepoints[i + 1] != ' ' && codepoints[i + 1] != '\t'))
        {
            int nextGlyphIndex = GetGlyphIndex(font, codepoints[i + 1]);
            nextWidth = (float)font.recs[nextGlyphIndex].width + charSpacing;
        }

        width += (float)font.recs[glyphIndex].width + charSpacing;

        if ((width + nextWidth) > layout->bounds.width)
        {
            width = 0.0f;

            if ((i + 1) < codepointCount)
            {
                while (codepoints[i] != ' ' && codepoints[i] != '\t')
                {
                    --i;
                }

                while (codepoints[i] == ' ' || codepoints[i] == '\t')
                {
                    --i;
                }
            }

            DrawTextCodepoints(font, currentCodepoints, (i + 1 - currentIndex), position, layout->fontSize, layout->lineSpacing, argData->color);
            ++i;

            while (codepoints[i] == ' ' || codepoints[i] == '\t')
            {
                ++i;
            }

            position.y += layout->fontSize + layout->lineSpacing;
            currentCodepoints = (codepoints + i);
            currentIndex = i;
        }
        else if ((i + 1) >= codepointCount)
        {
            DrawTextCodepoints(font, currentCodepoints, (i - currentIndex), position, layout->fontSize, layout->lineSpacing, argData->color);
        }
    }

    UnloadCodepoints(codepoints);


//    const char* current_text = argData->text;
//    int text_length = strlen(current_text);
//    float font_width = argData->fontSize;
//    int codepoint_count_per_line = (int)(layout->bounds.width / font_width);
//    Vector2 current_position = {layout->bounds.x, layout->bounds.y};
//    int i = 0;
//    while (i < text_length) {
//        int current_codepoint_length = 0;
//        int codepoint = GetCodepointNext(current_text, &current_codepoint_length);
//        DrawTextCodepoint(GetFontDefault(), codepoint, current_position, argData->fontSize, argData->color);
//
//        current_position.x += font_width;
//        i += current_codepoint_length;
//
//        if (i % codepoint_count_per_line == 0) {
//            current_position.y += argData->fontSize * 2.0f;
//            current_position.x = layout->bounds.x;
//        }
//
//        current_text += current_codepoint_length;
//    }
}

void rlautoDrawTexture(Layout *layout, void *args)
{
    TextureArgs *argData = (TextureArgs*)args;
    DrawTexturePro(argData->texture, (Rectangle){0.0f, 0.0f, (float)argData->texture.width, (float)argData->texture.height}, layout->bounds, (Vector2){0.0f, 0.0f}, 0.0f, argData->tint);
}

void rlautoDrawTextureNPatch(Layout *layout, void *args)
{
    NPatchTextureArgs *argData = (NPatchTextureArgs*)args;
    argData->nPatchInfo.source = (Rectangle){0.0f, 0.0f, (float)argData->texture.width, (float)argData->texture.height};
    DrawTextureNPatch(argData->texture, argData->nPatchInfo, layout->bounds, (Vector2){0.0f, 0.0f}, 0.0f, argData->tint);
}
