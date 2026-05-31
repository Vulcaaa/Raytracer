/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Triangle
*/

/// @file Triangle.hpp
/// @brief Triangle class definition

#pragma once

#include "APrimitive.hpp"
#include "Point3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a triangle in 3D space
    class Triangle : public APrimitive {
        public:
        Triangle() = default;

        /// @brief The three vertices of the triangle
        Point3D _v0;
        Point3D _v1;
        Point3D _v2;

        /// @brief Determines if a ray intersects the triangle and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the triangle
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the triangle
        /// @return True if the ray intersects the triangle, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the triangle with settings from a configuration file
        /// @param setting The configuration settings for the triangle
        void init(const libconfig::Setting& setting) override;
    };
}  // namespace RayTracer
