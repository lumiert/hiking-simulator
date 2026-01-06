# Hiking Simulator - Build & Run Guide

## 🎯 Quick Start

```powershell
# Build everything (DLLs + executables)
python build.py

# Run the main application
.\hiking.exe

# Or run the SDL2-only test (no OpenGL required)
.\sdl_test.exe

# Or run the Python demo
python demo.py
```

## 📦 What Was Fixed

The issue **"glew32.dll não foi encontrado"** (glew32.dll not found) has been solved by:

1. **Python Build System** - `build.py` automates compilation
2. **DLL Deployment** - Automatically copies SDL2.dll and glew32.dll to project root
3. **Dependency Verification** - Checks all libraries before compiling
4. **Clean Build** - Optional `--clean` and `--rebuild` flags

## 🚀 Available Executables

| Program | Purpose | Requirements |
|---------|---------|--------------|
| `hiking.exe` | Main application with OpenGL graphics | SDL2, GLEW, OpenGL 3.3 |
| `test.exe` | Test version with console output | SDL2, GLEW, OpenGL 3.3 |
| `sdl_test.exe` | Simple SDL2-only test (no graphics API) | SDL2 only |
| `demo.py` | Python simulation demo | Python 3.x |

## 🔨 Build Commands

```bash
# Build all targets
python build.py

# Clean build artifacts
python build.py --clean

# Clean and rebuild
python build.py --rebuild

# Check dependencies only
python build.py --check

# Verbose compilation (shows g++ commands)
python build.py --verbose
```

## 📁 Project Structure

```
D:\Apps\Hiking\
├── build.py                    # Python build system
├── build.bat                   # Legacy batch script
├── demo.py                     # Python demo
├── main_graphics.cpp           # Main app (SDL2 + OpenGL)
├── main_test.cpp               # Test app (SDL2 + OpenGL)
├── main_sdl_test.cpp           # SDL2-only test
├── hiking.exe                  # Compiled main app
├── test.exe                    # Compiled test app
├── sdl_test.exe                # Compiled SDL2 test
├── SDL2.dll                    # Runtime library (auto-deployed)
├── glew32.dll                  # Runtime library (auto-deployed)
├── engine/                     # Core engine code
│   ├── core/
│   ├── platform/
│   ├── render/
│   ├── physics/
│   └── ...
├── game/                       # Game-specific code
└── dependencies/               # External libraries
    ├── SDL2-2.28.5/
    ├── glew-2.2.0/
    └── glm/
```

## 🔧 Technical Details

- **Compiler**: WinLibs MinGW g++ 15.2.0 (x86_64-w64-mingw32)
- **C++ Standard**: C++17 with -O2 optimization
- **Build System**: Python 3.x (platform-independent)
- **Graphics**: OpenGL 3.3 core profile
- **Windowing**: SDL2 2.28.5
- **Extensions**: GLEW 2.2.0
- **Math**: GLM (header-only)

## ✅ Compilation Status

All three executables compile successfully:
- hiking.exe (2.59 MB)
- test.exe (2.59 MB)  
- sdl_test.exe (2.59 MB)

DLLs are automatically deployed:
- SDL2.dll (2.38 MB)
- glew32.dll (0.44 MB)

## 🎮 Features

- ✓ SDL2 window creation (1280x720 or 800x600)
- ✓ OpenGL 3.3 context management
- ✓ GLEW extension loading
- ✓ Event handling (keyboard, mouse, quit)
- ✓ Real-time rendering loop (60 FPS)
- ✓ Camera system with GLM matrices
- ✓ Physics simulation framework
- ✓ Logging and debugging

## 🐛 Troubleshooting

### Error: "glew32.dll not found"
- Run `python build.py` to ensure DLLs are deployed
- Check that SDL2.dll and glew32.dll exist in the same folder as .exe

### Error: "OpenGL context creation failed"
- Some systems may not support OpenGL 3.3
- Use `sdl_test.exe` which doesn't require OpenGL
- Update GPU drivers

### Compilation errors
- Ensure dependencies are in `dependencies/` folder
- Run `python build.py --check` to verify all libraries
- Run `python build.py --verbose` to see detailed g++ commands

## 📚 Documentation

See [BUILD_SUCCESS.md](BUILD_SUCCESS.md) for detailed build information.

## 🚀 Next Steps

1. **Graphics Rendering** - Implement shader system and mesh rendering
2. **Scene Management** - Build entity-component-system
3. **Physics** - Integrate rigid body simulation
4. **Terrain** - Generate procedural mountain landscape
5. **Audio** - Add music and sound effects

---

**Status**: ✅ Ready for Development  
**Last Updated**: 2026-01-06  
**Python Builder**: v1.0
