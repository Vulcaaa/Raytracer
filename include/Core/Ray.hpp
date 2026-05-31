/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Ray
*/

/// @file Ray.hpp
/// @brief Ray class definition

#pragma once

#include "Point3D.hpp"
#include "Vector3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a ray in 3D space
    class Ray {
        public:
        /// @brief Default constructor
        Ray() = default;
        /// @brief Constructs a Ray object
        /// @param origin The starting point of the ray in 3D space
        /// @param direction The direction vector of the ray
        Ray(const Point3D& origin, const Vector3D& direction)
            : _origin(origin), _direction(direction)
        {
        }

        /// @brief Origin point of the ray
        Point3D _origin;
        /// @brief Direction vector of the ray
        Vector3D _direction;
    };
}  // namespace RayTracer
