/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Plane
*/

/// @file Plane.hpp
/// @brief Plane class definition

#pragma once

#include <string>

#include "APrimitive.hpp"
#include "Point3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a plane in 3D space
    class Plane : public APrimitive {
        public:
        /// @brief Constructs a plane object
        Plane() = default;

        /// @brief Normal vector of the plane
        Vector3D _normal;

        /// @brief Position of the plane in 3D space
        Point3D _position;

        /// @brief Determines if a ray intersects the plane and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the plane
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the plane
        /// @return True if the ray intersects the plane, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the plane with settings from a configuration file
        /// @param setting The configuration settings for the plane
        void init(const libconfig::Setting& setting) override;
    };
}  // namespace RayTracer
