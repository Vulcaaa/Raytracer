#include "DirectionalLight.hpp"

#include <algorithm>
#include <iostream>
#include <memory>

#include "ConfigUtils.hpp"

Math::Vector3D<double> RayTracer::DirectionalLight::computeLight(const HitRecord& hit) const
{
    Math::Vector3D<double> toLight = getDirection();
    double dot = hit.normal.dot(toLight);
    double intensity = _intensity * std::max(0.0, dot);
    return (_color / 255.0) * intensity;
}

Math::Vector3D<double> RayTracer::DirectionalLight::getDirection() const { return -_direction; }

void RayTracer::DirectionalLight::init(const libconfig::Setting& setting)
{
    if (!ConfigUtils::getAsDouble(setting, "intensity", _intensity))
        throw RayTracerException("DirectionalLight: Missing required parameter 'intensity'.");

    parseCommonProperties(setting);

    Vector3D direction;
    ConfigUtils::parseVector3D(setting, "direction", direction, true);
    _direction = direction.normalized();
}

extern "C" {
RayTracer::ILight* entryPoint() { return new RayTracer::DirectionalLight(); }
}
