///*
// * MIT License
// *
// * Copyright (c) 2025 Rudy Fisher (kiyasui-hito)
// *
// * Permission is hereby granted, free of charge, to any person obtaining a copy
// * of this software and associated documentation files (the "Software"), to deal
// * in the Software without restriction, including without limitation the rights
// * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// * copies of the Software, and to permit persons to whom the Software is
// * furnished to do so, subject to the following conditions:
// *
// * The above copyright notice and this permission notice shall be included in all
// * copies or substantial portions of the Software.
// *
// * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// * SOFTWARE.
// */
//
//
//#include "autoraygui/gui_tree.hpp"
//#include "autoraygui/control.hpp"
//#include "autoraygui/enums.hpp"
//
//
//#include <raylib.h>
//
//#ifndef RAYGUI_IMPLEMENTATION
//#define RAYGUI_IMPLEMENTATION
//#endif
//#include <raygui.h>
//
//#include <iostream>
//
//
//int main() {
//    char format_string_buffer[48];
//
//    AutoRayGui::GuiTree<128u, AutoRayGui::INDEXING_MODE_SAFE> gui;
//    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
//    InitWindow(800, 600, "Raylib + RayGUI Example");
//
//    gui.BeginRoot()
//        ->SetChildAlignment({AutoRayGui::CHILD_ALIGNMENT_CENTER, AutoRayGui::CHILD_ALIGNMENT_CENTER})
//        ->BeginVBox()
//            ->SetPaddingAll(8.0f)
//            ->SetDrawFunc(AutoRayGui::build_raylib_draw_rectangle(&DrawRectangleLines, RAYWHITE));
//            const int vbox_children0 = 8;
//            int labels0[vbox_children0] = {};
//            int buttons0[vbox_children0] = {};
//            for (int i = 0; i < vbox_children0; ++i) {
//                gui.BeginHBox()
//                    ->SetPaddingAll(4.0f)
//                    ->SetDrawFunc(AutoRayGui::build_raylib_draw_rectangle(&DrawRectangle, SKYBLUE))
//                    ->BeginChild({120.0f, 16.0f}, AutoRayGui::build_ray_gui(labels0[i], &GuiLabel, "This is Label"))
//                    ->End()
//                    ->BeginChild({120.0f, 16.0f}, AutoRayGui::build_ray_gui(buttons0[i], &GuiButton, "This is a button"))
//                    ->End()
//                ->End();
//            }
//        gui.End()
//    ->EndRoot();
//
//
//    while (!WindowShouldClose()) // Main loop
//    {
//        gui.UpdateLayout();
//
//        BeginDrawing();
//        ClearBackground(BLACK);
//        gui.Draw();
//        EndDrawing();
//    }
//
//    CloseWindow();
//
//    return 0;
//}