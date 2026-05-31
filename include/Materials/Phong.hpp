/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Phong
*/

#pragma once

#include "AMaterial.hpp"
#include "Vector3D.hpp"

namespace RayTracer {
    class Phong : public AMaterial {
        public:
        Phong() = default;

        /// @brief Initializes the material with settings from a configuration file
        /// @param setting The configuration settings for the material
        void init(const libconfig::Setting& setting) override;

        bool scatter(const Ray& rayIn, HitRecord& rec, Math::Vector3D<double>& attenuation,
                     Ray& scattered) const override;

        Math::Vector3D<double> computeSpecular(
            const Ray& ray, const HitRecord& rec,
            const std::vector<std::unique_ptr<ILight>>& lights,
            const std::vector<std::unique_ptr<IPrimitive>>& primitives) const override;

        bool hasSpecular() const override;

        double getShininess() const override;

        double getSpecularStrength() const override;

        private:
        double _shininess;

        double _specularStrength;
    };
}  // namespace RayTracer
