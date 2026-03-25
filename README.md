# hot-coffee
Hot Coffee is a 3D engine

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

# Utilities Dependencies
- nlohmann json

# Graphics Open GL Dependencies
- GLEW 2.3.0

# Window SFML Project Dependencies
- SFML 3.0.2

# Editor Project Dependencies
- Imgui 1.92.5
- ImGuiFileDialog 0.6.8

# Asset Loaders Dependencies
- nothings stb
- assimp 6.0.4