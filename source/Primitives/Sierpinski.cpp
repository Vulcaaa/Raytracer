/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Sierpinski
*/

#include "Sierpinski.hpp"

#include "ConfigUtils.hpp"

void RayTracer::Sierpinski::init(const libconfig::Setting& setting)
{
    ConfigUtils::parsePoint3D(setting, "position", _position, true);

    ConfigUtils::parsePoint3D(setting, "rotation", _rotation, false);

    if (!ConfigUtils::getAsDouble(setting, "scale", _scale)) _scale = 1.0;

    if (!ConfigUtils::getAsDouble(setting, "iterations", _iterations))
        throw RayTracerException("Fractal: Invalid or missing required parameter 'iterations'");

    if (setting.exists("material")) {
        std::string name = setting["material"];
        _materialName = name;
    }
}

double RayTracer::Sierpinski::distanceEstimate(const Math::Vector3D<double>& pos) const
{
    static const Math::Vector3D<double> v0(1.0, 1.0, 1.0);
    static const Math::Vector3D<double> v1(-1.0, -1.0, 1.0);
    static const Math::Vector3D<double> v2(-1.0, 1.0, -1.0);
    static const Math::Vector3D<double> v3(1.0, -1.0, -1.0);

    Math::Vector3D<double> p = pos / _scale;
    double scale = 1.0;

    for (int i = 0; i < _iterations; ++i) {
        double d0 = (p - v0).length();
        double d1 = (p - v1).length();
        double d2 = (p - v2).length();
        double d3 = (p - v3).length();

        Math::Vector3D<double> closest = v0;
        double dmin = d0;
        if (d1 < dmin) {
            dmin = d1;
            closest = v1;
        }
        if (d2 < dmin) {
            dmin = d2;
            closest = v2;
        }
        if (d3 < dmin) {
            dmin = d3;
            closest = v3;
        }

        p = p * 2.0 - closest;
        scale *= 2.0;
    }
    return (p.length() / scale) * _scale;
}

extern "C" {
RayTracer::IPrimitive* entryPoint() { return new RayTracer::Sierpinski(); }
}
