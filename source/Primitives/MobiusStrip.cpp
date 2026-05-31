/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** MobiusStrip
*/

#include "MobiusStrip.hpp"

#include "ConfigUtils.hpp"
#include "RayTracerException.hpp"

void RayTracer::MobiusStrip::init(const libconfig::Setting& setting)
{
    ConfigUtils::parsePoint3D(setting, "position", _position, true);

    ConfigUtils::parsePoint3D(setting, "rotation", _rotation, false);

    if (!ConfigUtils::getAsDouble(setting, "scale", _scale)) _scale = 1.0;

    if (!ConfigUtils::getAsDouble(setting, "radius", _radius)) _radius = 1.0;

    if (!ConfigUtils::getAsDouble(setting, "width", _width)) _width = 0.4;

    if (!ConfigUtils::getAsDouble(setting, "thickness", _thickness)) _thickness = 0.05;

    if (!ConfigUtils::getAsDouble(setting, "halfTwist", _halfTwist)) _halfTwist = 1;

    if (setting.exists("material")) {
        std::string name = setting["material"];
        _materialName = name;
    }
}

double RayTracer::MobiusStrip::distanceEstimate(const Math::Vector3D<double>& p) const
{
    Math::Vector3D<double> ps = p / _scale;

    double u = std::atan2(ps._y, ps._x);

    double cu = std::cos(u), su = std::sin(u);
    double cu2 = std::cos(u * _halfTwist / 2.0);
    double su2 = std::sin(u * _halfTwist / 2.0);

    Math::Vector3D<double> center(_radius * cu, _radius * su, 0.0);
    Math::Vector3D<double> diff = ps - center;

    double along_width = diff._x * cu2 * cu + diff._y * cu2 * su + diff._z * su2;
    double along_normal = -diff._x * su2 * cu - diff._y * su2 * su + diff._z * cu2;

    double dw = std::abs(along_width) - _width;
    double dn = std::abs(along_normal) - _thickness;

    return std::max(dw, dn) * _scale;
}

extern "C" {
RayTracer::IPrimitive* entryPoint() { return new RayTracer::MobiusStrip(); }
}
