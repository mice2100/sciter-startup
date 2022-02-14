# sciter-startup

This is a template project to create a sciter.js (www.sciter.com) backend application.

The sciter official samples are provided in VisualStudio project on windows and CODE project on macOS.
Becaused of the heavy size of VStudio and Code, my routine workspace are based on VSCODE+CMAKE+command line compiler.
If you happen like the lightweigth solution like me, this is what you need.

If you prefer VS or Code, this can still helps you to maintain the project unified, becase anyway, you can 
use cmake to generate the projects easily.

Several features are included in this template:

1. Compatiable with MINGW(msys2) and Visual Studio (2017 command line tools tested) on Windows platform
2. Use brew + command line tools on macOS
3. Auto increate build number in the version string (3.1.2.xxx)
4. Included icons for both windows and macOS
5. Use devmode to control sciter debug mode & show/hide console window
6. Use TMPDLLLIST to specify dlls to be installed. (bin/x64 on windows, bundle folder on macOS)

Current it's using vcpkg as 3rd party librarie provider, modify .vscode\cmake-kits.json to use msys2 or brew.

- I simplified this from my original project so, if there's anything wrong, please PR or leave me a message.

