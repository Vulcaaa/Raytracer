/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** AMaterial
*/

#pragma once

#include "ILight.hpp"
#include "IMaterial.hpp"

namespace RayTracer {
    struct HitRecord;

    class AMaterial : public IMaterial {
        public:
        virtual ~AMaterial() = default;

        [[nodiscard]] inline std::string getName() const override { return _name; }

        [[nodiscard]] inline Math::Vector3D<double> getColor(
            [[maybe_unused]] double u = 0.0, [[maybe_unused]] double v = 0.0) const override
        {
            return _color;
        }

        void setColor(const Math::Vector3D<double>& color) override { _color = color; }

        bool isRefractive() const override { return false; };

        double getRefractive() const override { return 0.0; };

        bool isReflective() const override { return false; };

        double getReflective() const override { return 0.0; };

        Math::Vector3D<double> computeSpecular(
            const Ray& /*ray*/, const HitRecord& /*rec*/,
            const std::vector<std::unique_ptr<ILight>>& /*lights*/,
            const std::vector<std::unique_ptr<IPrimitive>>& /*primitives*/) const override
        {
            return {0, 0, 0};
        }

        virtual bool hasSpecular() const override { return false; }

        virtual double getShininess() const override { return 0.0; }

        virtual double getSpecularStrength() const override { return 0.0; }
    };
}  // namespace RayTracer
