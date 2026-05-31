/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Renderer
*/

/// @file Renderer.hpp
/// @brief Renderer class for ray tracing rendering

#pragma once

#include <algorithm>
#include <iostream>
#include <libconfig.h++>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "Camera.hpp"
#include "ILight.hpp"
#include "IMaterial.hpp"
#include "IPrimitive.hpp"
#include "Vector3D.hpp"

/// @brief Namespace for the Ray Tracer project
namespace RayTracer {
    /// @brief Renderer class responsible for rendering the scene
    class Renderer {
        public:
        /// @brief Default constructor
        Renderer() = default;

        /// @brief Initializes the renderer with settings from a configuration file
        /// @param setting The configuration settings for the renderer
        void init(const libconfig::Setting& setting);

        /// @brief Renders the scene to an image file
        /// @param camera The camera to use for rendering
        /// @param primitives The list of primitives in the scene
        /// @param lights The list of lights in the scene
        /// @param filename The name of the output image file
        void render(const Camera& camera,
                    const std::vector<std::unique_ptr<IPrimitive>>& primitives,
                    const std::vector<std::unique_ptr<ILight>>& lights,
                    const std::string& filename) const;

        [[nodiscard]] inline const std::string& getBackgroundMaterialName() const
        {
            return _backgroundMaterialName;
        }

        inline void setBackgroundMaterial(const std::shared_ptr<IMaterial>& material)
        {
            _backgroundMaterial = material;
        }

        private:
        /// @brief Width of the output image
        int _width;
        /// @brief Height of the output image
        int _height;
        /// @brief Number of samples per pixel
        int _samples{50};
        /// @brief Maximum ray bounce depth
        int _maxDepth{10};
        /// @brief Name of the background material
        std::string _backgroundMaterialName;
        /// @brief Background material of the scene
        std::shared_ptr<IMaterial> _backgroundMaterial;

        // [[nodiscard]] bool isInShadow(
        //     const HitRecord& hit, const Math::Vector3D<double>& lightDir,
        //     const std::vector<std::unique_ptr<IPrimitive>>& primitives) const;

        /// @brief Computes the color for a given ray based on the scene's primitives
        /// @param ray The ray to trace
        /// @param primitives The list of primitives in the scene
        /// @param lights The list of lights in the scene
        /// @return The computed color for the ray
        [[nodiscard]] Math::Vector3D<double> computeRayColor(
            const Ray& ray, int depth, const std::vector<std::unique_ptr<IPrimitive>>& primitives,
            const std::vector<std::unique_ptr<ILight>>& lights) const;

        void computeRows(const Camera& camera,
                         const std::vector<std::unique_ptr<IPrimitive>>& primitives,
                         const std::vector<std::unique_ptr<ILight>>& lights,
                         std::vector<uint8_t>& fullBuffer) const;

        /// @brief Writes the color of a pixel to the output stream
        /// @param out The output stream to write the color to
        /// @param color The color to write
        inline void writeColor(std::ostream& out, const Math::Vector3D<double>& color) const
        {
            unsigned char r = static_cast<unsigned char>(std::clamp(color._x, 0.0, 255.0));
            unsigned char g = static_cast<unsigned char>(std::clamp(color._y, 0.0, 255.0));
            unsigned char b = static_cast<unsigned char>(std::clamp(color._z, 0.0, 255.0));
            out.write(reinterpret_cast<const char*>(&r), 1);
            out.write(reinterpret_cast<const char*>(&g), 1);
            out.write(reinterpret_cast<const char*>(&b), 1);
        }
    };
}  // namespace RayTracer
