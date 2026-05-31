/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ILight
*/

#pragma once

#include <libconfig.h++>

#include "IPrimitive.hpp"

namespace RayTracer {
    class ILight {
        public:
        virtual ~ILight() = default;

        /// @brief Initializes the light with settings from a configuration file
        /// @param setting The configuration settings for the light
        virtual void init(const libconfig::Setting& setting) = 0;

        /// @brief Color of the light, used for rendering
        /// @param hit The hit record for which to compute the light
        /// @return Retourne la couleur de la lumière
        [[nodiscard]] virtual Math::Vector3D<double> computeLight(
            [[maybe_unused]] const HitRecord& hit) const = 0;

        /// @brief Checks if the light casts shadows
        /// @return True if the light casts shadows, false otherwise
        virtual bool castsShadow() const = 0;

        /// @brief Gets the direction of the light (if applicable)
        /// @return The direction vector of the light
        virtual Math::Vector3D<double> getDirection() const = 0;

        /// @brief Checks if the light has a meaningful direction
        /// @return True if the light has a direction, false otherwise
        virtual bool hasDirection() const = 0;
    };
}  // namespace RayTracer
