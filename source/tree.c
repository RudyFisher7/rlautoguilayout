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

#include <math.h>


#ifndef UNDEF_SIZE
#define UNDEF_SIZE 1
#endif

#ifndef UNDEF_INDEX_MODE
#define UNDEF_INDEX_MODE 1
#endif

#ifndef RLAUTO_LAYOUT_TREE_SIZE
#define RLAUTO_LAYOUT_TREE_SIZE 32
#endif

#ifndef RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE
#define RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE 1
#endif


#if RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE
#define GET(i) (i >= 0 && i < RLAUTO_LAYOUT_TREE_SIZE) ? &tree[i] : NULL
#else
#define GET(i) &tree[i]
#endif


static const int TreeSize = RLAUTO_LAYOUT_TREE_SIZE;
static Node tree[RLAUTO_LAYOUT_TREE_SIZE];
static Node* bFSTree[RLAUTO_LAYOUT_TREE_SIZE];


static void UpdateFitWidthContainer(Node*);
static void UpdateGrowWidthContainer(Node*);



static void DrawInternal(Node* current) {
    current->drawFunc.draw(current->layout.bounds);

    Node* currentChild = current->firstChild;
    while (currentChild) {
        DrawInternal(currentChild);
        currentChild = currentChild->rightSibling;
    }
}

static void UpdateFitWidths() {
    int last_index = TreeSize - 1;

    for (int i = last_index; i > 0; --i) {
        Node* current = GET(i);

        if (current->firstChild) {
            if (current->layout.sizeFlags.x != SIZE_FLAGS_FIXED) {
                UpdateFitWidthContainer(current);
            }
        } else {
            // leaf
        }
    }

    // set the root here because its index is 0 and this would cause bit overflow in the for loop above.
    Node* root = GET(0);
    if (root->layout.sizeFlags.x != SIZE_FLAGS_FIXED) {
        UpdateFitWidthContainer(root);
    }
}

static void UpdateFitWidthContainer(Node* node) {
    float width = 0.0f;
    node->layout.minSize.x = 0.0f;
    Node* current = node->firstChild;
    if (node->layout.childLayoutAxis == CHILD_LAYOUT_AXIS_X) {
        // add up all the siblings' widths, including their margins on both sides
        int childCount = 0;
        while (current) {
            Layout* layout = &current->layout;
            width += layout->bounds.width + layout->margins.y + layout->margins.w;

            // propogate min widths up the tree
            node->layout.minSize.x += layout->minSize.x + layout->margins.y + layout->margins.w;
            ++childCount;

            current = current->rightSibling;
        }

        // add the total spacing from the parent to the total width
        width += node->layout.childSpacing * (float)(childCount - 1u);

    } else {
        // get the max width out of each child, including their padding on both sides
        while (current) {
            Layout* layout = &current->layout;
            width = fmaxf(layout->bounds.width + layout->margins.y + layout->margins.w, width);

            current = current->rightSibling;
        }
    }

    width += node->layout.padding.y + node->layout.padding.w;

    // set the parent's width to the calculated width
//        node->data.layout.bounds.width = width;
    node->layout.bounds.width = fmaxf(width, node->layout.minSize.x);
}

static void UpdateGrowWidths() {
    for (int i = 0; i < TreeSize; ++i) {
        Node* current = bFSTree[i];

        if (current->firstChild) {
            UpdateGrowWidthContainer(current);
        }
    }
}

