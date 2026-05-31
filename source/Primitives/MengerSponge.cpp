/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Manger Sponge
*/

#include "MengerSponge.hpp"

#include <algorithm>

#include "ConfigUtils.hpp"

void RayTracer::MengerSponge::init(const libconfig::Setting& setting)
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

double RayTracer::MengerSponge::boxDE(const Math::Vector3D<double>& p, double size) const
{
    Math::Vector3D<double> q(std::abs(p._x) - size, std::abs(p._y) - size, std::abs(p._z) - size);
    return Math::Vector3D<double>(std::max(q._x, 0.0), std::max(q._y, 0.0), std::max(q._z, 0.0))
               .length() +
           std::min(std::max({q._x, q._y, q._z}), 0.0);
}

namespace {
    double centeredModulo(double value, double period)
    {
        return value - period * std::floor(value / period + 0.5);
    }
}  // namespace

double RayTracer::MengerSponge::distanceEstimate(const Math::Vector3D<double>& pos) const
{
    Math::Vector3D<double> p = pos / _scale;
    double d = boxDE(p, 1.0);
    double s = 1.0;

    for (int i = 0; i < _iterations; ++i) {
        Math::Vector3D<double> a(centeredModulo(p._x * s, 2.0), centeredModulo(p._y * s, 2.0),
                                 centeredModulo(p._z * s, 2.0));
        s *= 3.0;

        // coordonnées dans la cellule, échelle 3
        double rx = std::abs(a._x * 3.0);
        double ry = std::abs(a._y * 3.0);
        double rz = std::abs(a._z * 3.0);

        double da = std::max(rx, ry);
        double db = std::max(ry, rz);
        double dc = std::max(rx, rz);
        double c = (std::min({da, db, dc}) - 1.0) / s;

        d = std::max(d, -c);  // soustraction booléenne → creuse les trous
    }
    return d * _scale;
}

extern "C" {
RayTracer::IPrimitive* entryPoint() { return new RayTracer::MengerSponge(); }
}
