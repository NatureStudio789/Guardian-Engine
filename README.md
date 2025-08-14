# Guardian Engine

![Guardian Engine Logo](Guardian%20Engine/Resources/Icon/Guardian%20Engine.png)  


## Engine Introduction
Guardian Engine is a modern, Windows-focused game engine built on DirectX 12, designed to provide a flexible foundation for game development. It integrates core systems for rendering, gameplay logic, audio processing, scene management and editor, with a modular architecture that simplifies extension and customization.

Key technical foundations include:
- DirectX 12 for high-performance graphics rendering, supporting modern features like descriptor heaps and command lists.
- Entity-Component System (ECS) for flexible gameplay object management.
- Render graph system (`GRenderGraph`) for configurable rendering pipelines (e.g., lighting passes, wireframe rendering).
- Assimp integration for model loading (supports formats like FBX).
- XAudio2 for audio processing and playback.
- Cross-module communication via singleton patterns (e.g., `GEngine::Instance`, `GApplication::Instance`) and registry systems (e.g., `GSceneRegistry`, `GGraphicsContextRegistry`).

LeadMaker : NullEntityPtr* (Kevin)
Member : wzy2011


## Get Started

### Prerequisites
- **Operating System**: Windows 10/11 (64-bit)
- **Development Tools**: Visual Studio 2019 or later
- **Dependencies**:
  - DirectX 12 SDK
  - Assimp (Open Asset Import Library)
  - XAudio2 SDK
  - DirectXTex (for texture processing)
  - ImGui (for editor UI)


### Setup & Configuration
1. **Clone the Repository**  
   ```bash
   git clone <repository-url>
   cd GuardianEngine
   ```

2. **Open the Project**  
   Run the bat file:
   ```bash
   Generate Project.bat
   ```
   And input your IDE name, like vs2022 to build the project files.

   Or directly launch the solution file for Visual Studio 2022:  
   ```bash
   GuardianEngine.sln
   ```

3. **Configure Dependencies**  
   Ensure all required libraries (DirectX 12, Assimp, etc.) are linked correctly in the project settings. The engine uses preprocessor definitions (`GE_PLATFORM_WINDOWS`, `GE_BUILD_DLL`) to handle platform-specific builds.

4. **Build & Run**  
   - Set the build target to `x64` (32-bit is not supported).  
   - Build the solution (Ctrl+Shift+B).  
   - Run the generated executable. The default setup loads a sample scene.


## Engine Architecture
Guardian Engine follows a modular design, with key components organized into interconnected systems:

### 1. Core Engine (`GEngine`)
- Serves as the central coordinator, managing initialization, execution, and shutdown via `InitializeEngine()`, `LaunchEngine()`, and `ReleaseEngine()`.
- Integrates with `GProgram` (application-specific logic) and `GModuleSystem` (manages module lifecycles).


### 2. Rendering System
- **GRenderEngine**: Manages graphics context initialization, rendering updates, and resource release. Creates and registers the main `GGraphicsContext` (DirectX 12 device, swap chain, command queues).
- **GGraphicsContext**: Handles low-level DirectX 12 operations (descriptor heaps, command lists, framebuffers) and rendering synchronization via fences.
- **Render Graphs**: `GRenderGraph` and `GSceneRenderGraph` define rendering pipelines with configurable passes, enabling flexible scene rendering.
- **Model Rendering**: `GModel` loads meshes/textures via Assimp, with `GMesh` and `GMaterial` handling geometry and shading data.


### 3. Gameplay System
- **GGameplayEngine**: Manages active scenes (`GScene`), entities (`GEntity`), and gameplay updates.
- **Scene Management**: `GScene` and `GSceneRegistry` manage entity hierarchies, with support for parent-child relationships.
- **Components**: Entities extend functionality via components like `GTransformComponent` (position/rotation/scale) and `GModelComponent` (model attachment).


### 4. Audio System (`GAudioEngine`)
- Initializes and manages `GAudioContext` (via XAudio2) for audio playback.
- Supports audio sources (`GSourceVoice`) with buffer management for continuous playback.


### 5. Utility & Infrastructure
- **Core Utilities**: `GCore.h` defines platform-specific macros, type aliases, and dependencies (e.g., DirectXMath, Windows.h).
- **Error Handling**: Uses custom exceptions and message boxes for runtime error reporting.


## Future Plans
1. **Resource Management System**  
   Implement a centralized system to handle loading, caching, and unloading of assets (models, textures, audio files) to optimize memory usage and reduce redundant I/O operations.

2. **Ray Tracing Support**  
   Extend the rendering pipeline with DirectX Raytracing (DXR) to enable realistic lighting, reflections, and shadows, enhancing visual fidelity for modern games.

3. **Scripting System**  
   Integrate a scripting language (e.g., Lua) to allow designers and developers to prototype gameplay logic without recompiling the engine, improving iteration speed and flexibility.


## License
MIT


## Acknowledgements
- DirectX 12 SDK and Documentation
- Assimp (Open Asset Import Library)
- XAudio2 for audio processing
- ImGui for editor rendering
- DirectXTex for texture handling