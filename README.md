# Hiking Simulator - Walking Simulator Engine

A functional C++ game engine and walking simulator built with SDL2 and OpenGL.

## Project Structure

- **engine/** - Core engine code
  - **core/** - Logging, timing, file system, configuration
  - **math/** - Mathematics utilities (transforms, vectors)
  - **platform/** - SDL2 window, input, audio
  - **render/** - OpenGL rendering, shaders, cameras
  - **assets/** - Asset management and loading
  - **scene/** - ECS system (entities, components, systems)
  - **physics/** - Basic physics simulation
  - **environment/** - Wind and environmental systems
  - **audio/** - Sound and music systems
  - **ui/** - UI widgets and layouts
  - **tools/** - Debug tools (ImGui layer, debug panels)

- **game/** - Game-specific implementations
  - **components/** - Player controller, camera controller
  - **systems/** - Player system, gameplay system
  - **scene/** - Scene serialization
  - **levels/** - Level definitions
  - **definitions/** - Game data (climates, items)
  - **assets/** - Game assets (textures, models)

## Building

### Requirements
- C++17 compiler
- SDL2
- OpenGL 3.3+
- GLEW
- GLM

### CMake Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running

```bash
./HikingSimulator
```

## Controls

- **W/A/S/D** - Move forward/left/backward/right
- **Mouse (Right Click + Drag)** - Look around
- **ESC** - Exit application

## Features

### Engine
- Component-based Entity System
- Basic physics (gravity, velocity)
- Shader system with GLSL support
- Mesh and texture management
- Scene management
- Input handling (keyboard, mouse)
- Logging system
- Configuration management

### Game
- Walking simulator gameplay
- Environmental systems (wind, sun)
- Camera controller with mouse look
- Player movement system
- Scene serialization framework

## Architecture

The engine uses:
- **Singleton Pattern** - Logger, Timer, Input, Renderer
- **Component-Based ECS** - Entities with attachable components
- **System-Based Updates** - Systems process entity data
- **Asset Management** - Centralized loading and caching

## Extensibility

To add new features:
1. Create new component classes inheriting from `Component`
2. Create new system classes inheriting from `System`
3. Add entities and components in your game code
4. Systems automatically update compatible entities each frame

## Future Enhancements

- Terrain generation and rendering
- Advanced lighting (normal maps, PBR)
- Particle effects
- Audio playback
- Save/load system
- Level editor
