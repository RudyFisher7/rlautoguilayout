# Raylib Auto GUI Layout Library

A C99 library designed to be used with Raylib and Raygui for creating immediate mode responsive GUI.

## Notice
This project is early in development and may be missing vital functionality. Please feel free to contribute a PR or report bugs with MRPs attached.
Also, if you find something with this library that makes it in any way janky to work with, please submit your feedback!

## Credits
A big thanks and credit extended to Nic Barker, creator of Clay.h and his wonderful educational video on the topic. His library also supports Raylib and is highly recommended. (You will likely find no need for this library at all if you check out his, honestly.) Go use it! It is also likely that Clay computes layout faster than this library (TODO:: micro-benchmarks).

## Goals
So why create this library if Clay already exists? First and foremost, because Nic's video inspired me to try making one. Second, this library is designed with slightly different goals:
1. It uses the same controls that Raylib and Raygui use while keeping their native styling by default as well as keeping these controls exposed via function calls.
2. It allows the developer to define their own draw functions for the controls. In addition, within these functions, custom logic can be used to adjust the look and behavior.
3. (This is a minor contrast.) This library avoids using designated initializers so that legacy C++ can be used if desired (though you'll likely not need to).

## Usage
Using CMake, add this to your project with add_subdirectory(rlautolayout) or similar.
See CMakePresets.json and the example/ folder for more information.