/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Lambertian
*/

#include "Lambertian.hpp"

#include "ConfigUtils.hpp"
#include "IPrimitive.hpp"
#include "MaterialUtils.hpp"
#include "RayTracerException.hpp"

void RayTracer::Lambertian::init(const libconfig::Setting& setting)
{
    if (!setting.lookupValue("name", _name))
        throw RayTracerException("Lambertian material must have a name");
    ConfigUtils::parseColor(setting, "color", _color);
}

bool RayTracer::Lambertian::scatter(const Ray& /*rayIn*/, HitRecord& rec,
                                    Math::Vector3D<double>& attenuation, Ray& scattered) const
{
    Math::Vector3D<double> scatterDir = rec.normal + MaterialUtils::randomUnitVector();

    if (scatterDir.dot(scatterDir) < 1e-8) scatterDir = rec.normal;

    scattered = Ray(rec.p, scatterDir);
    attenuation = _color / 255.0;
    return true;
}

extern "C" {
RayTracer::IMaterial* entryPoint() { return new RayTracer::Lambertian(); }
}