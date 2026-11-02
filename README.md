# Hot Coffee
Hot Coffee is a work-in-progress, 3D game engine written in C++.

![Logo - WIP](readmeResources/logo-wip.JPG)

Work in Progress Logo

# Editor
Alongside the engine itself, the project includes an Editor that allows users to create and manage scenes, configure object parameters, and preview their work in real time using the engine's rendering capabilities.

![hippo](https://media.giphy.com/media/iG8bsk3ngUFSVLW92l/giphy.gif)

# Lights and Shadows
The engine currently supports three types of lights: Spotlights, Omnidirectional Lights, and Directional Lights. Each light type offers customizable parameters.

Shadow mapping is currently used for shadow rendering. Support for omnidirectional light shadows is still under development.

![hippo](https://media.giphy.com/media/nxCeORCivNL7fEnMO7/giphy.gif)

# PBR
The engine implements Physically Based Rendering (PBR) using the Metallic-Roughness workflow.

![hippo](https://media.giphy.com/media/zZzhbaAO1cZ0fSMR0H/giphy.gif)

# IBL
Image-Based Lighting (IBL) techniques for environment lighting and reflections.

![hippo](https://media.giphy.com/media/3QKYSX3eLnkwzB8LnD/giphy.gif)

# Special Materials
Special Materials intended to recreate specific styles, like Kajiya-Kay Hair Rendering, Toon Shading and more for comming.

![hippo](https://media.giphy.com/media/mtTfoBHupfuf726m3P/giphy.gif)

# Build Requirements

## Linux Prerequisites (Ubuntu/Debian)

Install required build tools and development libraries:

```bash
sudo apt update && sudo apt install -y \
	cmake ninja-build build-essential pkg-config \
	uuid-dev \
	libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
	libgl1-mesa-dev libopengl-dev libudev-dev libglew-dev
```

## Configure and Build (Linux)

This repository provides CMake presets:

- `ninja-debug`
- `ninja-release`

Configure and build:

```bash
cmake --preset ninja-debug
cmake --build --preset ninja-debug
```

For release:

```bash
cmake --preset ninja-release
cmake --build --preset ninja-release
```

# Dependencies
- nlohmann json
- GLEW 2.3.0
- SFML 3.0.2
- Imgui 1.92.5
- ImGuiFileDialog 0.6.8
- nothings stb
- assimp 6.0.4