static void UpdateGrowWidthContainer(Node* node) {
    float parent_remaining_width = (
            node->layout.bounds.width
            - node->layout.padding.w
            - node->layout.padding.y
    );

    if (node->layout.childLayoutAxis == CHILD_LAYOUT_AXIS_X) {
        Node *currentChild = node->firstChild;
        int childCount = 0;
        while (currentChild) {
            parent_remaining_width -= (
                    currentChild->layout.bounds.width
                    + currentChild->layout.margins.w
                    + currentChild->layout.margins.y
            );

            ++childCount;

            currentChild = currentChild->rightSibling;
        }

        float child_spacing = node->layout.childSpacing * (float)(childCount - 1u);
        parent_remaining_width -= child_spacing;

        int growableChildCount = 0;
        currentChild = node->firstChild;
        while (currentChild) {
            if (currentChild->layout.sizeFlags.x == SIZE_FLAGS_GROW) {
                ++growableChildCount;
            }

            current_child = current_child->right_sibling;
        }

        while (parent_remaining_width > FLT_EPSILON && growable_child_count > 0) {
            current_child = node->first_child;
            float smallest_width = FLT_MAX;
            float second_smallest_width = FLT_MAX;
            float width_to_add = parent_remaining_width;
            while (current_child) {
                if (current_child->data.layout.size_flags.x == SIZE_FLAGS_GROW) {
                    if (current_child->data.layout.bounds.width < smallest_width && current_child->data.layout.bounds.width < current_child->data.layout.max_size.x) {
                        second_smallest_width = smallest_width;
                        smallest_width = current_child->data.layout.bounds.width;
                    }

                    if (current_child->data.layout.bounds.width > smallest_width && current_child->data.layout.bounds.width < current_child->data.layout.max_size.x) {
                        second_smallest_width = fminf(second_smallest_width, current_child->data.layout.bounds.width);
                        width_to_add = second_smallest_width - smallest_width;
                    }
                }

                current_child = current_child->right_sibling;
            }

            width_to_add = fminf(width_to_add, parent_remaining_width / (float)growable_child_count);

            current_child = node->first_child;
            while (current_child) {
                if (current_child->data.layout.size_flags.x == SIZE_FLAGS_GROW) {
                    if (FloatEquals(current_child->data.layout.bounds.width, smallest_width) && current_child->data.layout.bounds.width < current_child->data.layout.max_size.x) {
                        float clamped_width_to_add = fminf(width_to_add, current_child->data.layout.max_size.x - current_child->data.layout.bounds.width);
                        current_child->data.layout.bounds.width += clamped_width_to_add;
                        parent_remaining_width -= clamped_width_to_add;

                        if (FloatEquals(current_child->data.layout.bounds.width, current_child->data.layout.max_size.x)) {
                            --growable_child_count;
                        }
                    }
                }

                current_child = current_child->right_sibling;
            }
        }
    } else {
        GuiNode *current_child = node->first_child;
        while (current_child) {
            if (current_child->data.layout.size_flags.x == SIZE_FLAGS_GROW) {
                current_child->data.layout.bounds.width += (
                        parent_remaining_width
                        - current_child->data.layout.bounds.width
                        - current_child->data.layout.margins.w
                        - current_child->data.layout.margins.y
                );

                current_child->data.layout.bounds.width = fminf(
                        current_child->data.layout.bounds.width,
                        current_child->data.layout.max_size.x
                );
            }

            current_child = current_child->right_sibling;
        }
    }
}

static void _update_text_wrapping() {
    for (int i = 0; i < this->_arena_size; ++i) {
        GuiNode* current = this->_bfs_queue[i];

        if (current->data.layout.text) {
            _update_text_wrapping(current);
        }
    }
}

static void _update_text_wrapping(GuiNode* node) {
    // fixme:: this algorithm only works when all codepoints are 1 byte/char. utf-8 uses variable length codepoints
    //use font size for the width for now.
    // usually you would find the width of each codepoint
    // but that might cause a cache miss due to accessing heap memory
    float font_width = node->data.layout.font_size;

    //font width * char count = width
    int line_count = 1;
    int char_count_per_line = (int)(node->data.layout.bounds.width / font_width);
    int i = char_count_per_line - 1;
    int previous_i = -1;
    while (i > previous_i && i < node->data.layout.text_length) {
        while (i > previous_i && !isspace(node->data.layout.text[i])) {
            --i;
        }

        if (i > previous_i) {
            ++line_count;
            previous_i = i;
            i += char_count_per_line;
        }
    }

    node->data.layout.bounds.height = fminf(
            (float)(
                    (node->data.layout.font_size * line_count)
                    + (node->data.layout.line_spacing * (line_count - 1))
            ),
            node->data.layout.max_size.y
    );
}

static void _update_fit_heights() {
    int last_index = this->_arena_size - 1u;

    for (int i = last_index; i > 0; --i) {
        GuiNode* current = &this->_get(i);

        if (current->first_child) {
            if (current->data.layout.size_flags.y != SIZE_FLAGS_FIXED) {
                _update_fit_height_container(current);
            }
        } else {
            // leaf
        }
    }

    // set the root here because its index is 0 and this would cause bit overflow in the for loop above.
    GuiNode* root = this->Root();
    if (root->data.layout.size_flags.y != SIZE_FLAGS_FIXED) {
        _update_fit_height_container(root);
    }
}

