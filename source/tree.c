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

#include <ctype.h>
#include <math.h>
#include <float.h>
#include <stdio.h>


#ifndef UNDEF_CAPACITY
#define UNDEF_CAPACITY 1
#endif

#ifndef UNDEF_INDEX_MODE
#define UNDEF_INDEX_MODE 1
#endif

#ifndef RLAUTO_LAYOUT_TREE_CAPACITY
#define RLAUTO_LAYOUT_TREE_CAPACITY 32
#endif

#ifndef RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE
#define RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE 1
#endif

// Data used inside this translation unit.
static int currentIndex = 0;
static int treeSize = 0;
static Node tree[RLAUTO_LAYOUT_TREE_CAPACITY];
static Node* bFSTree[RLAUTO_LAYOUT_TREE_CAPACITY];
static Node* currentNode = NULL;
static const Layout DefaultNodeLayout = {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f},
        {FLT_MAX, FLT_MAX},
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        {SIZE_FLAGS_FIT, SIZE_FLAGS_FIT},
        {CHILD_ALIGNMENT_BEGIN, CHILD_ALIGNMENT_BEGIN},
        CHILD_LAYOUT_AXIS_X,
        0,
        0
};


#ifndef ENABLE_DEBUG
#define ENABLE_DEBUG 1
#endif

#if ENABLE_DEBUG
#define DEBUG_DATA int debugCurrentIndex = currentIndex; int debugTreeSize = treeSize; Node* debugCurrentNode = currentNode;
#else
#define DEBUG_DATA ((void)0);
#endif


#if RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE
static inline Node* GetSafe(int i, const char* file, int line)
{
    if (i >= 0 && i < RLAUTO_LAYOUT_TREE_CAPACITY)
    {
        return &tree[i];
    }

    printf("Error: Index %d out of tree bounds at %s:%d.", i, file, line);

    return NULL;
}

static inline void SetSafe(int i, Node value, const char* file, int line)
{
    if (i >= 0 && i < RLAUTO_LAYOUT_TREE_CAPACITY)
    {
        tree[i] = value;
    }
    else
    {
        printf("Error: Index %d out of tree bounds at %s:%d.", i, file, line);
    }
}

#define GET(i) GetSafe(i, __FILE__, __LINE__)
#define SET(i, value) SetSafe(i, (value), __FILE__, __LINE__)
#else
#define GET(i) &tree[i]
#define SET(i, value) tree[i] = (value)
#endif

// Forward declarations of functions that are internal to this translation unit.
static void AssembleBFSTree(void);
static void BeginRootInternal(void);
static void EndRootInternal(void);
static void BeginInternal(void);
static void EndInternal(void);
static void DrawInternal(Node*);
static void UpdateFitWidths(void);
static void UpdateFitWidthContainer(Node*);
static void UpdateFitWidthChild(Node*);
static void UpdateGrowWidths(void);
static void UpdateGrowWidthChildren(Node *node);
static void UpdateTextWrapping(void);
static void UpdateTextWrappingHelper(Node*);
static void UpdateFitHeights(void);
static void UpdateFitHeightContainer(Node*);
static void UpdateFitHeightChild(Node*);
static void UpdateGrowHeights(void);
static void UpdateGrowHeightChildren(Node *node);
static void UpdatePositionsAndAlignment(void);
static void UpdatePositionsAndAlignmentHelper(Node*);
static void SetChildrenPositionsAlongX(Node*);
static void SetChildrenPositionsAlongY(Node*);
static void SetChildrenXBeginAlongX(Node*);
static void SetChildrenYBeginAlongX(Node*);
static void SetChildrenXBeginAlongY(Node*);
static void SetChildrenYBeginAlongY(Node*);
static void SetChildrenXCenterAlongX(Node*);
static void SetChildrenYCenterAlongX(Node*);
static void SetChildrenXCenterAlongY(Node*);
static void SetChildrenYCenterAlongY(Node*);
static void SetChildrenXEndAlongX(Node*);
static void SetChildrenYEndAlongX(Node*);
static void SetChildrenXEndAlongY(Node*);
static void SetChildrenYEndAlongY(Node*);
static int AreEqualApproxF(float a, float b);


void PassThroughDraw(Rectangle *bounds, void *args)
{
    (void)bounds;
    (void)args;
}


void BeginRoot(void)
{
    SET(0, ((Node){
            DefaultNodeLayout,
            (DrawFunc){&PassThroughDraw, NULL},
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
    }));

    BeginRootInternal();
}

void EndRoot(void)
{
    EndRootInternal();
}

void Begin(void)
{
    SET(currentIndex, ((Node){
            DefaultNodeLayout,
            (DrawFunc){&PassThroughDraw, NULL},
            currentNode,
            NULL,
            NULL,
            NULL,
            NULL
    }));

    BeginInternal();
}

