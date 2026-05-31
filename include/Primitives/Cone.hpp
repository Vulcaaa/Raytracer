/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.hpp
*/

/// @file Cone.hpp
/// @brief Cone class definition

#pragma once

#include <optional>

#include "APrimitive.hpp"
#include "Point3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a Cone in 3D space
    class Cone : public APrimitive {
        public:
        Cone() = default;

        /// @brief Apex of the cone
        Point3D _apex;
        /// @brief Radius of the cone
        /// if the cone is infinite the radius is base on a point (defined by the '_distance')
        /// if the cone is finished the radius is base on the cone's base
        double _radius;
        /// @brief The height of the cone (distance between apex and base)
        std::optional<double> _height;
        /// @brief The distance between the apex of the cone
        /// and the reference point, where the radius is set
        std::optional<double> _distance;
        /// @brief Orientation of the cone
        Vector3D _axis;
        /// @brief Slope factor of the cone used for ray-cone intersection.
        double _k;

        /// @brief Determines if a ray intersects the cone and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the cone
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the cone
        /// @return True if the ray intersects the cone, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the cone with settings from a configuration file
        /// @param setting The configuration settings for the cone
        void init(const libconfig::Setting& setting) override;
    };
}  // namespace RayTracer
