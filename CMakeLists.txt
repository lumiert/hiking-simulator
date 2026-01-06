cmake_minimum_required(VERSION 3.10)
project(HikingSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})

# Engine sources
set(ENGINE_SOURCES
    engine/core/Logger.cpp
    engine/core/Timer.cpp
    engine/core/FileSystem.cpp
    engine/core/Config.cpp
    engine/math/Transform.cpp
    engine/debug/DebugDraw.cpp
    engine/platform/SDLWindow.cpp
    engine/platform/SDLInput.cpp
    engine/platform/Time.cpp
    engine/environment/Wind.cpp
    engine/environment/EnvironmentSystem.cpp
    engine/render/OpenGLContext.cpp
    engine/render/Shader.cpp
    engine/render/Mesh.cpp
    engine/render/Camera.cpp
    engine/render/Texture.cpp
    engine/render/Renderer.cpp
    engine/render/materials/Material.cpp
    engine/render/materials/MaterialInstance.cpp
    engine/assets/ModelLoader.cpp
    engine/assets/TextureLoader.cpp
    engine/assets/AssetManager.cpp
    engine/scene/Entity.cpp
    engine/scene/World.cpp
    engine/physics/RigidBody.cpp
    engine/physics/PhysicsWorld.cpp
    engine/audio/Sound.cpp
    engine/audio/Music.cpp
    engine/audio/AudioSystem.cpp
    engine/ui/Widget.cpp
    engine/ui/Layout.cpp
    engine/ui/UIContext.cpp
    engine/tools/ImGuiLayer.cpp
    engine/tools/DebugPanels.cpp
)

# Game sources
set(GAME_SOURCES
    game/components/PlayerController.cpp
    game/components/CameraController.cpp
    game/systems/PlayerSystem.cpp
    game/systems/GameplaySystem.cpp
    game/scene/SceneSerializer.cpp
)

# Create executable
add_executable(HikingSimulator 
    main.cpp 
    ${ENGINE_SOURCES} 
    ${GAME_SOURCES}
)

# Link libraries
target_link_libraries(HikingSimulator 
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glm::glm
)

# Platform-specific settings
if(MSVC)
    target_compile_options(HikingSimulator PRIVATE /W4)
else()
    target_compile_options(HikingSimulator PRIVATE -Wall -Wextra -Wpedantic)
endif()