void BeginHBox()
{
    SET(currentIndex, ((Node){
            (Layout){
                    {0.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f},
                    {FLT_MAX, FLT_MAX},
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    {SIZE_FLAGS_FIT, SIZE_FLAGS_FIT},
                    {CHILD_ALIGNMENT_BEGIN, CHILD_ALIGNMENT_BEGIN},
                    CHILD_LAYOUT_AXIS_X,
                    0,
                    0
            },
            (DrawFunc){&PassThroughDraw, NULL},
            currentNode,
            NULL,
            NULL,
            NULL,
            NULL
    }));

    BeginInternal();
}

void BeginVBox()
{
    SET(currentIndex, ((Node){
            (Layout){
                    {0.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f},
                    {FLT_MAX, FLT_MAX},
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    {SIZE_FLAGS_FIT, SIZE_FLAGS_FIT},
                    {CHILD_ALIGNMENT_BEGIN, CHILD_ALIGNMENT_BEGIN},
                    CHILD_LAYOUT_AXIS_Y,
                    0,
                    0
            },
            (DrawFunc){&PassThroughDraw, NULL},
            currentNode,
            NULL,
            NULL,
            NULL,
            NULL
    }));

    BeginInternal();
}

void End()
{
    EndInternal();
}

void SetBounds(Rectangle value)
{
    currentNode->layout.bounds = value;
}

void SetPosition(Vector2 value)
{
    currentNode->layout.bounds.x = value.x;
    currentNode->layout.bounds.y = value.y;
}

void SetSize(Vector2 value)
{
    currentNode->layout.bounds.width = value.x;
    currentNode->layout.bounds.height = value.y;
}

void SetMargins(Vector4 value)
{
    currentNode->layout.margins = value;
}

void SetMarginsAll(float value)
{
    currentNode->layout.margins = (Vector4){value, value, value, value};
}

void SetMarginsX(Vector2 value)
{
    currentNode->layout.margins.y = value.y;
    currentNode->layout.margins.w = value.x;
}

void SetMarginsY(Vector2 value)
{
    currentNode->layout.margins.x = value.x;
    currentNode->layout.margins.z = value.y;
}

void SetPadding(Vector4 value)
{
    currentNode->layout.padding = value;
}

void SetPaddingAll(float value)
{
    currentNode->layout.padding = (Vector4){value, value, value, value};
}

void SetPaddingX(Vector2 value)
{
    currentNode->layout.padding.y = value.y;
    currentNode->layout.padding.w = value.x;
}

void SetPaddingY(Vector2 value)
{
    currentNode->layout.padding.x = value.x;
    currentNode->layout.padding.z = value.y;
}

void SetMinSize(Vector2 value)
{
    currentNode->layout.minSize = value;
}

void SetMinSizeX(float value)
{
    currentNode->layout.minSize.x = value;
}

void SetMinSizeY(float value)
{
    currentNode->layout.minSize.y = value;
}

void SetMaxSize(Vector2 value)
{
    currentNode->layout.maxSize = value;
}

void SetMaxSizeX(float value)
{
    currentNode->layout.maxSize.x = value;
}

void SetMaxSizeY(float value)
{
    currentNode->layout.maxSize.y = value;
}

void SetChildSpacing(float value)
{
    currentNode->layout.childSpacing = value;
}

void SetSizeFlags(Vector2UInt8 value)
{
    currentNode->layout.sizeFlags = value;
}

void SetSizeFlagsBoth(uint8_t value)
{
    DEBUG_DATA
    currentNode->layout.sizeFlags = (Vector2UInt8){value, value};
}

void SetSizeFlagX(uint8_t value)
{
    currentNode->layout.sizeFlags.x = value;
}

void SetSizeFlagY(uint8_t value)
{
    currentNode->layout.sizeFlags.y = value;
}

void SetChildAlignment(Vector2UInt8 value)
{
    currentNode->layout.childAlignment = value;
}

void SetChildLayoutAxis(ChildLayoutAxis value)
{
    currentNode->layout.childLayoutAxis = value;
}

void SetText(const char* value, int textLength, float fontSize, float lineSpacing)
{// fixme::
//    currentNode->layout.text = value;
    currentNode->layout.fontSize = fontSize;
    currentNode->layout.lineSpacing = lineSpacing;
//    currentNode->layout.textLength = textLength;
}

void SetDraw(DrawFunc value)
{
    currentNode->drawFunc = value;
}


static void BeginRootInternal() {
    currentNode = GET(0);
    currentIndex = 1;
    DEBUG_DATA
}

static void EndRootInternal() {
    currentNode = NULL;
    treeSize = currentIndex;
    currentIndex = 0;

    AssembleBFSTree();
}

static void AssembleBFSTree() {
    int size = 1;
    bFSTree[0] = GET(0);
    int i = 0;
    while (i < size) {
        Node* child = bFSTree[i]->firstChild;
        while (child) {
            bFSTree[size++] = child;

            child = child->rightSibling;
        }

        ++i;
    }
}

