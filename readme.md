# sciter-startup

This is a template project to create a sciter.js (www.sciter.com) backend application.

The sciter official samples are provided in Visual Studio project on windows and xcode project on macOS.
Becaused of the heavy size of VStudio and xcode, my routine workspace are based on VSCODE+CMAKE+command line compiler.
If you happen prefer lightweigth solution like me, this might help.

If you prefer VS or Code, this can still helps you to maintain the project unified, becase anyway, you can 
use cmake to generate the projects easily.

## Several features are included in this template:
1. Compatiable with MINGW(msys2) and Visual Studio (2017 command line tools tested) on Windows platform. Use brew + command line tools on macOS
2. Auto increate build number in the version string (3.1.2.xxx)
3. Icon and version info in application/bundle
4. Use devmode to control sciter debug mode & show/hide console window
5. Use TMPDLLLIST to specify dlls to be installed. (bin/x64 on windows, bundle folder on macOS)

## How to use it
    1. git clone https://github.com/mice2100/sciter-startup.git myproject
    2.  Add source files into src\
        Add third-party libs into third-party\dist

## Current it's using vcpkg as 3rd party librarie provider, modify .vscode\cmake-kits.json to the correct
## path on your computer, or specify msys2 or brew.

- This is extracted from my original project so, if there's anything wrong, please PR or leave me a message.

V2.0
Feb 6, 2023
1. efactor folder structure
2. Use python to recreate vendor folder(mkvendor.py), package resource(packres.py) and build
3. Use vcpkg and clang to compile(much faster than mingw), you can switch to mingw in bd.py
