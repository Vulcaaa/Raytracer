/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PointLight - Light source emitting from a specific position (implementation)
*/

#include "PointLight.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

#include "ConfigUtils.hpp"

void RayTracer::PointLight::init(const libconfig::Setting& setting)
{
    // Parse required intensity parameter
    if (!ConfigUtils::getAsDouble(setting, "intensity", _intensity))
        throw RayTracerException("PointLight: Missing required parameter 'intensity'.");

    // Parse required position parameter
    ConfigUtils::parsePoint3D(setting, "position", _position, true);

    // Parse optional translation and apply it directly to position
    Vector3D translation;
    ConfigUtils::parseVector3D(setting, "translation", translation, false);
    _position = _position + translation;

    // Parse optional rotation (stored for getDirectionFrom() use)
    ConfigUtils::parseVector3D(setting, "rotation", _rotation, false);

    // Parse common properties (color) from ALight
    parseCommonProperties(setting);

    // Parse optional attenuation parameters (bonus)
    ConfigUtils::getAsDouble(setting, "constant_attenuation", _constantAttenuation);
    ConfigUtils::getAsDouble(setting, "linear_attenuation", _linearAttenuation);
    ConfigUtils::getAsDouble(setting, "quadratic_attenuation", _quadraticAttenuation);

    // Parse optional radius parameter (bonus)
    ConfigUtils::getAsDouble(setting, "radius", _radius);
}

Math::Vector3D<double> RayTracer::PointLight::computeLight(const HitRecord& hit) const
{
    // Dynamic direction from hit point toward the light position
    Vector3D toLight = getDirectionFrom(hit.p);

    // Distance from hit point to light (used for attenuation and radius cutoff)
    double distance = (_position - hit.p).length();

    // Radius cutoff: light contributes nothing beyond its radius (bonus)
    if (_radius > 0.0 && distance > _radius) return {0.0, 0.0, 0.0};

    // Attenuation factor based on distance (bonus)
    double attenuation = calculateAttenuation(distance);

    // Lambert's cosine law: only surfaces facing the light receive contribution
    double dot = hit.normal.dot(toLight);
    double intensity = _intensity * std::max(0.0, dot) * attenuation;

    return (_color / 255.0) * intensity;
}

// Required by ILight interface — returns a zero vector because a point light
// has no fixed direction. Use getDirectionFrom(point) for the actual direction.
Math::Vector3D<double> RayTracer::PointLight::getDirection() const { return {0.0, 0.0, 0.0}; }

// Returns the normalized direction from a world-space point toward the light.
// This is what callers (renderer, shadow rays) should use for point lights.
Math::Vector3D<double> RayTracer::PointLight::getDirectionFrom(const Point3D& point) const
{
    return (_position - point).normalized();
}

double RayTracer::PointLight::getDistance(const Point3D& point) const
{
    return (_position - point).length();
}

Math::Vector3D<double> RayTracer::PointLight::applyRotation(Vector3D vec) const
{
    if (_rotation.length() == 0.0) return vec;

    double rx = _rotation._x * M_PI / 180.0;
    double ry = _rotation._y * M_PI / 180.0;
    double rz = _rotation._z * M_PI / 180.0;

    // Rotate around X axis
    double y1 = vec._y * std::cos(rx) - vec._z * std::sin(rx);
    double z1 = vec._y * std::sin(rx) + vec._z * std::cos(rx);
    vec._y = y1;
    vec._z = z1;

    // Rotate around Y axis
    double x2 = vec._x * std::cos(ry) + vec._z * std::sin(ry);
    double z2 = -vec._x * std::sin(ry) + vec._z * std::cos(ry);
    vec._x = x2;
    vec._z = z2;

    // Rotate around Z axis
    double x3 = vec._x * std::cos(rz) - vec._y * std::sin(rz);
    double y3 = vec._x * std::sin(rz) + vec._y * std::cos(rz);
    vec._x = x3;
    vec._y = y3;

    return vec;
}

double RayTracer::PointLight::calculateAttenuation(double distance) const
{
    // Clamp minimum distance to avoid division by zero / singularity at origin
    double d = std::max(distance, 0.001);

    double denominator =
        _constantAttenuation + _linearAttenuation * d + _quadraticAttenuation * d * d;

    return 1.0 / denominator;
}

extern "C" {
RayTracer::ILight* entryPoint() { return new RayTracer::PointLight(); }
}
