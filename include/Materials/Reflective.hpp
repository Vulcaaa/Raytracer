/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Reflective
*/

#pragma once

#include "AMaterial.hpp"
#include "Vector3D.hpp"

namespace RayTracer {
    class Reflective : public AMaterial {
        public:
        Reflective() = default;

        /// @brief Initializes the material with settings from a configuration file
        /// @param setting The configuration settings for the material
        void init(const libconfig::Setting& setting) override;

        bool scatter(const Ray& rayIn, HitRecord& rec, Math::Vector3D<double>& attenuation,
                     Ray& scattered) const override;

        bool isReflective() const override { return true; };

        double getReflective() const override { return _reflectiveIndex; };

        private:
        /// @brief reflective index of the glass
        double _reflectiveIndex;
    };
}  // namespace RayTracer
