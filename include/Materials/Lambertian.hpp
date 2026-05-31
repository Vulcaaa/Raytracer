/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Lambertian
*/

#pragma once

#include "AMaterial.hpp"
#include "Vector3D.hpp"

namespace RayTracer {
    class Lambertian : public AMaterial {
        public:
        Lambertian() = default;

        /// @brief Initializes the material with settings from a configuration file
        /// @param setting The configuration settings for the material
        void init(const libconfig::Setting& setting) override;

        bool scatter(const Ray& rayIn, HitRecord& rec, Math::Vector3D<double>& attenuation,
                     Ray& scattered) const override;
    };
}  // namespace RayTracer