static void _update_fit_height_container(GuiNode* node) {
    float height = 0.0f;
    node->data.layout.min_size.y = 0.0f;
    GuiNode* current = node->first_child;
    if (node->data.layout.child_layout_axis == CHILD_LAYOUT_AXIS_Y) {
        // add up all the siblings' heights, including their margins on both top and bottom
        int child_count = 0;
        while (current) {
            Layout* layout = &current->data.layout;
            height += layout->bounds.height + layout->margins.x + layout->margins.z;

            // propogate min heights up the tree
            node->data.layout.min_size.y += layout->min_size.x + layout->margins.x + layout->margins.y;
            ++child_count;

            current = current->right_sibling;
        }

        // add the total spacing from the parent to the total width
        height += node->data.layout.child_spacing * (float)(child_count - 1u);

    } else {
        // get the max height out of each child, including their padding on both top and bottom
        while (current) {
            Layout* layout = &current->data.layout;
            height = fmaxf(layout->bounds.height + layout->margins.x + layout->margins.z, height);

            current = current->right_sibling;
        }
    }

    height += node->data.layout.padding.x + node->data.layout.padding.z;

    // set the parent's height to the calculated height
//        node->data.layout.bounds.width = height;
    node->data.layout.bounds.height = fmaxf(height, node->data.layout.min_size.y);
}

static void _update_grow_heights() {
    for (int i = 0; i < this->_arena_size; ++i) {
        GuiNode* current = this->_bfs_queue[i];

        if (current->first_child) {
            _update_grow_height_container(this->_bfs_queue[i]);
        }
    }
}

static void _update_grow_height_container(GuiNode* node) {
    float parent_remaining_height = (
            node->data.layout.bounds.height
            - node->data.layout.padding.x
            - node->data.layout.padding.z
    );

    if (node->data.layout.child_layout_axis == CHILD_LAYOUT_AXIS_Y) {
        GuiNode *current_child = node->first_child;
        int child_count = 0;
        while (current_child) {
            parent_remaining_height -= (
                    current_child->data.layout.bounds.height
                    + current_child->data.layout.margins.x
                    + current_child->data.layout.margins.z
            );

            ++child_count;

            current_child = current_child->right_sibling;
        }

        float child_spacing = node->data.layout.child_spacing * (float) (child_count - 1u);
        parent_remaining_height -= child_spacing;

        int growable_child_count = 0;
        current_child = node->first_child;
        while (current_child) {
            if (current_child->data.layout.size_flags.y == SIZE_FLAGS_GROW) {
                ++growable_child_count;
            }

            current_child = current_child->right_sibling;
        }

        while (parent_remaining_height > FLT_EPSILON && growable_child_count > 0) {
            current_child = node->first_child;
            float smallest_height = FLT_MAX;
            float second_smallest_height = FLT_MAX;
            float height_to_add = parent_remaining_height;
            while (current_child) {
                if (current_child->data.layout.size_flags.y == SIZE_FLAGS_GROW) {
                    if (current_child->data.layout.bounds.height < smallest_height && current_child->data.layout.bounds.height < current_child->data.layout.max_size.y) {
                        second_smallest_height = smallest_height;
                        smallest_height = current_child->data.layout.bounds.height;
                    }

                    if (current_child->data.layout.bounds.height > smallest_height && current_child->data.layout.bounds.height < current_child->data.layout.max_size.y) {
                        second_smallest_height = fminf(second_smallest_height, current_child->data.layout.bounds.height);
                        height_to_add = second_smallest_height - smallest_height;
                    }
                }

                current_child = current_child->right_sibling;
            }

            height_to_add = fminf(height_to_add, parent_remaining_height / (float)growable_child_count);

            current_child = node->first_child;
            while (current_child) {
                if (current_child->data.layout.size_flags.y == SIZE_FLAGS_GROW) {
                    if (FloatEquals(current_child->data.layout.bounds.height, smallest_height) && current_child->data.layout.bounds.height < current_child->data.layout.max_size.y) {
                        float clamped_width_to_add = fminf(height_to_add, current_child->data.layout.max_size.y - current_child->data.layout.bounds.height);
                        current_child->data.layout.bounds.height += clamped_width_to_add;
                        parent_remaining_height -= clamped_width_to_add;

                        if (FloatEquals(current_child->data.layout.bounds.height, current_child->data.layout.max_size.y)) {
                            --growable_child_count;
                        }
                    }
                }

                current_child = current_child->right_sibling;
            }
        }
    } else {
        GuiNode *current_child = node->first_child;
        while (current_child) {
            if (current_child->data.layout.size_flags.y == SIZE_FLAGS_GROW) {
                current_child->data.layout.bounds.height += (
                        parent_remaining_height
                        - current_child->data.layout.bounds.height
                        - current_child->data.layout.margins.x
                        - current_child->data.layout.margins.z
                );

                current_child->data.layout.bounds.height = fminf(
                        current_child->data.layout.bounds.height,
                        current_child->data.layout.max_size.y
                );
            }

            current_child = current_child->right_sibling;
        }
    }
}

