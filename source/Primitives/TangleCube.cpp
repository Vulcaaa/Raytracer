/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Tangle Cube
*/

#include "TangleCube.hpp"

#include <algorithm>

#include "ConfigUtils.hpp"

void RayTracer::TangleCube::init(const libconfig::Setting& setting)
{
    ConfigUtils::parsePoint3D(setting, "position", _position, true);

    ConfigUtils::parsePoint3D(setting, "rotation", _rotation, false);

    if (!ConfigUtils::getAsDouble(setting, "scale", _scale)) _scale = 1.0;

    if (!ConfigUtils::getAsDouble(setting, "constant", _constant))
        throw RayTracerException("Fractal: Invalid or missing required parameter 'constant'");

    if (setting.exists("material")) {
        std::string name = setting["material"];
        _materialName = name;
    }
}

double RayTracer::TangleCube::distanceEstimate(const Math::Vector3D<double>& pos) const
{
    Math::Vector3D<double> p = pos / _scale;

    double x2 = p._x * p._x, y2 = p._y * p._y, z2 = p._z * p._z;
    double x4 = x2 * x2, y4 = y2 * y2, z4 = z2 * z2;

    double f = x4 - 5.0 * x2 + y4 - 5.0 * y2 + z4 - 5.0 * z2 + _constant;

    double gx = 4.0 * p._x * x2 - 10.0 * p._x;
    double gy = 4.0 * p._y * y2 - 10.0 * p._y;
    double gz = 4.0 * p._z * z2 - 10.0 * p._z;
    double grad = std::sqrt(gx * gx + gy * gy + gz * gz);

    double dist = (f / std::max(grad, 1e-6)) * _scale;
    return std::clamp(dist, -0.1 * _scale, 0.5 * _scale);
}

extern "C" {
RayTracer::IPrimitive* entryPoint() { return new RayTracer::TangleCube(); }
}
