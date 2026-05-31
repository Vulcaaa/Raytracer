/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Reflective
*/

#include "Reflective.hpp"

#include "ConfigUtils.hpp"
#include "IPrimitive.hpp"
#include "MaterialUtils.hpp"
#include "RayTracerException.hpp"

void RayTracer::Reflective::init(const libconfig::Setting& setting)
{
    if (!setting.lookupValue("name", _name))
        throw RayTracerException("Reflective material must have a name");

    ConfigUtils::parseColor(setting, "color", _color);

    if (!ConfigUtils::getAsDouble(setting, "reflectiveIndex", _reflectiveIndex))
        throw RayTracerException("Reflective material must have a refraction parameter");
}

bool RayTracer::Reflective::scatter(const Ray& rayIn, HitRecord& rec,
                                    Math::Vector3D<double>& attenuation, Ray& scattered) const
{
    Math::Vector3D<double> normal = rec.normal;
    if (normal.dot(rayIn._direction) > 0) normal = -normal;

    Math::Vector3D<double> reflected =
        MaterialUtils::reflect(rayIn._direction.normalized(), normal);

    reflected = reflected + MaterialUtils::randomInUnitSphere() * (1.0 - _reflectiveIndex);

    attenuation = _color / 255.0;
    scattered = Ray(rec.p + normal * 0.001, reflected);
    return true;
}

extern "C" {
RayTracer::IMaterial* entryPoint() { return new RayTracer::Reflective(); }
}
