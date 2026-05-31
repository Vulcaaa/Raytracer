#include "AmbientLight.hpp"

#include <memory>

#include "ConfigUtils.hpp"

Math::Vector3D<double> RayTracer::AmbientLight::computeLight(
    [[maybe_unused]] const HitRecord& hit) const
{
    return (_color / 255.0) * _intensity;
}

bool RayTracer::AmbientLight::castsShadow() const { return false; }

void RayTracer::AmbientLight::init(const libconfig::Setting& setting)
{
    _intensity = 0.0;
    ConfigUtils::getAsDouble(setting, "ambient", _intensity);

    parseCommonProperties(setting);
}

extern "C" {
RayTracer::ILight* entryPoint() { return new RayTracer::AmbientLight(); }
}
