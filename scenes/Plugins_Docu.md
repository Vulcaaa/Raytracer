# Raytracer Plugins Documentation

This document lists all the available parameters for each plugin (Primitives and Lights) of your Raytracer rendering engine, as well as the expected syntax in a `.cfg` configuration file.

---

## 1. Global Setup (Camera & Renderer)

Before defining primitives and materials, the `.cfg` file must contain mandatory sections for the `camera` and `renderer` settings.

### 1.1. Camera (`camera`)

The camera block defines the viewpoint from which the scene is rendered.

**Parameters:**

- `position` *(block)*: The 3D coordinates of the camera `{x=..., y=..., z=...}`. *(Optional, default: `{x=0, y=0, z=0}`)*
- `rotation` *(block)*: Euler rotation of the camera in degrees `{x=..., y=..., z=...}`. *(Optional, default: `{x=0, y=0, z=0}`)*
- `fieldOfView` *(float)*: The vertical Field of View (FOV) in degrees. *(Optional, default: 90.0)*

**Example:**

```cfg
camera = {
    position = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
    rotation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
    fieldOfView = 70.0; # Optional
};
```

### 1.2. Renderer (`renderer`)

The renderer block sets up the global rendering engine parameters.

**Parameters:**

- `resolution` *(block)*: The dimensions of the output image `{width=..., height=...}`. **(Required)**
- `samples` *(int)*: The number of rays to cast per pixel (anti-aliasing and soft shadows). A higher value reduces noise but increases render time. *(Optional, default: 50)*
- `maxDepth` *(int)*: The maximum number of bounces a ray can make (for reflections/refractions). *(Optional, default: 10)*
- `background` *(string)*: Name of the material to use as the background/skybox when a ray hits nothing. *(Optional, default: none/black)*

**Example:**

```cfg
renderer = {
    resolution = { width = 1920; height = 1080; };
    samples = 100; # Optional
    maxDepth = 15; # Optional
    background = "sky_material"; # Optional (Needs to be defined in materials)
};
```

---

## 2. Common Parameters (Standardized)

To simplify scene creation, **all** primitives now use this unified parameter base:

- `position` *(block)*: Object coordinates `{x=..., y=..., z=...}`. **(Required for Sphere, Plane, Cylinder)**.
- `translation` *(block)*: Global offset of the object `{x=..., y=..., z=...}`. *(Optional, default: `{x=0, y=0, z=0}`)*.
- `rotation` *(block)*: Euler rotation in degrees `{x=..., y=..., z=...}`. *(Optional, default: `{x=0, y=0, z=0}`)*.
- `material` *(string)*: Name of the material to apply to the object. *(Optional, default: BLACK)*.

> **Note:** Materials must be declared in a `materials = { ... };` block at the root of the configuration before being referenced by their name in the `material` parameter of a primitive.

---

## 3. Primitives

Primitives must be declared inside the main `primitives = { ... };` block. Each primitive type is a list bearing the plural name of the plugin (e.g., `spheres`, `planes`).

### 3.1. Sphere (`spheres`)

Represents a 3D sphere.

**Additional parameters:**

- `r` *(float)*: Sphere radius. **(Required)**

**Example:**

```cfg
spheres = (
    {
        position = { x = 0.0; y = 0.0; z = -10.0; };
        r = 2.0;
        translation = { x = 1.0; y = 0.0; z = 0.0; }; # Optional
        rotation = { x = 0.0; y = 45.0; z = 0.0; }; # Optional
        material = "my_red_material"; # Optional
    }
);
```

### 3.2. Plane (`planes`)

Represents an infinite plane. The plane is oriented upwards by default (Y axis: `0, 1, 0`). Use `rotation` to tilt it.

**Example:**

```cfg
planes = (
    {
        position = { x = 0.0; y = -2.0; z = 0.0; };
        rotation = { x = 90.0; y = 0.0; z = 0.0; }; # Optional
        translation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
        material = "floor_material"; # Optional
    }
);
```

### 3.3. Cylinder (`cylinders`)

