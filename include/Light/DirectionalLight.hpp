/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** DirectionalLight
*/

#pragma once

#include "ALight.hpp"
#include "math.h"

namespace RayTracer {
    class DirectionalLight : public ALight {
        public:
        /// @brief Default constructor for DirectionalLight
        DirectionalLight() = default;

        /// @brief Initializes the light with settings from a configuration file
        /// @param setting The configuration settings for the light
        void init(const libconfig::Setting& setting) override;

        /// @brief Gets the direction of the light
        /// @return Returns the direction of the light
        Math::Vector3D<double> getDirection() const override;

        /// @brief Computes the light contribution at a given hit point
        /// @param hit The hit record for which to compute the light
        /// @return Returns the color of the light contribution at the hit point
        [[nodiscard]] Math::Vector3D<double> computeLight(
            [[maybe_unused]] const HitRecord& hit) const override;

        private:
        /// @brief Direction of the light, used for rendering
        Vector3D _direction;
    };
}  // namespace RayTracer