static void _update_positions_and_alignment() {
    GuiNode* root = this->Root();
    _update_positions_and_alignment(root);
}

static void _update_positions_and_alignment(GuiNode* node) {
    if (node->first_child) {
        switch (node->data.layout.child_layout_axis) {
            case CHILD_LAYOUT_AXIS_X:
                _set_children_positions_along_x(node);
                break;
            case CHILD_LAYOUT_AXIS_Y:
                _set_children_positions_along_y(node);
                break;
            default:
                break;
        }
    }

    GuiNode* current = node->first_child;
    while (current) {
        _update_positions_and_alignment(current);

        current = current->right_sibling;
    }
}

static void _set_children_positions_along_x(GuiNode* current) {
    switch (current->data.layout.child_alignment.x) {
        case CHILD_ALIGNMENT_BEGIN:
            _set_children_x_begin_along_x(current);
            break;
        case CHILD_ALIGNMENT_CENTER:
            _set_children_x_center_along_x(current);
            break;
        case CHILD_ALIGNMENT_END:
            _set_children_x_end_along_x(current);
            break;
        case CHILD_ALIGNMENT_RADIAL:
            break;
        default:
            _set_children_x_begin_along_x(current);
            break;
    }

    switch (current->data.layout.child_alignment.y) {
        case CHILD_ALIGNMENT_BEGIN:
            _set_children_y_begin_along_x(current);
            break;
        case CHILD_ALIGNMENT_CENTER:
            _set_children_y_center_along_x(current);
            break;
        case CHILD_ALIGNMENT_END:
            _set_children_y_end_along_x(current);
            break;
        case CHILD_ALIGNMENT_RADIAL:
            break;
        default:
            _set_children_y_begin_along_x(current);
            break;
    }
}