Represents a cylinder (infinite or limited). By default, the cylinder axis points upwards (Y axis: `0, 1, 0`). Use `rotation` to tilt it.

**Additional parameters:**

- `r` *(float)*: Cylinder radius. Must be positive. **(Required)**
- `h` *(float, optional)*: Cylinder height. If absent, the cylinder will be infinite.

**Example:**

```cfg
cylinders = (
    {
        position = { x = 0.0; y = -2.0; z = -5.0; };
        r = 1.5;
        h = 5.0; # Remove for infinite
        rotation = { x = 0.0; y = 0.0; z = 45.0; }; # Optional
        translation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
        material = "column_material"; # Optional
    }
);
```

### 3.4. Triangle (`triangles`)

Represents a triangle defined by 3 vertices. The `position` and `rotation` apply to the vertices.

**Additional parameters:**

- `v0`, `v1`, `v2` *(blocks)*: The three base vertices of the triangle. Each must contain `{x, y, z}`. **(Required)**

**Example:**

```cfg
triangles = (
    {
        v0 = { x = -1.0; y = -1.0; z = -5.0; };
        v1 = { x =  1.0; y = -1.0; z = -5.0; };
        v2 = { x =  0.0; y =  1.0; z = -5.0; };
        translation = { x = 2.0; y = 0.0; z = 0.0; }; # Optional
        rotation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
        material = "blue_glass"; # Optional
    }
);
```

### 3.5. Obj (`objs`)

Loads a 3D model from an `.obj` file.

**Additional parameters:**

- `file` *(string)*: File path to the `.obj` file. **(Required)**
- `scale` *(float, optional)*: Scale factor to resize the model. Default: 1.0.
- `position` is defined in the `.obj` file.

**Example:**

```cfg
objs = (
    {
        file = "models/cube.obj";
        scale = 2.0;
        translation = { x = 0.0; y = 0.0; z = -5.0; }; # Optional
        rotation = { x = 45.0; y = 45.0; z = 0.0; }; # Optional
        material = "gold_metal"; # Optional
    }
);
```

### 3.6. Box (`boxes`)

Represents a 3D box (rectangular cuboid), defined by a `position`, a `rotation` and it's size along the three axes.

**Additional parameters:**

- `dimension` *(blocks)*: Size of the box along each axis `{x, y, z}`. **(Required)**
  - `x`: width
  - `y`: height
  - `z`: depth
  - All values must be **strictly positive**

**Example:**

```cfg
boxes = (
    {
        position = { x = -1.0; y = 0.0; z = 2.0; };
        dimension = { x =  1.5; y = 1.0; z = 1.0; };
        rotation = { x =  2.0; y =  0.0; z = 0.0; }; # Optional
        translation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
        material = "red"; # Optional
    }
);
```

### 3.7. Cone (`cones`)

Represents a cone (infinite or limited). By default, the cone axis points upwards (Y axis: `0, 1, 0`). Use `rotation` to tilt it.

**Additional parameters:**

- `r` *(float)*: Cone radius. Must be positive. **(Required)**
- `h` *(float, optional)*: Cone height *(for a limited cone)*.
- `d` *(float, optional)*: Distance along the cone axis from the apex, at which the cone reaches the specified radius `r` *(for an infinite cone)*.
- You must call either `h` or `d`. **(Required)**

**Example:**

Infinite:

```cfg
cones = (
    {
        position = { x = 0.0; y = -2.0; z = -5.0; };
        r = 1.0;
        d = 5.0; # Replace by `h` for limited
        rotation = { x = 0.0; y = 0.0; z = 45.0; }; # Optional
        translation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
        material = "column_material"; # Optional
    }
);
```

Limited:

```cfg
torus = (
    {
        position = { x = 0.0; y = -2.0; z = -5.0; };
        r = 1.0;
        h = 3.0; # Replace by `d` for infinite
        rotation = { x = 0.0; y = 0.0; z = 45.0; }; # Optional
        translation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
        material = "column_material"; # Optional
    }
);
```

### 3.8. Torus (`torus`)

Represents a torus in 3D space.

**Additional parameters:**

