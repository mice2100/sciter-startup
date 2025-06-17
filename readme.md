# sciter-startup

This is a template project to create a sciter.js (www.sciter.com) backend application.

The sciter official samples are provided in Visual Studio project on Windows and Xcode project on macOS.
This template provides a lightweight solution using VSCODE + CMAKE + CLANG for cross-platform development.

## Features
1. Cross-platform support:
   - Windows: Compatible with MINGW/CLANG(msys2) and Visual Studio (2017+)
   - macOS: Universal binary (arm64 + x86_64) support
2. Modern CMake configuration with clean project structure
3. Auto-increment build number in version string (x.x.x.xxx)
4. Proper application bundle/executable configuration:
   - Windows: Icon and version info in executable
   - macOS: Proper bundle structure with Frameworks
5. Development features:
   - Debug mode control for sciter
   - Console window toggle
   - Local mode for development
   - JS debugging support
6. Dependencies:
   - spdlog for logging
   - fmt for string formatting
   - Sciter.js SDK integration
7. Build automation:
   - Python scripts for resource packaging and building
   - Automated version management
   - Resource file generation
8. Dependency Management:
   - Uses vcpkg for C++ dependency management
   - Easy integration of third-party libraries
   - Cross-platform dependency resolution

## Project Structure
```
.
├── src/                    # Source files
│   ├── CMakeLists.txt     # Main build configuration
│   ├── AppMain.cpp        # Application entry point
│   └── res/               # Resources (icons, plists)
├── ui/                     # UI files (HTML/CSS/JS)
├── bin/                    # Build output directory
├── packres.py             # Resource packaging script
└── bd.py                  # Build automation script
```

## Prerequisites
1. Install vcpkg:
   ```bash
   # Windows
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install

   # macOS
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.sh
   ./vcpkg integrate install
   ```

2. The project uses `vcpkg.json` for dependency management. Required packages will be automatically installed during the build process. The current dependencies are:
   - spdlog: For logging
   - fmt: For string formatting

## How to Use
1. Clone the repository:
   ```bash
   git clone https://github.com/mice2100/sciter-startup.git myproject
   ```

2. Package resources:
   ```bash
   python3 packres.py
   ```
   This will generate `resources.cpp` from your UI files.

3. Build the project:
   ```bash
   python3 bd.py [options]
   ```
   
   Build script options:
   - `-os`: Target platform (default: "osx" on macOS, "x32" on Windows)
     - `x32`: 32-bit Windows
     - `x64`: 64-bit Windows
     - `osx`: macOS
   - `-build`: Build type (default: "Release")
     - `Debug`: Debug build
     - `Release`: Release build
   - `-step`: Build step (default: "all")
     - `cmake`: Only run CMake configuration
     - `build`: Only build the project
     - `all`: Run both CMake and build

   Examples:
   ```bash
   # Build for macOS (default)
   python3 bd.py

   # Build 64-bit Windows release
   python3 bd.py -os x64 -build Release

   # Only run CMake configuration for debug build
   python3 bd.py -build Debug -step cmake
   ```

   The script will:
   1. Automatically increment the build number in `src/BN.txt`
   2. Create a build directory named `build-{os}-{build}`
   3. Run CMake configuration if needed
   4. Build and install the project

4. Add your source files to `src/`
5. Add your UI files to `ui/`

## Configuration Options
- `LOCALMODE`: Enable local development mode
- `DEBUG`: Enable debug build
- `CONSOLE`: Show console window
- `ARCH`: Target architecture (default: x32)

## Dependencies
- Sciter.js SDK (5.0.3.11+)
- CMake 3.20.0+
- C++17 compatible compiler
- Python 3.x (for build scripts)
- vcpkg (for dependency management)
  - spdlog
  - fmt

## Notes
- This template is extracted from a production project
- For issues or improvements, please submit a PR or open an issue
- Make sure to run `packres.py` before building if you've modified UI files
- Ensure vcpkg is properly installed and integrated with your system

## Changelog
### V2.1.0
- Updated to Sciter.js SDK 5.0.3.11(this is the latest version of 5.0 which includes universal binary)
- Added universal binary support for macOS
- Improved CMake configuration
- Modernized build system
- Added proper bundle structure for macOS
- Added Python build automation scripts
- Integrated vcpkg for dependency management

### V2.0 (Feb 6, 2023)
- Refactored folder structure
- Added Python scripts for vendor management and resource packaging
- Switched to vcpkg and clang for faster compilation
- Implemented full path loading for main.htm

