/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Sphere
*/

/// @file Sphere.hpp
/// @brief Sphere class definition

#pragma once

#include "APrimitive.hpp"
#include "Point3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a sphere in 3D space
    class Sphere : public APrimitive {
        public:
        Sphere() = default;

        /// @brief Center point of the sphere
        Point3D _center;
        /// @brief Radius of the sphere
        double _radius;

        /// @brief Determines if a ray intersects the sphere and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the sphere
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the sphere
        /// @return True if the ray intersects the sphere, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the sphere with settings from a configuration file
        /// @param setting The configuration settings for the sphere
        void init(const libconfig::Setting& setting) override;

        private:
        Vector3D applyRotation(Vector3D& normal) const;

        /// @brief Rotation of the sphere (useful for texture)
        Vector3D _rotation = {0.0, 0.0, 0.0};
    };
}  // namespace RayTracer
