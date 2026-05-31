/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** AmbientLight
*/

#include "ALight.hpp"
#include "math.h"

#pragma once

namespace RayTracer {
    class AmbientLight : public ALight {
        public:
        /// @brief Default constructor for AmbientLight
        AmbientLight() = default;

        /// @brief Checks if the light casts shadows
        /// @return True if the light casts shadows, false otherwise
        bool castsShadow() const override;

        /// @brief Initializes the light with settings from a configuration file
        /// @param setting The configuration settings for the light
        void init(const libconfig::Setting& setting) override;

        /// @brief Color of the light, used for rendering
        /// @param hit The hit record for which to compute the light
        /// @return Retourne la couleur de la lumière
        [[nodiscard]] Math::Vector3D<double> computeLight(
            [[maybe_unused]] const HitRecord& hit) const override;

        bool hasDirection() const override { return false; }
    };
}  // namespace RayTracer
