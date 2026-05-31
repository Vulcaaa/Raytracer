# Raytracer

A CPU raytracer written in **C++23**, built around an object-oriented, plugin-friendly architecture. Scenes are described in plain configuration files and rendered to PPM images, with multithreaded rendering via OpenMP.

> Epitech 2nd-year OOP project — a team project (see [Credits](#credits)).

## Features

- **OOP architecture** — primitives, lights and materials behind clean interfaces, instantiated through factories
- **Primitives** — spheres, planes and more (`source/Primitives`)
- **Lights & materials** — point lights, ambient lighting, Lambertian materials
- **Scene description** — human-readable `.cfg` files parsed with `libconfig++` (no recompilation to change a scene)
- **Multithreading** — parallel rendering with OpenMP when available
- **Sample scenes** — demos, fractals and primitive showcases in `scenes/`

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

See `scenes/` for ready-to-use scene files and `scenes/Plugins_Docu.md` for the scene/plugin format.

## Credits

Group project by:

- Nielsen Combe-Bracciale
- Mathys Aberkane
- Julie
- and contributors *sefkan*, *Yumi-alt*

Originally developed in a private team repository; this is a public snapshot for portfolio purposes.
