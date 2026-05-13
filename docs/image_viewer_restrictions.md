# SDD Restrictions: Image Viewer System

## 1. Architectural Restrictions
- **Decoupling:** Image loading and processing logic must be strictly decoupled from rendering and UI. All core image logic must reside in `src/io/` or `src/image/`.
- **Interface-Driven:** Rendering must be performed through an abstraction layer (e.g., `IRenderer`) to allow for headless testing and multiple backends.
- **Minimal Abstraction:** Avoid deep inheritance hierarchies. Prefer composition for image effects and filters.

## 2. Resource Restrictions
- **Memory Management:** Image buffers must be explicitly disposed of or managed via smart pointers. No raw `new`/`delete` for pixel data.
- **Thread Safety:** Concurrent image loading must use a thread pool and avoid blocking the main UI thread.

## 3. Tool Restrictions
- **Allowed Libraries:** `stb_image` (header-only) for decoding, standard C++17 library.
- **Prohibited:** External GUI frameworks (Qt, GTK) for core logic.

## 4. Verification Mapping (Batching)

### Batch A: Image IO and Robustness (ImageIOCard.cpp)
- Verifications: 1-8, 23-27, 30-31, 44-46, 52-55, 71-73, 86-93.
- Focus: Loading, saving, corrupted files, security, cache, plugins.

### Batch B: Viewport and Interaction (ImageViewCard.cpp)
- Verifications: 9-14, 32-43, 47-49, 50-51, 56-61, 78-79, 95-97.
- Focus: Panning, zooming, shortcuts, slideshow, performance, platform-specifics.

### Batch C: Processing and Rendering (ImageProcCard.cpp)
- Verifications: 15-22, 28-29, 68-70, 74-77, 80-85, 94.
- Focus: Rotation, filters, histogram, EXIF, logging, GPU/Software paths, stress tests.

### Batch D: Configuration and Environment (ImageConfigCard.cpp)
- Verifications: 62-67, 98-100.
- Focus: CLI, config files, headless mode, CI/CD, builds.
