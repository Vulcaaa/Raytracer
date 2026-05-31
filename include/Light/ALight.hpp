/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ALight - Abstract Light base class
*/

#pragma once

#include <libconfig.h++>

#include "ILight.hpp"
#include "Vector3D.hpp"

namespace RayTracer {
    /// @brief Abstract base class for all light sources
    /// @details Provides common functionality for light sources including color and intensity
    class ALight : public ILight {
        public:
        virtual ~ALight() = default;

        /// @brief Initializes the light with settings from a configuration file
        /// @details Parses common color parameter
        /// @param setting The configuration settings for the light
        virtual void init(const libconfig::Setting& setting) = 0;

        bool castsShadow() const { return true; }

        Math::Vector3D<double> getDirection() const { return {0, 0, 0}; }

        bool hasDirection() const { return true; }

        protected:
        /// @brief Intensity of the light, used for rendering
        double _intensity = 1.0;

        /// @brief Color of the light, used for rendering (RGB values 0-255)
        Math::Vector3D<double> _color = {255, 255, 255};

        /// @brief Helper method to parse common light properties (color)
        /// @param setting The configuration settings for the light
        void parseCommonProperties(const libconfig::Setting& setting);
    };
}  // namespace RayTracer
