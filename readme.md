# sciter-startup

This is a template project to create a sciter.js (www.sciter.com) backend application.

The sciter official samples are provided in VisualStudio project on windows and CODE project on macOS.
Becaused of the heavy size of VStudio and Code, my routine workspace are based on VSCODE+CMAKE+command line compiler.
If you happen like the lightweigth solution like me, this is what you need.

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
