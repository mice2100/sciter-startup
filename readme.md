# sciter-startup

This is a template project to create a sciter.js (www.sciter.com) backend application.

The sciter official samples are provided in VisualStudio project on windows and CODE project on MACOS.
If you prefer VSCODE+CMAKE+command line compiler solution like me, this is what you need. - I simplified this from my original project, wish this helps.

Several features are included in this template:

1. Compatiable with MINGW(msys2) and Visual Studio (2012 command line tools tested) on Windows platform
2. Use brew + command line tools on MACOS
3. Auto increate build number in the version string (3.1.2.xxx)
4. Included icons for both windows and macos

Current it's using vcpkg as 3rd party librarie provider, modify .vscode\cmake-its.json to use msys2 or brew.
