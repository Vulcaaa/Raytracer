/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Phong
*/

#include "Phong.hpp"

#include "ConfigUtils.hpp"
#include "IPrimitive.hpp"
#include "MaterialUtils.hpp"
#include "RayTracerException.hpp"

void RayTracer::Phong::init(const libconfig::Setting& setting)
{
    if (!setting.lookupValue("name", _name))
        throw RayTracerException("Lambertian material must have a name");

    ConfigUtils::parseColor(setting, "color", _color);

    if (!ConfigUtils::getAsDouble(setting, "shininess", _shininess)) _shininess = 32;

    if (!ConfigUtils::getAsDouble(setting, "specularStrength", _specularStrength))
        _specularStrength = 0.5;
}

bool RayTracer::Phong::scatter(const Ray& /*rayIn*/, HitRecord& rec,
                               Math::Vector3D<double>& attenuation, Ray& scattered) const
{
    Math::Vector3D<double> scatterDir = rec.normal + MaterialUtils::randomUnitVector();

    if (scatterDir.dot(scatterDir) < 1e-8) scatterDir = rec.normal;

    scattered = Ray(rec.p, scatterDir);
    attenuation = _color / 255.0;
    return true;
}

Math::Vector3D<double> RayTracer::Phong::computeSpecular(
    const Ray& ray, const HitRecord& rec, const std::vector<std::unique_ptr<ILight>>& lights,
    const std::vector<std::unique_ptr<IPrimitive>>& primitives) const
{
    Math::Vector3D<double> specular(0.0, 0.0, 0.0);
    Math::Vector3D<double> V = (-ray._direction).normalized();

    for (const auto& light : lights) {
        if (!light->hasDirection()) continue;
        if (light->castsShadow() &&
            MaterialUtils::isInShadow(rec, light->getDirection(), primitives))
            continue;

        Math::Vector3D<double> L = light->getDirection();
        Math::Vector3D<double> R = (rec.normal * 2.0 * rec.normal.dot(L) - L).normalized();
        double spec = std::pow(std::max(R.dot(V), 0.0), _shininess) * _specularStrength;
        specular += {spec, spec, spec};
    }
    return specular;
}

bool RayTracer::Phong::hasSpecular() const { return true; }

double RayTracer::Phong::getShininess() const { return _shininess; }

double RayTracer::Phong::getSpecularStrength() const { return _specularStrength; }

extern "C" {
RayTracer::IMaterial* entryPoint() { return new RayTracer::Phong(); }
}