- `outer_radius` *(float)*: Torus outer radius. Must be positive. **(Required)**
- `inner_radius` *(float)*: Torus inner radius. Must be positive. **(Required)**

**Example:**

Infinite:

```cfg
cones = (
    {
        position = { x = 0.0; y = -2.0; z = -5.0; };
        outer_radius = 1.4;
        inner_radius = 0.8;
        rotation = { x = 0.0; y = 0.0; z = 45.0; }; # Optional
        translation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
        material = "column_material"; # Optional
    }
);
```

### 3.9. Mandelbulb (`mandelbulbs`)

Represents a 3D Mandelbulb fractal, generated via ray marching and a spherical coordinate distance estimator.

**Additional parameters:**

- `scale`: Uniform size of the bulb in world space. (1.0 by default)
- `iterations`: Controls detail level. `10` is standard. Below `6` the surface becomes blurry, above `20` yields diminishing returns. **(Required)**
- `power` *(float)*: Exponent applied during orbit iteration. `8.0` is the classic Mandelbulb. Higher values produce spikier geometry, lower values produce more organic shapes. **(Required)**
- `bailout` *(float)*: Escape radius threshold — iteration stops when the orbit exceeds this value. `2.0` is the mathematically correct value. **(Required)**

**Example:**

```cfg
mandelbulbs = (
    {
        position = { x = 0.0; y = 0.0; z = 0.0 };
        rotation = { x = 90.0; y = 0.0; z = 0.0 }; # Optional
        scale = 1.0; # Optional
        iterations = 20;
        power = 10.0;
        bailout = 2.0;
        material = "mat_0_255_0"; # Optional
    }
);
```

### 3.10. Sierpinski Tetrahedron (`sierpinskis`)

Represents a 3D Sierpinski tetrahedron fractal, generated via ray marching and an IFS (Iterated Function System) distance estimator folding space toward the four vertices of a regular tetrahedron.

**Additional parameters:**

- `scale`: Uniform size of the tetrahedron in world space. (1.0 by default)
- `iterations`: Controls recursion depth and detail level. **(Required)**

**Example:**

```cfg
sierpinskis = (
    {
        position = { x = 0.0; y = 0.0; z = 0.0 };
        rotation = { x = -45.0; y = 0.0; z = -45.0 }; # Optional
        scale = 2.0; # Optional
        iterations = 8;
        material = "mat_255_100_0"; # Optional
    }
);
```

### 3.11. Menger Sponge (`mengersponges`)

Represents a 3D Menger Sponge fractal, generated via ray marching and an IFS (Iterated Function System) distance estimator that recursively removes the center and face-center subcubes at each iteration.

**Additional parameters:**

- `scale`: Uniform size of the sponge in world space. (`1.0` by default)
- `iterations`: Controls recursion depth and detail level. `3` is standard. Above `5` yields diminishing returns and heavier marching cost. **(Required)**

**Example:**

```cfg
mengersponges = (
    {
        position = { x = 0.0; y = 0.0; z = 0.6 };
        rotation = { x = 0.0; y = 0.0; z = 0.0 }; # Optional
        scale = 0.8; # Optional
        iterations = 5;
        material = "mat_255_0_0"; # Optional
    }
);
```

### 3.12. Möbius Strip (`mobiusstrips`)

Represents a Möbius strip surface, generated via ray marching and a local-frame SDF projecting onto the twisted ribbon. The number of half-twists controls the topology of the surface.

**Additional parameters:**

- `scale`: Uniform size of the strip in world space. *(Default: 1.0)*
- `radius` *(float)*: Distance from the world origin to the center line of the ribbon. *(Default: 1.0)*
- `width` *(float)*: Half-width of the ribbon along its local tangent axis. *(Default: 4.0)*
- `thickness` *(float)*: Half-thickness of the ribbon, giving it physical volume for ray marching. *(Default: 0.05)*
- `halfTwist` *(int)*: Number of half-turns applied along the ribbon. `1` produces the classic single-sided Möbius strip. `0` produces a plain ring, even values produce orientable surfaces, odd values produce non-orientable ones. *(Default: 1.0)*

**Example:**

