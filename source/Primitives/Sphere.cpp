/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Sphere
*/

/// @file Sphere.cpp
/// @brief Implementation of the Sphere class.

#include "Sphere.hpp"

#include <cmath>
#include <memory>
#include <numbers>

#include "ConfigUtils.hpp"
#include "RayTracerException.hpp"

bool RayTracer::Sphere::hits(const Ray& ray, HitRecord& rec) const
{
    rec.primitive = this;
    rec.material = _material.get();
    Vector3D originToCenter = ray._origin - _center;

    double a = ray._direction.dot(ray._direction);
    double b = 2.0 * ray._direction.dot(originToCenter);
    double c = originToCenter.dot(originToCenter) - (_radius * _radius);

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0) return false;

    double sqrtd = std::sqrt(discriminant);
    double root = (-b - sqrtd) / (2.0 * a);
    if (root <= 0.001) {
        root = (-b + sqrtd) / (2.0 * a);
        if (root <= 0.001) return false;
    }

    rec.distance = root;
    rec.p = ray._origin + (ray._direction * rec.distance);
    rec.normal = (rec.p - _center) / _radius;

    Math::Vector3D<double> n = applyRotation(rec.normal);

    // UV Mapping
    double theta = std::acos(-n._y);
    double phi = std::atan2(-n._z, n._x) + std::numbers::pi;
    rec.u = phi / (2.0 * std::numbers::pi);
    rec.v = theta / std::numbers::pi;

    return true;
}

Math::Vector3D<double> RayTracer::Sphere::applyRotation(Math::Vector3D<double>& normal) const
{
    double rx = _rotation._x * std::numbers::pi / 180.0;
    double ry = _rotation._y * std::numbers::pi / 180.0;
    double rz = _rotation._z * std::numbers::pi / 180.0;

    Math::Vector3D<double> v = normal;
    double y1 = v._y * std::cos(rx) - v._z * std::sin(rx);
    double z1 = v._y * std::sin(rx) + v._z * std::cos(rx);
    v._y = y1;
    v._z = z1;

    double x2 = v._x * std::cos(ry) + v._z * std::sin(ry);
    double z2 = -v._x * std::sin(ry) + v._z * std::cos(ry);
    v._x = x2;
    v._z = z2;

    double x3 = v._x * std::cos(rz) - v._y * std::sin(rz);
    double y3 = v._x * std::sin(rz) + v._y * std::cos(rz);
    v._x = x3;
    v._y = y3;

    return v;
}

void RayTracer::Sphere::init(const libconfig::Setting& setting)
{
    ConfigUtils::parsePoint3D(setting, "position", _center, true);

    Math::Vector3D<double> translation;
    ConfigUtils::parseVector3D(setting, "translation", translation, false);
    _center = _center + translation;

    Math::Vector3D<double> rotation;
    ConfigUtils::parseVector3D(setting, "rotation", rotation, false);
    _rotation = rotation;

    if (!ConfigUtils::getAsDouble(setting, "r", _radius))
        throw RayTracer::RayTracerException("Sphere: Missing required parameter 'r'.");

    if (_radius < 0) throw RayTracer::RayTracerException("Sphere: Radius cannot be negative.");

    if (setting.exists("material")) {
        std::string name = setting["material"];
        _materialName = name;
    }
}

extern "C" {
RayTracer::IPrimitive* entryPoint() { return new RayTracer::Sphere(); }
}
