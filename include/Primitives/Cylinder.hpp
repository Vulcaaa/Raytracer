/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.hpp
*/

/// @file Cylinder.hpp
/// @brief Cylinder class definition

#pragma once

#include <optional>

#include "APrimitive.hpp"
#include "Point3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a Cylinder in 3D space
    class Cylinder : public APrimitive {
        public:
        Cylinder() = default;

        /// @brief Center of the cylinder's base
        Point3D _base;
        /// @brief Orientation of the cylinder
        Vector3D _axis;
        /// @brief Radius of the cylinder
        double _radius;
        /// @brief height The height of the cylinder
        std::optional<double> _height;

        /// @brief Determines if a ray intersects the cylinder and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the cylinder
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the cylinder
        /// @return True if the ray intersects the cylinder, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the cylinder with settings from a configuration file
        /// @param setting The configuration settings for the cylinder
        void init(const libconfig::Setting& setting) override;
    };
}  // namespace RayTracer
