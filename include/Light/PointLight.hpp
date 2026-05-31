/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PointLight - Light source emitting from a specific position
*/

#pragma once

#include "ALight.hpp"
#include "Point3D.hpp"
#include "math.h"

namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a point light source in 3D space
    /// @details A point light emits light in all directions from a specific position
    class PointLight : public ALight {
        public:
        /// @brief Default constructor for PointLight
        PointLight() = default;

        /// @brief Initializes the point light with settings from a configuration file
        /// @param setting The configuration settings for the light
        void init(const libconfig::Setting& setting) override;

        /// @brief Computes the light contribution at a given hit point
        /// @param hit The hit record for which to compute the light
        /// @return Returns the color of the light contribution at the hit point
        [[nodiscard]] Math::Vector3D<double> computeLight(const HitRecord& hit) const override;

        /// @brief Gets the direction from hit point to light position
        /// @return Returns the normalized direction vector from hit to light
        Math::Vector3D<double> getDirection() const override;

        /// @brief Point lights have no fixed direction (direction depends on hit point)
        /// @return Always returns false for point lights
        bool hasDirection() const override { return false; }

        /// @brief Gets the position of the point light
        /// @return Returns the 3D position of the light
        Point3D getPosition() const { return _position; }

        /// @brief Gets the distance from a point to this light
        /// @param point The point to calculate distance from
        /// @return Returns the distance from the point to the light
        double getDistance(const Point3D& point) const;

        private:
        /// @brief Position of the point light in world space
        Point3D _position = {0, 0, 0};

        /// @brief Rotation of the light (for potential future use)
        Vector3D _rotation = {0.0, 0.0, 0.0};

        /// @brief Constant attenuation factor (bonus feature)
        double _constantAttenuation = 1.0;

        /// @brief Linear attenuation factor (bonus feature)
        double _linearAttenuation = 0.09;

        /// @brief Quadratic attenuation factor (bonus feature)
        double _quadraticAttenuation = 0.032;

        /// @brief Radius of the light source (bonus feature) - 0 means no radius
        double _radius = 0.0;

        /// @brief Applies rotation transformation to a direction vector
        /// @param vec The vector to rotate
        /// @return Returns the rotated vector
        Vector3D applyRotation(Vector3D vec) const;

        /// @brief Calculates attenuation based on distance
        /// @param distance The distance from light to hit point
        /// @return Returns the attenuation factor (0.0 to 1.0)
        double calculateAttenuation(double distance) const;

        /// @brief Gets the normalized direction from a world-space point toward the light
        /// @param point The world-space point
        /// @return Returns the normalized direction vector toward the light
        Math::Vector3D<double> getDirectionFrom(const Point3D& point) const;
    };
}  // namespace RayTracer
