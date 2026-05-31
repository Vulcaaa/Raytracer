/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Box.hpp
*/

/// @file Box.hpp
/// @brief Box class definition

#pragma once

#include <optional>

#include "APrimitive.hpp"
#include "Point3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a Box in 3D space
    class Box : public APrimitive {
        public:
        Box() = default;

        /// @brief Minimum of the box (Origin)
        Point3D _min;
        /// @brief Maximum of the box (Dimension)
        Point3D _max;
        /// @brief Rotation of the box
        Vector3D _rotation;

        /// @brief Determines if a ray intersects the box and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the box
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the box
        /// @return True if the ray intersects the box, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the box with settings from a configuration file
        /// @param setting The configuration settings for the box
        void init(const libconfig::Setting& setting) override;
    };
}  // namespace RayTracer