static void _set_children_x_begin_along_x(GuiNode* current) {
    int child_count = 0;
    GuiNode *current_child = current->first_child;
    while (current_child) {
        ++child_count;

        current_child = current_child->right_sibling;
    }

    float current_x = current->data.layout.bounds.x + current->data.layout.padding.w;

    current_child = current->first_child;
    while (current_child) {
        float left_padding_adjustment_x = current_child->data.layout.margins.w;
        if (current_child->left_sibling) {
            left_padding_adjustment_x += current_child->left_sibling->data.layout.margins.y + current->data.layout.child_spacing;
        }

        current_child->data.layout.bounds.x = current_x + left_padding_adjustment_x;
        current_x += current_child->data.layout.bounds.width + left_padding_adjustment_x;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_y_begin_along_x(GuiNode* current) {
    float current_y = current->data.layout.bounds.y + current->data.layout.padding.x;
    GuiNode *current_child = current->first_child;
    while (current_child) {
        float top_padding_adjustment = current_child->data.layout.margins.x;
        current_child->data.layout.bounds.y = current_y + top_padding_adjustment;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_x_center_along_x(GuiNode* current) {
    float child_widths = 0.0f;
    int child_count = 0;
    GuiNode *current_child = current->first_child;

    child_widths += current->data.layout.padding.y + current->data.layout.padding.w;
    while (current_child) {
        child_widths += (
                current_child->data.layout.bounds.width
                + current_child->data.layout.margins.y
                + current_child->data.layout.margins.w
        );

        ++child_count;

        current_child = current_child->right_sibling;
    }

    float child_spacing = current->data.layout.child_spacing * (float) (child_count - 1u);

    float current_x = 0.0f;
    switch (current->data.layout.size_flags.x) {
        case SIZE_FLAGS_FIT:
            current_x = current->data.layout.bounds.x + current->data.layout.padding.w;
            break;
        case SIZE_FLAGS_GROW:
        case SIZE_FLAGS_FIXED:
            current_x = current->data.layout.bounds.x + current->data.layout.padding.w + ((current->data.layout.bounds.width - child_widths - child_spacing) / 2.0f);
            break;
        default:
            break;
    }

    current_child = current->first_child;
    while (current_child) {
        float left_padding_adjustment_x = current_child->data.layout.margins.w;
        if (current_child->left_sibling) {
            left_padding_adjustment_x += current->data.layout.child_spacing;
        }

        current_child->data.layout.bounds.x = current_x + left_padding_adjustment_x;
        current_x += current_child->data.layout.bounds.width + left_padding_adjustment_x;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_y_center_along_x(GuiNode* current) {
    float current_y_parent_part = 0.0f;

    current_y_parent_part = current->data.layout.bounds.y + (current->data.layout.bounds.height / 2.0f);

    GuiNode *current_child = current->first_child;
    while (current_child) {
        float current_y = (
                current_y_parent_part
                - (current_child->data.layout.bounds.height / 2.0f)
        );

        current_child->data.layout.bounds.y = current_y;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_x_end_along_x(GuiNode* current) {
    int child_count = 0;
    GuiNode *current_child = current->first_child;
    while (current_child) {
        ++child_count;

        current_child = current_child->right_sibling;
    }

    float current_x = current->data.layout.bounds.x + current->data.layout.bounds.width - current->data.layout.padding.y;

    current_child = current->last_child;
    while (current_child) {
        float right_padding_adjustment_x = current_child->data.layout.margins.y;
        if (current_child->right_sibling) {
            right_padding_adjustment_x += current_child->right_sibling->data.layout.margins.w + current->data.layout.child_spacing;
        }

        current_x -= current_child->data.layout.bounds.width + right_padding_adjustment_x;
        current_child->data.layout.bounds.x = current_x;

        current_child = current_child->left_sibling;
    }
}

static void _set_children_y_end_along_x(GuiNode* current) {
    float current_y_parent_part = current->data.layout.bounds.y + current->data.layout.bounds.height - current->data.layout.padding.z;

    GuiNode *current_child = current->first_child;
    while (current_child) {
        float current_y = current_y_parent_part - current_child->data.layout.bounds.height - current_child->data.layout.margins.z;
        current_child->data.layout.bounds.y = current_y;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_positions_along_y(GuiNode* current) {
    switch (current->data.layout.child_alignment.x) {
        case CHILD_ALIGNMENT_BEGIN:
            _set_children_x_begin_along_y(current);
            break;
        case CHILD_ALIGNMENT_CENTER:
            _set_children_x_center_along_y(current);
            break;
        case CHILD_ALIGNMENT_END:
            _set_children_x_end_along_y(current);
            break;
        case CHILD_ALIGNMENT_RADIAL:
            break;
        default:
            _set_children_x_begin_along_y(current);
            break;
    }

    switch (current->data.layout.child_alignment.y) {
        case CHILD_ALIGNMENT_BEGIN:
            _set_children_y_begin_along_y(current);
            break;
        case CHILD_ALIGNMENT_CENTER:
            _set_children_y_center_along_y(current);
            break;
        case CHILD_ALIGNMENT_END:
            _set_children_y_end_along_y(current);
            break;
        case CHILD_ALIGNMENT_RADIAL:
            break;
        default:
            _set_children_y_begin_along_y(current);
            break;
    }
}

static void _set_children_y_begin_along_y(GuiNode* current) {
    int child_count = 0;
    GuiNode *current_child = current->first_child;
    while (current_child) {
        ++child_count;

        current_child = current_child->right_sibling;
    }

    float current_y = current->data.layout.bounds.y + current->data.layout.padding.x;

    current_child = current->first_child;
    while (current_child) {
        float top_magin_adjustment = current_child->data.layout.margins.x;
        if (current_child->left_sibling) {
            top_magin_adjustment += current_child->left_sibling->data.layout.margins.y + current->data.layout.child_spacing;
        }

        current_child->data.layout.bounds.y = current_y + top_magin_adjustment;
        current_y += current_child->data.layout.bounds.height + top_magin_adjustment;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_x_begin_along_y(GuiNode* current) {
    float current_x = current->data.layout.bounds.x + current->data.layout.padding.w;
    GuiNode *current_child = current->first_child;
    while (current_child) {
        float left_margin_adjustment = current_child->data.layout.margins.w;
        current_child->data.layout.bounds.x = current_x + left_margin_adjustment;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_y_center_along_y(GuiNode* current) {
    float child_heights = 0.0f;
    int child_count = 0;
    GuiNode *current_child = current->first_child;

    child_heights += current->data.layout.padding.x + current->data.layout.padding.z;
    while (current_child) {
        child_heights += (
                current_child->data.layout.bounds.height
                + current_child->data.layout.margins.x
                + current_child->data.layout.margins.z
        );

        ++child_count;

        current_child = current_child->right_sibling;
    }

    float child_spacing = current->data.layout.child_spacing * (float) (child_count - 1u);

    float current_y = 0.0f;
    switch (current->data.layout.size_flags.y) {
        case SIZE_FLAGS_FIT:
            current_y = current->data.layout.bounds.y + current->data.layout.padding.x;
            break;
        case SIZE_FLAGS_GROW:
        case SIZE_FLAGS_FIXED:
            current_y = current->data.layout.bounds.y + current->data.layout.padding.x + ((current->data.layout.bounds.height - child_heights - child_spacing) / 2.0f);
            break;
        default:
            break;
    }

    current_child = current->first_child;
    while (current_child) {
        float top_margin_adjustment = current_child->data.layout.margins.x;
        if (current_child->left_sibling) {
            top_margin_adjustment += current->data.layout.child_spacing;
        }

        current_child->data.layout.bounds.y = current_y + top_margin_adjustment;
        current_y += top_margin_adjustment + current_child->data.layout.bounds.height;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_x_center_along_y(GuiNode* current) {
    float current_x_parent_part = 0.0f;

    current_x_parent_part = current->data.layout.bounds.x + (current->data.layout.bounds.width / 2.0f);

    GuiNode *current_child = current->first_child;
    while (current_child) {
        float current_x = (
                current_x_parent_part
                - (current_child->data.layout.bounds.width / 2.0f)
        );

        current_child->data.layout.bounds.x = current_x;

        current_child = current_child->right_sibling;
    }
}

static void _set_children_y_end_along_y(GuiNode* current) {
    int child_count = 0;
    GuiNode *current_child = current->first_child;
    while (current_child) {
        ++child_count;

        current_child = current_child->right_sibling;
    }

    float current_y = current->data.layout.bounds.y + current->data.layout.bounds.height - current->data.layout.padding.z;

    current_child = current->last_child;
    while (current_child) {
        float bottom_margin_adjustment = current_child->data.layout.margins.z;
        if (current_child->right_sibling) {
            bottom_margin_adjustment += current_child->right_sibling->data.layout.margins.x + current->data.layout.child_spacing;
        }

        current_y -= current_child->data.layout.bounds.width + bottom_margin_adjustment;
        current_child->data.layout.bounds.y = current_y;

        current_child = current_child->left_sibling;
    }
}

static void _set_children_x_end_along_y(GuiNode* current) {
    float current_x_parent_part = current->data.layout.bounds.x + current->data.layout.bounds.width - current->data.layout.padding.y;

    GuiNode *current_child = current->first_child;
    while (current_child) {
        float current_x = current_x_parent_part - current_child->data.layout.bounds.width - current_child->data.layout.margins.y;
        current_child->data.layout.bounds.x = current_x;

        current_child = current_child->right_sibling;
    }
}


#undef GET

#if UNDEF_SIZE
#undef RLAUTO_LAYOUT_TREE_SIZE
#endif

#if UNDEF_INDEX_MODE
#undef RLAUTO_LAYOUT_TREE_INDEX_MODE_SAFE
#endif