```cfg
mobiusstrips = (
    {
        position = { x = 0.0; y = 0.0; z = 3.0 };
        rotation = { x = 0.0; y = 0.0; z = 0.0 }; # Optional
        scale = 0.8; # Optional
        radius = 2.0; # Optional
        width = 0.4; # Optional
        thickness = 0.07; # Optional
        halfTwist = 1; # Optional
        material = "mat_255_255_255"; # Optional
    }
);
```

### 3.13. Tangle Cube (`tanglecubes`)

Represents a Tangle Cube implicit surface, generated via ray marching. It is defined by the isosurface equation: x⁴ - 5x² + y⁴ - 5y² + z⁴ - 5z² + constant = 0
This produces a smooth, organic shape with four rounded lobes connected by concave bridges, resembling a cube with heavily rounded and pinched faces.

**Additional parameters:**

- `scale`: Uniform size of the surface in world space. *(Default: 1.0)*
- `constant` *(float)*: Controls the shape of the isosurface. `11.8` produces the classic four-lobed Tangle Cube. Lower values tighten the lobes, higher values expand them. Valid range is approximately `[0.0, 15.0]`. **(Required)**

**Example:**

```cfg
tanglecubes = (
    {
        position = { x = 0.0; y = 0.0; z = 5.0 };
        rotation = { x = 0.0; y = 160.0; z = 0.0 }; # Optional
        scale    = 0.8; # Optional
        constant = 11.8;
        material = "mat_255_255_255"; # Optional
    }
);
```
## 3.13. Composite Primitives

The raytracer supports two ways to group primitives into a single transformable unit: **Scene Graph** (inline children) and **Scene Import** (external `.cfg` file). Both produce a `PrimitiveGroup` internally and support `position`, `rotation`, and `scale`.

### 3.13.1. Scene Graph — `children`

Attach child primitives directly to a parent primitive inside the same `.cfg` file. The parent and all its children share the group's transform (position, rotation, scale).

**Parameters:**

- `children` *(block)*: Contains one or more primitive type lists. Accepts any primitive type as key.
> **Note:** Child positions are expressed in the **parent's local space**.

**Example:**

```cfg
primitives:
{
    boxes = (
        {
            position = { x = 0; y = 0; z = 1; };
            dimension = { x = 1; y = 1; z = 1; };
            material = "red";
            children = {
                rotation    = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
                translation = { x = 0.0; y = 0.0;  z = 0.0; }; # Optional

                boxes = (
                    {
                        position  = { x = 0; y = 0; z = 0; }; # local space
                        dimension = { x = 1; y = 1; z = 1; };
                        material  = "white";
                    }
                );
            };
        }
    );
};
``` 

### 3.13.2. Scene Import — `import`

Load an external `.cfg` scene file as a sub-scene and place it in the current scene. All primitives from the imported file are wrapped into a `PrimitiveGroup` and transformed as a unit.

**Parameters:**

- `filepath` *(string)*: Path to the `.cfg` scene file to import, relative to the working directory. **(Required)**
- `position` *(group)*: World-space offset applied to the imported sub-scene. *(Optional)*
- `rotation` *(group)*: Euler angles in degrees applied to the imported sub-scene. *(Optional)*
- `scale` *(float)*: Uniform scale factor applied to the imported sub-scene. *(Default: `1.0`)* *(Optional)*

> **Note:** Multiple scenes can be imported in the same `import` list. Each entry is independent and gets its own transform.

**Example:**

```cfg
# Single import
primitives:
{
    import = (
        {
            filepath = "scenes/primitives/scene_sphere.cfg";
            position = { x = 0; y = 2; z = 2; }; # Optional
            rotation = { x = 0; y = 45; z = 0; }; # Optional
            scale    = 1.5;                        # Optional
        }
    );
};

# Multiple imports
primitives:
{
    import = (
        {
            filepath = "scenes/primitives/scene_sphere.cfg";
            position = { x = 0; y = 2; z = 2; };
        },
        {
            filepath = "scenes/primitives/scene_tangleCube.cfg";
            position = { x = -1; y = 0; z = 0; };
            scale    = 0.5;
        }
    );
};
```


