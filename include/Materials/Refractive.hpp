/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Refractive
*/

#pragma once

#include "AMaterial.hpp"
#include "Vector3D.hpp"

namespace RayTracer {
    class Refractive : public AMaterial {
        public:
        Refractive() = default;

        /// @brief Initializes the material with settings from a configuration file
        /// @param setting The configuration settings for the material
        void init(const libconfig::Setting& setting) override;

        bool scatter(const Ray& rayIn, HitRecord& rec, Math::Vector3D<double>& attenuation,
                     Ray& scattered) const override;

        bool isRefractive() const override { return true; }

        double getRefractive() const override { return _transparency; };

        private:
        /// @brief Transparency of the glass
        double _transparency;

        /// @brief refrative index of the glass
        double _refractiveIndex;
    };
}  // namespace RayTracer
