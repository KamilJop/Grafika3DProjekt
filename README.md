# 3D Horror Escape Room

[![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)]()
[![Graphics](https://img.shields.io/badge/graphics-OpenGL%203.3-red.svg)]()
[![License](https://img.shields.io/badge/license-MIT-green.svg)]()

**3D Horror Escape Room** is a first-person puzzle and exploration game built on a custom 3D rendering engine.
The player navigates a dark environment, solves puzzles, and attempts to escape using a flashlight as their main tool.
The project focuses on atmospheric rendering, advanced lighting, and interactive gameplay systems.

Author: **Kamil Jop**

---

## Important Notice

This project is currently a **Work in Progress**.
While assets (models, textures, shaders) are included in the repository, external libraries
(**GLFW**, **GLAD**, **GLM**, **Assimp**, **FreeType**, **Dear ImGui**) must be installed and linked manually
(CMake or Visual Studio project configuration).

---

## Table of Contents

- [Features](#features)
- [Controls](#controls)
- [Setup](#setup)
- [License](#license)

---

# Features

## Core Engine & Rendering
- **Blinn–Phong lighting model** supporting:
  - Directional lights
  - Point lights
  - Spotlights (Flashlight)
- **Shadow Mapping**:
  - Orthographic shadow maps for directional lights
  - Omni-directional cube-map shadows using geometry shaders
- **Surface Detail**:
  - Normal Mapping
  - Steep Parallax Mapping for depth simulation
- **Post-Processing**:
  - Object outlining using stencil buffer operations
- **Skybox** using cubemap rendering

## Gameplay & Mechanics
- **Escape Room logic**:
  - Interactive objects (doors, chests) with animated state interpolation
  - Raycasting-based interaction detection
- **Flashlight system**:
  - Dynamic spotlight with “sway” movement simulation
- **Physics**:
  - FPS controller (gravity, jump, crouch)
  - Real-time AABB collision detection

## System & UI
- **Configuration Manager**:
  - Loads resolution, volume, and color settings from `config.txt`
- **UI System**:
  - Dear ImGui for runtime settings, debug options, and pause menu
- **Resource Handling**:
  - Model loading via Assimp for multiple 3D formats

---

# Controls

| Key              | Action                   |
|------------------|--------------------------|
| W, A, S, D       | Movement                 |
| SPACE            | Jump                     |
| Left SHIFT       | Crouch                   |
| F                | Toggle Flashlight        |
| E                | Interact (Door/Chest)    |
| ESC              | Pause Menu               |

---

# Setup

## 1. Dependencies

Ensure the following libraries are installed and linked in your IDE/CMake:

- GLFW
- GLAD
- GLM
- Assimp
- FreeType
- Dear ImGui

## 2. Project Structure

Ensure your working directory contains these folders:

- `Shaders/` - Shader source files (.vert, .frag, .geom)
- `Models/` - 3D models loaded via Assimp (.obj, .fbx, etc.)
- `Textures/` - Diffuse, normal, and height maps


---

# License

This project is released under the **MIT License**.
