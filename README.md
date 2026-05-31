# Raytracer

A CPU raytracer written in **C++23**, built around an object-oriented, plugin-based architecture. Scenes are described in plain configuration files and rendered to PPM images, with multithreaded rendering via OpenMP.

## Features

- **OOP architecture** — primitives, lights and materials behind clean interfaces (`IPrimitive`, `ILight`, `IMaterial`), instantiated through factories
- **Plugin system** — components are loaded dynamically as shared libraries (`DLLoader` / `PluginFactory`), so the renderer can be extended without recompiling the core (see `scenes/Plugins_Docu.md`)
- **Primitives** — spheres, planes, boxes, cones, cylinders, tori, triangles, and **`.obj` mesh loading**
- **3D fractals** — Mandelbulb, Menger sponge, Sierpinski, Möbius strip, tangle cube
- **Materials** — Lambertian, Phong, reflective, refractive, and textures
- **Lights** — point, directional and ambient lighting
- **Scene description** — human-readable `.cfg` files parsed with `libconfig++`; change a scene without recompiling
- **Multithreading** — parallel rendering with OpenMP when available
- **Sample scenes** — point-light, primitive and fractal demos in `scenes/`

## Build

Requires CMake, a C++23 compiler, and `libconfig++` (fetched automatically if missing). OpenMP is used when available.

```bash
cmake -B build
cmake --build build
```

## Usage

```bash
./raytracer <scene.cfg>
```

The render is written to `output.ppm`. Example:

```bash
./raytracer scenes/scene_pointlight.cfg
```

A scene file describes the renderer settings, camera, materials, lights and primitives. See `scenes/` for ready-to-use examples and `scenes/Plugins_Docu.md` for the scene/plugin format.

## Credits

Team project realized as part of the Epitech curriculum — **OOP** module.

- Nielsen Combe-Bracciale
- Mathys Aberkane
- Sefkan Aydogdu
- Julie Vincent
