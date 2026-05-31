/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** APrimitive
*/

#include "APrimitive.hpp"

namespace RayTracer {
    const Math::Vector3D<double> APrimitive::getColor() const
    {
        if (!_material) return Math::Vector3D<double>(0, 0, 0);
        return _material->getColor();
    }

    void APrimitive::setColor(const Math::Vector3D<double>& color)
    {
        if (_material) _material->setColor(color);
    }

    const std::string& APrimitive::getMaterialName() const { return _materialName; }

    const std::string& APrimitive::setMaterialName(const std::string& materialName)
    {
        _materialName = materialName;
        return _materialName;
    }

    const std::shared_ptr<IMaterial>& APrimitive::getMaterial() const { return _material; }

    const std::shared_ptr<IMaterial>& APrimitive::setMaterial(
        const std::shared_ptr<IMaterial>& material)
    {
        _material = material;
        return _material;
    }

    bool APrimitive::isGroup() const { return false; }
}  // namespace RayTracer