---

## 4. Materials

Materials define the appearance and light reaction of primitives. They must be declared in a `materials = { ... };` block at the root of the configuration file, before the primitives.

Each material must have a unique name (`name = "..."`) which will then be used by primitives via the `material` property.

### 4.1. Lambertian (`lambertian`)

Represents a classic matte (diffuse) material. It scatters light equally in all directions.

**Parameters:**

- `name` *(string)*: The material's unique identifier. **(Required)**
- `color` *(block, optional)*: The object's color `{r, g, b}`. *(Default: white)*
- `shininess` *(double)*: The material's unique identifier. **(Required)**
- `color` *(block, optional)*: The object's color `{r, g, b}`. *(Default: white)*

**Example:**

```cfg
materials = {
    lambertians = (
        { name = "my_red_material"; color = { r = 255.0; g = 0.0; b = 0.0; }; },
        { name = "floor_material";  color = { r = 100.0; g = 100.0; b = 100.0; }; }
    );
};
```

### 4.2. Texture (`texture`)

Represents an image-based material mapped onto a primitive using UV coordinates. Supports **bump mapping** (normal mapping) to simulate surface relief from the texture's luminance variations.

**Parameters:**

- `name` *(string)*: The material's unique identifier. **(Required)**
- `filepath` *(string)*: Path to the texture image file. **(Required)**
- `strength` *(float, optional)*: Intensity of the bump mapping effect. Controls how much the surface normals are perturbed based on the texture's luminance gradient. *(Default: `0.0`)*
  - Recommended range: `0.5` – `2.0` for natural results
  - Values above `3.0` may produce visual artifacts (doubled or blurred appearance)
- `step` *(int, optional)*: Pixel offset used to compute the luminance gradient. A larger value captures low-frequency relief; a smaller value captures fine detail. *(Default: `1`)*
  - Recommended range: `1` – `3`

**Example:**

```cfg
materials = {
    textures = (
        {
            # Texture with default bump mapping
            name = "earth";
            filepath = "textures/earth.jpg";
        },
        {
            # Texture with custom bump intensity
            name = "rough_stone";
            filepath = "textures/stone.jpg";
            strength = 1.8;
            step = 2;
        }
    );
};
```

### 4.3. Refractive (`refractive`)

Represents a refractive material that allows light to pass through, simulating glass or water.

**Parameters:**

- `name` *(string)*: The material's unique identifier. **(Required)**
- `color` *(block, optional)*: The base color of the material `{r, g, b}`. *(Default: white)*
- `transparency` *(float)*: The material's transparency level (0.0 to 1.0). **(Required)**
- `refractiveIndex` *(float)*: The material's refractive index. **(Required)**

**Example:**

```cfg
materials = {
    refractives = (
        {
            name = "MyRefractiveMaterial";
            color = { r = 255.0; g = 255.0; b = 255.0; }; # Optional
            transparency = 0.5;
            refractiveIndex = 1.5;
        }
    );
};
```

### 4.4. Reflective (`reflective`)

Represents a reflective/metallic material that mirrors its surroundings.

**Parameters:**

- `name` *(string)*: The material's unique identifier. **(Required)**
- `color` *(block, optional)*: The material's color tint `{r, g, b}`. *(Default: white)*
- `reflectiveIndex` *(float)*: Controls the surface smoothness/reflectivity. **(Required)**
  - Higher values: Smoother, more mirror-like reflections
  - Lower values: Rougher reflections with more diffusion
  - Can be any positive value (not limited to 0-1 range)

**Example:**

```cfg
materials = {
    reflectives = (
        {
            name = "polished_gold";
            color = { r = 255.0; g = 200.0; b = 0.0; };
            reflectiveIndex = 0.9;
        },
        {
            name = "brushed_steel";
            color = { r = 200.0; g = 200.0; b = 200.0; };
            reflectiveIndex = 0.6;
        }
    );
};
```

### 4.5. Phong (`phongs`)

Represents a material with full Phong illumination: ambient, diffuse, and specular components.

**Parameters:**

