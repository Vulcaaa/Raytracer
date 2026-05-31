/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Texture
*/

#pragma once

#include <SFML/Graphics/Image.hpp>
#include <libconfig.h++>
#include <memory>
#include <string>

#include "AMaterial.hpp"

namespace RayTracer {
    /// @brief Material that applies a texture to a primitive using UV mapping
    class Texture : public AMaterial {
        public:
        /// @brief Initializes the texture material with settings from a configuration file
        /// @param setting The configuration settings for the texture material
        void init(const libconfig::Setting& setting) override;

        /// @brief Computes the scattered ray and attenuation for a given ray and hit record
        /// @param rayIn Incoming ray that hits the material
        /// @param rec Hit record for the intersection
        /// @param attenuation Output parameter for the attenuation (color reduction) of the
        /// scattered ray
        /// @param scattered Output parameter for the scattered ray
        /// @return True if the ray was scattered, false otherwise
        bool scatter(const Ray& /*rayIn*/, HitRecord& rec, Math::Vector3D<double>& attenuation,
                     Ray& scattered) const override;

        /// @brief Gets the color of the texture at given UV coordinates
        /// @param u U coordinate
        /// @param v V coordinate
        /// @return Color from the texture scaled 0-255
        [[nodiscard]] Math::Vector3D<double> getColor(double u = 0.0,
                                                      double v = 0.0) const override;

        private:
        /// @brief Gets the luminance of a pixel
        /// @param pixel the pixel to which we calculate the luminance
        /// @return luminance
        double getLuminance(sf::Color pixel) const;

        /// @brief File path to the texture image
        std::string _filepath;
        /// @brief Width of the texture image
        int _width = 0;
        /// @brief Height of the texture image
        int _height = 0;
        /// @brief SFML image object that holds the texture data
        sf::Image _image;

        /// @brief strength of the normal mapping
        double _strength = 0;
        /// @brief The increment step used for texture sampling and gradient calculation
        double _step = 0;
    };
}  // namespace RayTracer