static void BeginInternal() {
    Node* current = GET(currentIndex);

    // if there is a current parent
    if (currentNode) {

        // if the parent has no children, set its first one to the current node
        if (!currentNode->firstChild) {
            currentNode->firstChild = current;
        }

        // if the parent has >= 1 children
        if (currentNode->lastChild) {
            // set the parent's last child's right sibling to the current node
            currentNode->lastChild->rightSibling = current;
        }

        // set the current node's left sibling to the parent's last child
        current->leftSibling = currentNode->lastChild;

        // set the parent's last child to the current node
        currentNode->lastChild = current;
    }

    // set the current parent to the current node for the next Begin() call
    currentNode = current;

    ++currentIndex;
    DEBUG_DATA
}

static void EndInternal() {
    DEBUG_DATA
    currentNode = currentNode->parent;
}


void UpdateLayout()
{
    Layout* rootLayout = &GET(0)->layout;
    rootLayout->minSize = (Vector2){0.0f, 0.0f};
    rootLayout->maxSize = (Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()};
    rootLayout->bounds = (Rectangle){0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
    rootLayout->sizeFlags = (Vector2UInt8){SIZE_FLAGS_FIXED, SIZE_FLAGS_FIXED};

    // note:: update fixed widths would go here, but is unnecessary since the bounds' widths are just the values set at design time
    UpdateFitWidths();
    UpdateGrowWidths();
    // note:: update fixed heights would go here, but is unnecessary since the bounds' heights are just the values set at design time
    UpdateTextWrapping();
    UpdateFitHeights();
    UpdateGrowHeights();
    UpdatePositionsAndAlignment();
}

static void UpdateFitWidths()
{
    int lastIndex = treeSize - 1;

    for (int i = lastIndex; i > 0; --i)
    {
        Node* current = GET(i);

        if (current->firstChild)
        {
            if (current->layout.sizeFlags.x != SIZE_FLAGS_FIXED)
            {
                UpdateFitWidthContainer(current);
            }
        }
        else
        {
            if (current->layout.sizeFlags.x != SIZE_FLAGS_FIXED)
            {
                UpdateFitWidthChild(current);
            }
        }
    }

    // set the root here because its index is 0 and this would cause bit overflow in the for loop above.
    Node* root = GET(0);
    if (root->layout.sizeFlags.x != SIZE_FLAGS_FIXED)
    {
        UpdateFitWidthContainer(root);
    }
}

static void UpdateFitWidthContainer(Node* node)
{
    float width = 0.0f;
    node->layout.minSize.x = 0.0f;
    Node* current = node->firstChild;
    if (node->layout.childLayoutAxis == CHILD_LAYOUT_AXIS_X)
    {
        // add up all the siblings' widths, including their margins on both sides
        int childCount = 0;
        while (current)
        {
            Layout* layout = &current->layout;
            width += layout->bounds.width + layout->margins.y + layout->margins.w;

            // propogate min widths up the tree
            node->layout.minSize.x += layout->minSize.x + layout->margins.y + layout->margins.w;
            ++childCount;

            current = current->rightSibling;
        }

        // add the total spacing from the parent to the total width
        width += node->layout.childSpacing * (float)(childCount - 1);

    }
    else
    {
        // get the max width out of each child, including their padding on both sides
        while (current)
        {
            Layout* layout = &current->layout;
            width = fmaxf(layout->bounds.width + layout->margins.y + layout->margins.w, width);

            current = current->rightSibling;
        }
    }

    width += node->layout.padding.y + node->layout.padding.w;

    // set the parent's width to the calculated width
//        node->layout.bounds.width = width;
    node->layout.bounds.width = fmaxf(width, node->layout.minSize.x);
}

static void UpdateFitWidthChild(Node* node)
{
    node->layout.bounds.width = node->layout.minSize.x;
}

static void UpdateGrowWidths()
{
    for (int i = 0; i < treeSize; ++i)
    {
        Node* current = bFSTree[i];

        if (current->firstChild)
        {
            UpdateGrowWidthChildren(current);
        }
    }
}

static void UpdateGrowWidthChildren(Node* node)
{
    float parentRemainingWidth = FLT_MAX;
    if (!node->layout.hScrollEnabled)
    {
        parentRemainingWidth = (
                node->layout.bounds.width
                - node->layout.padding.w
                - node->layout.padding.y
        );
    }

    if (node->layout.childLayoutAxis == CHILD_LAYOUT_AXIS_X)
    {
        Node *currentChild = node->firstChild;
        int childCount = 0;
        while (currentChild)
        {
            parentRemainingWidth -= (
                    currentChild->layout.bounds.width
                    + currentChild->layout.margins.y
                    + currentChild->layout.margins.w
            );

            ++childCount;

            currentChild = currentChild->rightSibling;
        }

        float childSpacing = node->layout.childSpacing * (float)(childCount - 1);
        parentRemainingWidth -= childSpacing;

        int growableChildCount = 0;
        currentChild = node->firstChild;
        while (currentChild)
        {
            if (currentChild->layout.sizeFlags.x == SIZE_FLAGS_GROW)
            {
                ++growableChildCount;
            }

            currentChild = currentChild->rightSibling;
        }

        while (parentRemainingWidth > FLT_EPSILON && growableChildCount > 0)
        {
            currentChild = node->firstChild;
            float smallestWidth = FLT_MAX;
            float secondSmallestWidth = FLT_MAX;
            float widthToAdd = parentRemainingWidth;
            while (currentChild)
            {
                if (currentChild->layout.sizeFlags.x == SIZE_FLAGS_GROW)
                {
                    if (currentChild->layout.bounds.width < smallestWidth && currentChild->layout.bounds.width < currentChild->layout.maxSize.x)
                    {
                        secondSmallestWidth = smallestWidth;
                        smallestWidth = currentChild->layout.bounds.width;
                    }

                    if (currentChild->layout.bounds.width > smallestWidth && currentChild->layout.bounds.width < currentChild->layout.maxSize.x)
                    {
                        secondSmallestWidth = fminf(secondSmallestWidth, currentChild->layout.bounds.width);
                        widthToAdd = secondSmallestWidth - smallestWidth;
                    }
                }

                currentChild = currentChild->rightSibling;
            }

            widthToAdd = fminf(widthToAdd, parentRemainingWidth / (float)growableChildCount);

            currentChild = node->firstChild;
            while (currentChild)
            {
                if (currentChild->layout.sizeFlags.x == SIZE_FLAGS_GROW)
                {
                    if (AreEqualApproxF(currentChild->layout.bounds.width, smallestWidth) && currentChild->layout.bounds.width < currentChild->layout.maxSize.x)
                    {
                        float clampedWidthToAdd = fminf(widthToAdd, currentChild->layout.maxSize.x - currentChild->layout.bounds.width);
                        currentChild->layout.bounds.width += clampedWidthToAdd;
                        parentRemainingWidth -= clampedWidthToAdd;

                        if (AreEqualApproxF(currentChild->layout.bounds.width, currentChild->layout.maxSize.x)) {
                            --growableChildCount;
                        }
                    }
                }

                currentChild = currentChild->rightSibling;
            }
        }

        float totalChildWidth = 0.0f;
        currentChild = node->firstChild;
        while (currentChild)
        {
            totalChildWidth += (
                    currentChild->layout.bounds.height
                    + currentChild->layout.margins.x
                    + currentChild->layout.margins.z
            );

            currentChild = currentChild->rightSibling;
        }

        node->layout.scrollContentBounds.height = totalChildWidth;
    }
    else
    {
        Node *currentChild = node->firstChild;
        while (currentChild)
        {
            if (currentChild->layout.sizeFlags.x == SIZE_FLAGS_GROW)
            {
                currentChild->layout.bounds.width += (
                        parentRemainingWidth
                        - currentChild->layout.bounds.width
                        - currentChild->layout.margins.w
                        - currentChild->layout.margins.y
                );

                currentChild->layout.bounds.width = fminf(
                        currentChild->layout.bounds.width,
                        currentChild->layout.maxSize.x
                );
            }

            currentChild = currentChild->rightSibling;
        }

        float totalChildWidth = 0.0f;
        currentChild = node->firstChild;
        while (currentChild)
        {
            totalChildWidth = fmaxf(
                    totalChildWidth,
                    currentChild->layout.bounds.width + currentChild->layout.margins.y + currentChild->layout.margins.w
            );

            currentChild = currentChild->rightSibling;
        }

        node->layout.scrollContentBounds.height = totalChildWidth;
    }
}

static void UpdateTextWrapping()
{
    return;
    for (int i = 0; i < treeSize; ++i)
    {
        Node* current = bFSTree[i];

//        if (current->layout.text)
//        {
//            UpdateTextWrappingHelper(current);
//        }
    }
}

static void UpdateTextWrappingHelper(Node* node)
{
    // fixme:: this algorithm only works when all codepoints are 1 byte/char. utf-8 uses variable length codepoints
    //use font size for the width for now.
    // usually you would find the width of each codepoint
    // but that might cause a cache miss due to accessing heap memory
    float fontWidth = node->layout.fontSize;

    //font width * char count = width
    int lineCount = 1;
    int charCountPerLine = (int)(node->layout.bounds.width / fontWidth);
    int i = charCountPerLine - 1;
    int previousI = -1;
//    while (i > previousI && i < node->layout.textLength)
//    {
//        while (i > previousI && !isspace(node->layout.text[i]))
//        {
//            --i;
//        }
//
//        if (i > previousI)
//        {
//            ++lineCount;
//            previousI = i;
//            i += charCountPerLine;
//        }
//    }

    node->layout.bounds.height = fminf(
            (float)(
                    (node->layout.fontSize * lineCount)
                    + (node->layout.lineSpacing * (lineCount - 1))
            ),
            node->layout.maxSize.y
    );
}

static void UpdateFitHeights()
{
    int lastIndex = treeSize - 1;

    for (int i = lastIndex; i > 0; --i)
    {
        Node* current = GET(i);

        if (current->firstChild)
        {
            if (current->layout.sizeFlags.y != SIZE_FLAGS_FIXED)
            {
                UpdateFitHeightContainer(current);
            }
        }
        else
        {
            if (current->layout.sizeFlags.y != SIZE_FLAGS_FIXED)
            {
                UpdateFitHeightChild(current);
            }
        }
    }

    // set the root here because its index is 0 and this would cause bit overflow in the for loop above.
    Node* root = GET(0);
    if (root->layout.sizeFlags.y != SIZE_FLAGS_FIXED)
    {
        UpdateFitHeightContainer(root);
    }
}

static void UpdateFitHeightContainer(Node* node)
{
    float height = 0.0f;
    node->layout.minSize.y = 0.0f;
    Node* current = node->firstChild;
    if (node->layout.childLayoutAxis == CHILD_LAYOUT_AXIS_Y)
    {
        // add up all the siblings' heights, including their margins on both top and bottom
        int childCount = 0;
        while (current)
        {
            Layout* layout = &current->layout;
            height += layout->bounds.height + layout->margins.x + layout->margins.z;

            // propogate min heights up the tree
            node->layout.minSize.y += layout->minSize.y + layout->margins.x + layout->margins.z;
            ++childCount;

            current = current->rightSibling;
        }

        // add the total spacing from the parent to the total width
        height += node->layout.childSpacing * (float)(childCount - 1);

    }
    else
    {
        // get the max height out of each child, including their padding on both top and bottom
        while (current)
        {
            Layout* layout = &current->layout;
            height = fmaxf(layout->bounds.height + layout->margins.x + layout->margins.z, height);

            current = current->rightSibling;
        }
    }

    height += node->layout.padding.x + node->layout.padding.z;

    // set the parent's height to the calculated height
//        node->layout.bounds.width = height;
    node->layout.bounds.height = fmaxf(height, node->layout.minSize.y);
}

static void UpdateFitHeightChild(Node* node)
{
    node->layout.bounds.height = node->layout.minSize.y;
}

static void UpdateGrowHeights()
{
    for (int i = 0; i < treeSize; ++i)
    {
        Node* current = bFSTree[i];

        if (current->firstChild)
        {
            UpdateGrowHeightChildren(bFSTree[i]);
        }
    }
}

static void UpdateGrowHeightChildren(Node* node)
{
//    if (node->parent)
//    {
//        // propogate max sizes down the tree
//        node->layout.maxSize.y = fminf(node->layout.maxSize.y, node->parent->layout.maxSize.y);
//    }

    float parentRemainingHeight = FLT_MAX;
    if (!node->layout.hScrollEnabled) {
        parentRemainingHeight = (
                node->layout.bounds.height
                - node->layout.padding.x
                - node->layout.padding.z
        );
    }

    if (node->layout.childLayoutAxis == CHILD_LAYOUT_AXIS_Y)
    {
        Node *currentChild = node->firstChild;
        int childCount = 0;
        while (currentChild)
        {
            parentRemainingHeight -= (
                    currentChild->layout.bounds.height
                    + currentChild->layout.margins.x
                    + currentChild->layout.margins.z
            );

            ++childCount;

            currentChild = currentChild->rightSibling;
        }

        float childSpacing = node->layout.childSpacing * (float) (childCount - 1);
        parentRemainingHeight -= childSpacing;

        int growableChildCount = 0;
        currentChild = node->firstChild;
        while (currentChild)
        {
            if (currentChild->layout.sizeFlags.y == SIZE_FLAGS_GROW)
            {
                ++growableChildCount;
            }

            currentChild = currentChild->rightSibling;
        }

        while (parentRemainingHeight > FLT_EPSILON && growableChildCount > 0)
        {
            currentChild = node->firstChild;
            float smallestHeight = FLT_MAX;
            float secondSmallestHeight = FLT_MAX;
            float heightToAdd = parentRemainingHeight;
            while (currentChild)
            {
                if (currentChild->layout.sizeFlags.y == SIZE_FLAGS_GROW)
                {
                    if (currentChild->layout.bounds.height < smallestHeight && currentChild->layout.bounds.height < currentChild->layout.maxSize.y)
                    {
                        secondSmallestHeight = smallestHeight;
                        smallestHeight = currentChild->layout.bounds.height;
                    }

                    if (currentChild->layout.bounds.height > smallestHeight && currentChild->layout.bounds.height < currentChild->layout.maxSize.y)
                    {
                        secondSmallestHeight = fminf(secondSmallestHeight, currentChild->layout.bounds.height);
                        heightToAdd = secondSmallestHeight - smallestHeight;
                    }
                }

                currentChild = currentChild->rightSibling;
            }

            heightToAdd = fminf(heightToAdd, parentRemainingHeight / (float)growableChildCount);

            currentChild = node->firstChild;
            while (currentChild)
            {
                if (currentChild->layout.sizeFlags.y == SIZE_FLAGS_GROW)
                {
                    if (AreEqualApproxF(currentChild->layout.bounds.height, smallestHeight) && currentChild->layout.bounds.height < currentChild->layout.maxSize.y)
                    {
                        float clampedWidthToAdd = fminf(heightToAdd, currentChild->layout.maxSize.y - currentChild->layout.bounds.height);
                        currentChild->layout.bounds.height += clampedWidthToAdd;
                        parentRemainingHeight -= clampedWidthToAdd;

                        if (AreEqualApproxF(currentChild->layout.bounds.height, currentChild->layout.maxSize.y))
                        {
                            --growableChildCount;
                        }
                    }
                }

                currentChild = currentChild->rightSibling;
            }
        }

        float totalChildHeight = 0.0f;
        currentChild = node->firstChild;
        while (currentChild)
        {
            totalChildHeight += (
                    currentChild->layout.bounds.height
                    + currentChild->layout.margins.x
                    + currentChild->layout.margins.z
            );

            currentChild = currentChild->rightSibling;
        }

        node->layout.scrollContentBounds.height = totalChildHeight;
    }
    else
    {
        Node *currentChild = node->firstChild;
        while (currentChild)
        {
            if (currentChild->layout.sizeFlags.y == SIZE_FLAGS_GROW)
            {
                currentChild->layout.bounds.height += (
                        parentRemainingHeight
                        - currentChild->layout.bounds.height
                        - currentChild->layout.margins.x
                        - currentChild->layout.margins.z
                );

                currentChild->layout.bounds.height = fminf(
                        currentChild->layout.bounds.height,
                        currentChild->layout.maxSize.y
                );
            }

            currentChild = currentChild->rightSibling;
        }

        float totalChildHeight = 0.0f;
        currentChild = node->firstChild;
        while (currentChild)
        {
            totalChildHeight = fmaxf(
                    totalChildHeight,
                    currentChild->layout.bounds.height + currentChild->layout.margins.x + currentChild->layout.margins.z
            );

            currentChild = currentChild->rightSibling;
        }

        node->layout.scrollContentBounds.height = totalChildHeight;
    }
}

static void UpdatePositionsAndAlignment()
{
    Node* root = GET(0);
    UpdatePositionsAndAlignmentHelper(root);
}

static void UpdatePositionsAndAlignmentHelper(Node* node)
{
    if (node->firstChild)
    {
        switch (node->layout.childLayoutAxis)
        {
            case CHILD_LAYOUT_AXIS_X:
                SetChildrenPositionsAlongX(node);
                break;
            case CHILD_LAYOUT_AXIS_Y:
                SetChildrenPositionsAlongY(node);
                break;
            default:
                break;
        }
    }

    Node* current = node->firstChild;
    while (current)
    {
        UpdatePositionsAndAlignmentHelper(current);

        current = current->rightSibling;
    }
}

static void SetChildrenPositionsAlongX(Node* current)
{
    switch (current->layout.childAlignment.x)
    {
        case CHILD_ALIGNMENT_BEGIN:
            SetChildrenXBeginAlongX(current);
            break;
        case CHILD_ALIGNMENT_CENTER:
            SetChildrenXCenterAlongX(current);
            break;
        case CHILD_ALIGNMENT_END:
            SetChildrenXEndAlongX(current);
            break;
        case CHILD_ALIGNMENT_RADIAL:
            break;
        default:
            SetChildrenXBeginAlongX(current);
            break;
    }

    switch (current->layout.childAlignment.y)
    {
        case CHILD_ALIGNMENT_BEGIN:
            SetChildrenYBeginAlongX(current);
            break;
        case CHILD_ALIGNMENT_CENTER:
            SetChildrenYCenterAlongX(current);
            break;
        case CHILD_ALIGNMENT_END:
            SetChildrenYEndAlongX(current);
            break;
        case CHILD_ALIGNMENT_RADIAL:
            break;
        default:
            SetChildrenYBeginAlongX(current);
            break;
    }
}

static void SetChildrenXBeginAlongX(Node* current)
{
    int childCount = 0;
    Node *currentChild = current->firstChild;
    while (currentChild)
    {
        ++childCount;

        currentChild = currentChild->rightSibling;
    }

    float currentX = current->layout.bounds.x + current->layout.padding.w;

    currentChild = current->firstChild;
    while (currentChild)
    {
        float leftPaddingAdjustmentX = currentChild->layout.margins.w;
        if (currentChild->leftSibling)
        {
            leftPaddingAdjustmentX += currentChild->leftSibling->layout.margins.y + current->layout.childSpacing;
        }

        currentChild->layout.bounds.x = currentX + leftPaddingAdjustmentX;
        currentX += currentChild->layout.bounds.width + leftPaddingAdjustmentX;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenYBeginAlongX(Node* current)
{
    float currentY = current->layout.bounds.y + current->layout.padding.x;
    Node *currentChild = current->firstChild;
    while (currentChild)
    {
        float topPaddingAdjustment = currentChild->layout.margins.x;
        currentChild->layout.bounds.y = currentY + topPaddingAdjustment;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenXCenterAlongX(Node* current)
{
    float childWidths = 0.0f;
    int childCount = 0;
    Node *currentChild = current->firstChild;

    childWidths += current->layout.padding.y + current->layout.padding.w;
    while (currentChild)
    {
        childWidths += (
                currentChild->layout.bounds.width
                + currentChild->layout.margins.y
                + currentChild->layout.margins.w
        );

        ++childCount;

        currentChild = currentChild->rightSibling;
    }

    float childSpacing = current->layout.childSpacing * (float) (childCount - 1);

    float currentX = 0.0f;
    switch (current->layout.sizeFlags.x)
    {
        case SIZE_FLAGS_FIT:
            currentX = current->layout.bounds.x + current->layout.padding.w;
            break;
        case SIZE_FLAGS_GROW:
        case SIZE_FLAGS_FIXED:
            currentX = current->layout.bounds.x + current->layout.padding.w + ((current->layout.bounds.width - childWidths - childSpacing) / 2.0f);
            break;
        default:
            break;
    }

    currentChild = current->firstChild;
    while (currentChild)
    {
        float leftPaddingAdjustmentX = currentChild->layout.margins.w;
        if (currentChild->leftSibling)
        {
            leftPaddingAdjustmentX += current->layout.childSpacing;
        }

        currentChild->layout.bounds.x = currentX + leftPaddingAdjustmentX;
        currentX += currentChild->layout.bounds.width + leftPaddingAdjustmentX;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenYCenterAlongX(Node* current)
{
    float currentYParentPart = 0.0f;

    currentYParentPart = current->layout.bounds.y + (current->layout.bounds.height / 2.0f);

    Node *currentChild = current->firstChild;
    while (currentChild)
    {
        float currentY = (
                currentYParentPart
                - (currentChild->layout.bounds.height / 2.0f)
        );

        currentChild->layout.bounds.y = currentY;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenXEndAlongX(Node* current)
{
    int childCount = 0;
    Node *currentChild = current->firstChild;
    while (currentChild)
    {
        ++childCount;

        currentChild = currentChild->rightSibling;
    }

    float currentX = current->layout.bounds.x + current->layout.bounds.width - current->layout.padding.y;

    currentChild = current->lastChild;
    while (currentChild)
    {
        float rightPaddingAdjustmentX = currentChild->layout.margins.y;
        if (currentChild->rightSibling)
        {
            rightPaddingAdjustmentX += currentChild->rightSibling->layout.margins.w + current->layout.childSpacing;
        }

        currentX -= currentChild->layout.bounds.width + rightPaddingAdjustmentX;
        currentChild->layout.bounds.x = currentX;

        currentChild = currentChild->leftSibling;
    }
}

static void SetChildrenYEndAlongX(Node* current)
{
    float currentYParentPart = current->layout.bounds.y + current->layout.bounds.height - current->layout.padding.z;

    Node *currentChild = current->firstChild;
    while (currentChild)
    {
        float currentY = currentYParentPart - currentChild->layout.bounds.height - currentChild->layout.margins.z;
        currentChild->layout.bounds.y = currentY;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenPositionsAlongY(Node* current)
{
    switch (current->layout.childAlignment.x)
    {
        case CHILD_ALIGNMENT_BEGIN:
            SetChildrenXBeginAlongY(current);
            break;
        case CHILD_ALIGNMENT_CENTER:
            SetChildrenXCenterAlongY(current);
            break;
        case CHILD_ALIGNMENT_END:
            SetChildrenXEndAlongY(current);
            break;
        case CHILD_ALIGNMENT_RADIAL:
            break;
        default:
            SetChildrenXBeginAlongY(current);
            break;
    }

    switch (current->layout.childAlignment.y)
    {
        case CHILD_ALIGNMENT_BEGIN:
            SetChildrenYBeginAlongY(current);
            break;
        case CHILD_ALIGNMENT_CENTER:
            SetChildrenYCenterAlongY(current);
            break;
        case CHILD_ALIGNMENT_END:
            SetChildrenYEndAlongY(current);
            break;
        case CHILD_ALIGNMENT_RADIAL:
            break;
        default:
            SetChildrenYBeginAlongY(current);
            break;
    }
}

static void SetChildrenYBeginAlongY(Node* current)
{
    int childCount = 0;
    Node *currentChild = current->firstChild;
    while (currentChild)
{
        ++childCount;

        currentChild = currentChild->rightSibling;
    }

    float currentY = current->layout.bounds.y + current->layout.padding.x;

    currentChild = current->firstChild;
    while (currentChild)
{
        float topMarginAdjustment = currentChild->layout.margins.x;
        if (currentChild->leftSibling)
{
            topMarginAdjustment += currentChild->leftSibling->layout.margins.y + current->layout.childSpacing;
        }

        currentChild->layout.bounds.y = currentY + topMarginAdjustment;
        currentY += currentChild->layout.bounds.height + topMarginAdjustment;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenXBeginAlongY(Node* current)
{
    float currentX = current->layout.bounds.x + current->layout.padding.w;
    Node *currentChild = current->firstChild;
    while (currentChild)
{
        float leftMarginAdjustment = currentChild->layout.margins.w;
        currentChild->layout.bounds.x = currentX + leftMarginAdjustment;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenYCenterAlongY(Node* current)
{
    float childHeights = 0.0f;
    int childCount = 0;
    Node *currentChild = current->firstChild;

    childHeights += current->layout.padding.x + current->layout.padding.z;
    while (currentChild)
{
        childHeights += (
                currentChild->layout.bounds.height
                + currentChild->layout.margins.x
                + currentChild->layout.margins.z
        );

        ++childCount;

        currentChild = currentChild->rightSibling;
    }

    float childSpacing = current->layout.childSpacing * (float) (childCount - 1);

    float currentY = 0.0f;
    switch (current->layout.sizeFlags.y)
{
        case SIZE_FLAGS_FIT:
            currentY = current->layout.bounds.y + current->layout.padding.x;
            break;
        case SIZE_FLAGS_GROW:
        case SIZE_FLAGS_FIXED:
            currentY = current->layout.bounds.y + current->layout.padding.x + ((current->layout.bounds.height - childHeights - childSpacing) / 2.0f);
            break;
        default:
            break;
    }

    currentChild = current->firstChild;
    while (currentChild)
{
        float topMarginAdjustment = currentChild->layout.margins.x;
        if (currentChild->leftSibling)
{
            topMarginAdjustment += current->layout.childSpacing;
        }

        currentChild->layout.bounds.y = currentY + topMarginAdjustment;
        currentY += topMarginAdjustment + currentChild->layout.bounds.height;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenXCenterAlongY(Node* current)
{
    float currentXParentPart = 0.0f;

    currentXParentPart = current->layout.bounds.x + (current->layout.bounds.width / 2.0f);

    Node *currentChild = current->firstChild;
    while (currentChild)
    {
        float currentX = (
                currentXParentPart
                - (currentChild->layout.bounds.width / 2.0f)
        );

        currentChild->layout.bounds.x = currentX;

        currentChild = currentChild->rightSibling;
    }
}

static void SetChildrenYEndAlongY(Node* current)
{
    int childCount = 0;
    Node *currentChild = current->firstChild;
    while (currentChild)
    {
        ++childCount;

        currentChild = currentChild->rightSibling;
    }

    float currentY = current->layout.bounds.y + current->layout.bounds.height - current->layout.padding.z;

    currentChild = current->lastChild;
    while (currentChild)
    {
        float bottomMarginAdjustment = currentChild->layout.margins.z;
        if (currentChild->rightSibling)
        {
            bottomMarginAdjustment += currentChild->rightSibling->layout.margins.x + current->layout.childSpacing;
        }

        currentY -= currentChild->layout.bounds.height + bottomMarginAdjustment;
        currentChild->layout.bounds.y = currentY;

        currentChild = currentChild->leftSibling;
    }
}

static void SetChildrenXEndAlongY(Node* current)
{
    float currentXParentPart = current->layout.bounds.x + current->layout.bounds.width - current->layout.padding.y;

    Node *currentChild = current->firstChild;
    while (currentChild)
    {
        float currentX = currentXParentPart - currentChild->layout.bounds.width - currentChild->layout.margins.y;
        currentChild->layout.bounds.x = currentX;

        currentChild = currentChild->rightSibling;
    }
}

void Draw()
{
    DrawInternal(GET(0));
}

static void DrawInternal(Node* current)
{
    int isScrollContainer = current->layout.hScrollEnabled || current->layout.vScrollEnabled;
    if (isScrollContainer)
    {
        BeginScissorMode(
                (int)current->layout.bounds.x,
                (int)current->layout.bounds.y,
                (int)current->layout.bounds.width,
                (int)current->layout.bounds.height
        );
    }

    current->drawFunc.draw(&current->layout.bounds, current->drawFunc.args);

    Node* currentChild = current->firstChild;
    while (currentChild)
    {
        DrawInternal(currentChild);
        currentChild = currentChild->rightSibling;
    }

    if (isScrollContainer)
    {
        EndScissorMode();
    }
}


static int AreEqualApproxF(float a, float b)
{
    return fabsf(a - b) < FLT_EPSILON;
}


#undef GET
#undef SET
#undef DEBUG_DATA

#if UNDEF_CAPACITY
#undef RLAUTO_LAYOUT_TREE_CAPACITY
#endif

#if UNDEF_INDEX_MODE
#undef RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE
#endif