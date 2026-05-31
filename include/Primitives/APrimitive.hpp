/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** APrimitive
*/

#pragma once

#include "IPrimitive.hpp"

namespace RayTracer {
    class APrimitive : public IPrimitive {
        public:
        ~APrimitive() override = default;

        [[nodiscard]] const Math::Vector3D<double> getColor() const override;

        void setColor(const Math::Vector3D<double>& color) override;

        [[nodiscard]] const std::string& getMaterialName() const override;

        const std::string& setMaterialName(const std::string& materialName) override;

        [[nodiscard]] const std::shared_ptr<IMaterial>& getMaterial() const override;

        const std::shared_ptr<IMaterial>& setMaterial(
            const std::shared_ptr<IMaterial>& material) override;

        bool isGroup() const override;

        protected:
        std::string _materialName;
        std::shared_ptr<IMaterial> _material;
    };
}  // namespace RayTracer