- `name` *(string)*: The material's unique identifier. **(Required)**
- `color` *(block)*: The material's diffuse color `{r, g, b}` in 0–255 range. **(Required)**
- `shininess` *(float, optinnal)*: Controls the size of the specular highlight. Higher values produce smaller, sharper highlights (polished); lower values produce softer highlights (matte). Typical range: 8–256. Default: 32.
- `specularStrength` *(float, optionnal)*: Controls the intensity of the specular highlight. Range: 0.0 (none) to 1.0 (full intensity).  Default: 0.8.

**Example:**

```cfg
materials = {
    phongs = (
        {
            name = "shiny_gray";
            color = { r = 100; g = 100; b = 100; };
            shininess = 32;
            specularStrength = 0.8;
        }
    );
};
```

---

## 5. Lights

*Note: Unlike primitives which now use materials, lights continue to use the `color` parameter directly.*

### 5.1. AmbientLight

Global ambient light that uniformly illuminates all objects.

**Parameters:**

- `ambient` *(float, optional)*: Intensity of the ambient light (0.0 to 1.0). Default: 0.
- `color` *(block, optional)*: Tint `{r, g, b}`. Default: white.

**Example:**

```cfg
lights = {
    ambient = 0.2;
    color = { r = 255.0; g = 255.0; b = 255.0; }; # Optional
};
```

### 5.2. DirectionalLight (`directional`)

Directional light.

**Parameters:**

- `intensity` *(float)*: Light intensity. **(Required)**
- `direction` *(block)*: Direction vector `{x, y, z}`. Will be normalized. **(Required)**
- `color` *(block, optional)*: Light color. Default: white.

**Example:**

```cfg
lights = {
    directionals = (
        {
            intensity = 0.8;
            direction = { x = -1.0; y = -1.0; z = -1.0; };
            color = { r = 255.0; g = 255.0; b = 255.0; }; # Optional
        }
    );
};
```

### 5.3. PointLight (`pointlight`)

Point light source that emits light in all directions from a specific position in the scene.

**Parameters (Required):**

- `intensity` *(float)*: Light intensity. **(Required)**
- `position` *(block)*: 3D position `{x, y, z}`. **(Required)**

**Parameters (Optional):**

- `color` *(block)*: Light color `{r, g, b}`. Default: white (255, 255, 255).
- `translation` *(block)*: Global offset applied to position `{x, y, z}`. Default: {0, 0, 0}.
- `rotation` *(block)*: Euler rotation in degrees `{x, y, z}`. Default: {0, 0, 0}.

**Bonus Parameters (Attenuation & Radius):**

- `constant_attenuation` *(float)*: Constant attenuation factor. Default: 1.0.
- `linear_attenuation` *(float)*: Linear attenuation factor. Default: 0.09.
- `quadratic_attenuation` *(float)*: Quadratic attenuation factor. Default: 0.032.
- `radius` *(float)*: Physical size/reach of the light (0.0 = infinite). Default: 0.0.

**Note on Attenuation:**

Light intensity is reduced by distance using the formula:

```python
attenuation = 1.0 / (constant + linear*distance + quadratic*distance²)
```

The `radius` parameter, when set to a value > 0.0, caps the maximum reach of the light for optimization (light has no effect beyond this distance).

**Example (Basic):**

```cfg
lights = {
    pointlight = (
        {
            intensity = 1.0;
            position = { x = 5.0; y = 3.0; z = 5.0; };
            color = { r = 255.0; g = 255.0; b = 255.0; }; # Optional
        }
    );
};
```

**Example (With Attenuation & Radius):**

```cfg
lights = {
    pointlight = (
        {
            intensity = 2.0;
            position = { x = 0.0; y = 5.0; z = 0.0; };
            color = { r = 255.0; g = 200.0; b = 150.0; }; # Warm light
            translation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
            rotation = { x = 0.0; y = 0.0; z = 0.0; }; # Optional
            constant_attenuation = 1.0; # Optional
            linear_attenuation = 0.09; # Optional
            quadratic_attenuation = 0.032; # Optional
            radius = 20.0; # Optional - Light reach limit
        }
    );
};
```
