/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** IPrimitive
*/

/// @file IPrimitive.hpp
/// @brief IPrimitive interface definition

#pragma once

#include <libconfig.h++>
#include <memory>
#include <string>

#include "IMaterial.hpp"
#include "Ray.hpp"

/// @brief Namespace for the Ray Tracer project
namespace RayTracer {
    class IPrimitive;
    /// @brief Struct to hold hit record information when a ray intersects a primitive
    struct HitRecord {
        /// @brief Distance from the ray origin to the hit point
        double distance;
        /// @brief Point of intersection in 3D space
        Math::Point3D<double> p;
        /// @brief Normal vector at the point of intersection
        Math::Vector3D<double> normal;
        /// @brief U texture coordinate
        double u = 0.0;
        /// @brief V texture coordinate
        double v = 0.0;
        const IPrimitive* primitive;
        /// @brief Material of the primitive that was hit
        IMaterial* material;
    };

    /// @brief Interface for geometric primitives that can be intersected by rays
    class IPrimitive {
        public:
        virtual ~IPrimitive() = default;

        /// @brief Initializes the primitive with settings from a configuration file
        /// @param setting The configuration settings for the primitive
        virtual void init(const libconfig::Setting& setting) = 0;

        /// @brief Determines if a ray intersects the primitive and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the primitive
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the primitive
        /// @return True if the ray intersects the primitive, false otherwise
        [[nodiscard]] virtual bool hits(const Ray& ray, HitRecord& hitRecord) const = 0;

        /// @brief Gets the color of the primitive
        /// @return The color vector of the primitive
        [[nodiscard]] virtual const Math::Vector3D<double> getColor() const = 0;

        /// @brief Sets the color of the primitive
        /// @param color The color vector to set for the primitive
        virtual void setColor(const Math::Vector3D<double>& color) = 0;

        [[nodiscard]] virtual const std::string& getMaterialName() const = 0;

        virtual const std::string& setMaterialName(const std::string& materialName) = 0;

        [[nodiscard]] virtual const std::shared_ptr<IMaterial>& getMaterial() const = 0;

        virtual const std::shared_ptr<IMaterial>& setMaterial(
            const std::shared_ptr<IMaterial>& material) = 0;

        virtual bool isGroup() const = 0;
    };
}  // namespace RayTracer
