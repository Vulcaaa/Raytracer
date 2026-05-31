/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.cpp
*/

/// @file Cone.cpp
/// @brief Implementation of the Cone class.

#include "Cone.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>

#include "ConfigUtils.hpp"
#include "RayTracerException.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {

    bool Cone::hits(const Ray& ray, HitRecord& rec) const
    {
        const double EPS = 1e-6;

        rec.material = _material.get();

        Vector3D axis = _axis.normalized();
        Vector3D originToApex = ray._origin - _apex;

        double dv = ray._direction.dot(axis);
        double ov = originToApex.dot(axis);

        Vector3D dD = ray._direction - axis * dv;
        Vector3D dO = originToApex - axis * ov;

        double a = dD.dot(dD) - _k * dv * dv;
        double b = 2.0 * (dD.dot(dO) - _k * dv * ov);
        double c = dO.dot(dO) - _k * ov * ov;

        double closest = std::numeric_limits<double>::max();
        Vector3D normal;
        bool hit = false;

        double discriminant = b * b - 4.0 * a * c;

        if (discriminant >= EPS && std::abs(a) > EPS) {
            double sqrtd = std::sqrt(discriminant);
            double t1 = (-b - sqrtd) / (2 * a);
            double t2 = (-b + sqrtd) / (2 * a);

            for (double t : {t1, t2}) {
                if (t <= EPS) continue;

                Point3D P = ray._origin + ray._direction * t;
                double h = (P - _apex).dot(axis);
                if (h < 0.0 || (_height.has_value() && h > _height.value())) continue;

                if (t < closest) {
                    closest = t;
                    hit = true;

                    Vector3D V = P - _apex;
                    double d = V.dot(axis);
                    Vector3D perp = V - axis * d;

                    normal = (perp - axis * _k * d).normalized();
                }
            }
        }

        if (_height.has_value()) {
            double denom = ray._direction.dot(axis);
            if (std::abs(denom) > EPS) {
                Point3D baseCenter = _apex + axis * _height.value();
                double tbase = (baseCenter - ray._origin).dot(axis) / denom;
                if (tbase > EPS && tbase < closest) {
                    Point3D P = ray._origin + ray._direction * tbase;
                    Vector3D v = P - baseCenter;
                    if (v.dot(v) <= _radius * _radius) {
                        closest = tbase;
                        hit = true;
                        normal = -axis;
                    }
                }
            }
        }

        if (!hit) return false;

        rec.distance = closest;
        rec.p = ray._origin + ray._direction * closest;
        rec.normal = normal;

        // UV Mapping
        Vector3D p = rec.p - _apex;
        double h = p.dot(axis);
        Vector3D radial = p - axis * h;

        double phi = std::atan2(radial._z, radial._x);
        if (phi < 0) phi += 2 * std::numbers::pi;

        rec.u = phi / (2 * std::numbers::pi);
        rec.v = h * 0.1;

        return true;
    }

    void Cone::init(const libconfig::Setting& setting)
    {
        ConfigUtils::parsePoint3D(setting, "position", _apex, true);

        Vector3D translation;
        ConfigUtils::parseVector3D(setting, "translation", translation, false);
        _apex = _apex + translation;

        Vector3D rotation;
        ConfigUtils::parseVector3D(setting, "rotation", rotation, false);

        // Default axis is pointing up (Y axis)
        _axis = Vector3D(0.0, 1.0, 0.0);
        _axis.rotateX(rotation._x);
        _axis.rotateY(rotation._y);
        _axis.rotateZ(rotation._z);
        _axis = _axis.normalized();

        if (!ConfigUtils::getAsDouble(setting, "r", _radius))
            throw RayTracerException("Cone: Missing required parameter 'r'.");

        if (_radius < 0) throw RayTracerException("Cone: Radius cannot be negative.");

        if (setting.exists("h")) {
            double h;
            ConfigUtils::getAsDouble(setting, "h", h);
            _height = h;
            _k = (_radius * _radius) / (h * h);
            if (h < 0) throw RayTracerException("Cone: Height cannot be negative.");
        } else {
            _height = std::nullopt;
        }

        if (setting.exists("d")) {
            double d;
            ConfigUtils::getAsDouble(setting, "d", d);
            _distance = d;
            _k = (_radius * _radius) / (d * d);
            if (d < 0) throw RayTracerException("Cone: Distance cannot be negative.");
        } else {
            _distance = std::nullopt;
        }

        if (!_distance.has_value() && !_height.has_value())
            throw RayTracerException("Cone: Missing parameter: distance or height.");

        if (_distance.has_value() && _height.has_value())
            throw RayTracerException(
                "Cone: Too many parameters: you must choose between distance or height.");

        if (setting.exists("material")) {
            std::string name = setting["material"];
            _materialName = name;
        }
    }

    extern "C" {
    IPrimitive* entryPoint() { return new Cone(); }
    }
}  // namespace RayTracer
