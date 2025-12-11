# 3D Horror Escape Room

[![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)]()
[![Graphics](https://img.shields.io/badge/graphics-OpenGL%203.3-red.svg)]()
[![License](https://img.shields.io/badge/license-MIT-green.svg)]()

**3D Horror Escape Room** is a first-person puzzle and exploration game built on a custom 3D rendering engine.
The player navigates a dark environment, solves environmental puzzles, and manages an inventory system to escape.
The project focuses on atmospheric rendering, advanced lighting, 3D spatial audio, and interactive gameplay systems.

Author: **Kamil Jop**

---

## Important Notice

This project is currently a **Work in Progress**.
While assets (models, textures, shaders, audio) are included in the repository, external libraries
(**GLFW**, **GLAD**, **GLM**, **Assimp**, **FreeType**, **Dear ImGui**, **SoLoud**) must be installed and linked manually
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
- **Blinn–Phong Lighting Model**:
  - Directional lights (Sun/Moon)
  - Point lights (Lamps, Candles)
  - Dynamic Spotlights (Flashlight)
- **Advanced Shadows**:
  - Orthographic shadow maps for directional lights
  - **Omni-directional cube-map shadows** for point lights (Geometry Shaders)
- **Surface Detail**:
  - Normal Mapping for realistic texture details
  - Steep Parallax Mapping for depth simulation
- **Post-Processing**:
  - Object outlining using Stencil Buffer operations
- **Skybox** using cubemap rendering

## Gameplay & Mechanics
- **Interaction System**:
  - Raycasting-based detection for items and interactive objects
  - Physics-based pick-up system involving "Held Entity" logic
- **Inventory System**:
  - Visual HUD with item sprites and active slot selection
  - **3D Held Items**: Items selected in the inventory are rendered in the player's hand in 3D space
  - Dynamic notification system for gameplay events
- **Puzzles**:
  - Logic-based obstacles (e.g., Door & Key mechanics)
  - **Bookshelf Puzzle**: Interactive sequencing puzzle requiring specific object manipulation
- **Physics**:
  - FPS controller (gravity, jump, crouch)
  - Real-time AABB collision detection

## Audio System
- **3D Spatial Audio** (powered by **SoLoud**):
  - Sounds attenuate based on distance and position (e.g., door creaks, object movements)
- **2D UI Sounds**:
  - Feedback for inventory management and menu interactions

## System & UI
- **Configuration Manager**:
  - Loads resolution, volume (SFX/Music), and graphics settings from `config.txt`
- **UI System**:
  - **Dear ImGui** for pause menu, settings, and debug options
  - **FreeType** for in-game text rendering (crosshair, subtitles, notifications)
- **Resource Handling**:
  - Model loading via Assimp (.obj, .fbx)
  - Texture loading via stb_image

---

# Controls

| Key              | Action                   |
|------------------|--------------------------|
| **W, A, S, D** | Movement                 |
| **Mouse** | Look Around              |
| **SPACE** | Jump                     |
| **Left SHIFT** | Crouch                   |
| **F** | Toggle Flashlight        |
| **E** | Interact / Pick Up Item  |
| **1-9 / Scroll** | Change Active Item       |
| **ESC** | Pause Menu               |

---

# Setup

## 1. Dependencies

Ensure the following libraries are installed and linked in your IDE (e.g., Visual Studio):

- **GLFW** (Windowing and Input)
- **GLAD** (OpenGL Loader)
- **GLM** (Mathematics)
- **Assimp** (Model Loading)
- **FreeType** (Font Rendering)
- **Dear ImGui** (UI Interface)
- **SoLoud** (Audio Engine)

## 2. Project Structure

Ensure your working directory contains these folders next to the executable:

- `Shaders/` - GLSL source files (.vert, .frag, .geom)
- `Models/` - 3D models (.obj, .fbx)
- `Textures/` - Diffuse, normal, height maps, and UI icons
- `Audio/` - Sound effects and music files
- `Fonts/` - .ttf font files

---

# License

This project is released under the **MIT License**.