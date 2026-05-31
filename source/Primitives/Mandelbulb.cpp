/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Fractal_Mandelbulb
*/

#include "Mandelbulb.hpp"

#include "ConfigUtils.hpp"

void RayTracer::Mandelbulb::init(const libconfig::Setting& setting)
{
    ConfigUtils::parsePoint3D(setting, "position", _position, true);

    ConfigUtils::parsePoint3D(setting, "rotation", _rotation, false);

    if (!ConfigUtils::getAsDouble(setting, "scale", _scale)) _scale = 1.0;

    if (!ConfigUtils::getAsDouble(setting, "iterations", _iterations))
        throw RayTracerException("Fractal: Invalid or missing required parameter 'iterations'");

    if (!ConfigUtils::getAsDouble(setting, "bailout", _bailout))
        throw RayTracerException("Fractal: Invalid or missing required parameter 'bailout'");

    if (setting.exists("material")) {
        std::string name = setting["material"];
        _materialName = name;
    }

    if (!ConfigUtils::getAsDouble(setting, "power", _power))
        throw RayTracerException("Mandelbulb: Missing required parameter 'power'");
}

double RayTracer::Mandelbulb::distanceEstimate(const Math::Vector3D<double>& pos) const
{
    Math::Vector3D<double> z = pos / _scale;
    double dr = 1.0;
    double r = 0.0;

    for (int i = 0; i < _iterations; ++i) {
        r = z.length();
        if (r > _bailout) break;

        double theta = std::acos(z._z / r);
        double phi = std::atan2(z._y, z._x);
        dr = std::pow(r, _power - 1.0) * _power * dr + 1.0;

        double zr = std::pow(r, _power);
        theta *= _power;
        phi *= _power;
        z = Math::Vector3D<double>(zr * std::sin(theta) * std::cos(phi),
                                   zr * std::sin(theta) * std::sin(phi), zr * std::cos(theta)) +
            pos / _scale;
    }
    return (0.5 * std::log(r) * r / dr) * _scale;
}

extern "C" {
RayTracer::IPrimitive* entryPoint() { return new RayTracer::Mandelbulb(); }
}
