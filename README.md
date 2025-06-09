# Raylib Auto GUI Layout Library

### Discord: https://discord.gg/NMxxQK9kZt
![img.png](img.png)
A C99 library designed to be used with Raylib and Raygui for creating immediate mode responsive GUI.

## Notice
This project is early in development but the layout algorithm should be complete enough to use in a project. Please feel free to contribute a PR or report bugs with MRPs attached.
Also, if you find something with this library that makes it in any way janky to work with, please submit your feedback! Part of the goal of this library is for my own learning, so your outside perspective is greatly appreciated!

## Credits
A big thanks and credit extended to Nic Barker, creator of Clay.h and his wonderful educational video on the topic. His library also supports Raylib and is highly recommended. (You will likely find no need for this library at all if you check out his, honestly.) Go use it! It is also likely that Clay computes layout faster than this library (TODO:: micro-benchmarks).

## Goals
So why create this library if Clay already exists? First and foremost, because Nic's video inspired me to try making one. Second, this library is designed with slightly different goals:
1. It uses the same control functions that Raylib and Raygui use, while keeping them accessible to the developer. In addition, it uses Raylib and Raygui's native styling systems.
2. The developer can define their own draw functions for the controls. This allows custom logic to execute during the draw step as well as custom controls to be made.
3. Though it's a small thing, this library avoids using designated initializers so that legacy C++ can be used if desired.

## Usage
Using CMake, add this to your project with add_subdirectory(rlautolayout) or similar.
See CMakePresets.json and the example/ folder for more information.

## Library Structure
rlauto is made of 2 basic components:
1. A tree structure that stores and computes the layout (tree.h and tree.c). This implements all the layout logic and simply calls draw on each control node (in depth first order). The tree is meant to be mostly self-contained and have no dependencies except for the c std library. Currently some Raylib structs are needed but I plan to allow  building this library without Raylib in future updates.
2. The controls. These are a collection of draw functions that display the GUI and implement any logic needed to execute during the draw step. They are of type void(Layout*, void*). raylib_draw_functions.h and raygui_draw_functions.h implement Raylib-specific draw functions for use with the layout tree.

## TODO::
- Add configuration for building without Raylib and/or Raygui.
- Implement the rest if Raylib and Raygui controls.
- Add other ways for including into a project.
- Support dynamic linking.
- Write benchmarks.
- Clean Arg* structs.
- Add documentation comments.