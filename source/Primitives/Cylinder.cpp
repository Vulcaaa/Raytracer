/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.cpp
*/

/// @file Cylinder.cpp
/// @brief Implementation of the Cylinder class.

#include "Cylinder.hpp"

#include <cmath>
#include <memory>
#include <numbers>

#include "ConfigUtils.hpp"
#include "RayTracerException.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {

    bool Cylinder::hits(const Ray& ray, HitRecord& rec) const
    {
        rec.primitive = this;
        rec.material = _material.get();
        const double EPS = 1e-6;

        Vector3D originToBase = ray._origin - _base;
        Vector3D axis = _axis.normalized();

        Vector3D dD = ray._direction - axis * ray._direction.dot(axis);
        Vector3D dO = originToBase - axis * originToBase.dot(axis);

        double a = dD.dot(dD);
        double b = 2.0 * dD.dot(dO);
        double c = dO.dot(dO) - _radius * _radius;

        double closest = std::numeric_limits<double>::max();
        Vector3D normal;
        bool hit = false;

        double discriminant = b * b - 4.0 * a * c;
        if (discriminant >= 0 && std::abs(a) > EPS) {
            double sqrtd = std::sqrt(discriminant);
            double t1 = (-b - sqrtd) / (2 * a);
            double t2 = (-b + sqrtd) / (2 * a);

            for (double t : {t1, t2}) {
                if (t < EPS) continue;

                Point3D P = ray._origin + ray._direction * t;
                double h = (P - _base).dot(axis);

                bool inBounds = !_height.has_value() || (h >= 0 && h <= _height.value());
                if (inBounds && t < closest) {
                    closest = t;
                    hit = true;

                    Vector3D radial = (P - _base) - axis * h;
                    normal = radial.normalized();
                }
            }
        }

        if (_height.has_value()) {
            double denom = ray._direction.dot(axis);

            if (std::abs(denom) > EPS) {
                double t = (_base - ray._origin).dot(axis) / denom;
                if (t > EPS && t < closest) {
                    Point3D P = ray._origin + ray._direction * t;
                    Vector3D v = P - _base;

                    if (v.dot(v) <= _radius * _radius) {
                        closest = t;
                        hit = true;
                        normal = axis * -1.0;
                    }
                }

                Point3D top = _base + axis * _height.value();
                t = (top - ray._origin).dot(axis) / denom;

                if (t > EPS && t < closest) {
                    Point3D P = ray._origin + ray._direction * t;
                    Vector3D v = P - top;

                    if (v.dot(v) <= _radius * _radius) {
                        closest = t;
                        hit = true;
                        normal = axis;
                    }
                }
            }
        }
        if (!hit) return false;

        rec.distance = closest;
        rec.p = ray._origin + ray._direction * closest;
        rec.normal = normal;

        // UV Mapping
        double phi = std::atan2(normal._z, normal._x);
        if (phi < 0) phi += 2 * std::numbers::pi;
        rec.u = phi / (2 * std::numbers::pi);
        double h = (rec.p - _base).dot(_axis.normalized());
        rec.v = h * 0.1;  // scale factor

        return true;
    }

    void Cylinder::init(const libconfig::Setting& setting)
    {
        ConfigUtils::parsePoint3D(setting, "position", _base, true);

        Math::Vector3D<double> translation;
        ConfigUtils::parseVector3D(setting, "translation", translation, false);
        _base = _base + translation;

        Math::Vector3D<double> rotation;
        ConfigUtils::parseVector3D(setting, "rotation", rotation, false);

        // Default axis is pointing up (Y axis)
        _axis = Math::Vector3D<double>(0.0, 1.0, 0.0);
        _axis.rotateX(rotation._x);
        _axis.rotateY(rotation._y);
        _axis.rotateZ(rotation._z);
        _axis = _axis.normalized();

        if (!ConfigUtils::getAsDouble(setting, "r", _radius))
            throw RayTracerException("Cylinder: Missing required parameter 'r'.");

        if (_radius < 0) throw RayTracerException("Cylinder: Radius cannot be negative.");

        if (setting.exists("h")) {
            double h = 0.0;
            ConfigUtils::getAsDouble(setting, "h", h);
            _height = h;
        } else
            _height = std::nullopt;

        if (_height.has_value() && _height.value() < 0)
            throw RayTracerException("Cylinder: Height cannot be negative.");

        if (setting.exists("material")) {
            std::string name = setting["material"];
            _materialName = name;
        }
    }

    extern "C" {
    IPrimitive* entryPoint() { return new Cylinder(); }
    }
}  // namespace RayTracer
