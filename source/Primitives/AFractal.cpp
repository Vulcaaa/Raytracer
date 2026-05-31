/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** AFractal
*/

#include "AFractal.hpp"

#include "ConfigUtils.hpp"
#include "RayTracerException.hpp"

bool RayTracer::AFractal::hits(const Ray& ray, HitRecord& hitRecord) const
{
    double t = 0.05;
    constexpr int MAX_STEPS = 300;
    constexpr double MAX_DIST = 20.0;
    constexpr double EPSILON = 0.001;

    hitRecord.primitive = this;
    hitRecord.material = _material.get();
    Math::Vector3D<double> origin = ray._origin - _position;
    Math::Vector3D<double> dir = rotateInverse(ray._direction);
    origin = rotateInverse(origin);

    for (int i = 0; i < MAX_STEPS; ++i) {
        Math::Vector3D<double> p = origin + dir * t;
        double d = distanceEstimate(p);

        if (d < EPSILON) {
            hitRecord.distance = t;
            hitRecord.p = ray._origin + ray._direction * t;
            hitRecord.normal = rotate(computeNormal(p));
            hitRecord.material->setColor(_material->getColor());
            return true;
        }
        t += d;
        if (t > MAX_DIST) return false;
    }
    return false;
}

Math::Vector3D<double> RayTracer::AFractal::computeNormal(const Math::Vector3D<double>& p) const
{
    constexpr double H = 0.0001;
    double dx = distanceEstimate({p._x + H, p._y, p._z}) - distanceEstimate({p._x - H, p._y, p._z});
    double dy = distanceEstimate({p._x, p._y + H, p._z}) - distanceEstimate({p._x, p._y - H, p._z});
    double dz = distanceEstimate({p._x, p._y, p._z + H}) - distanceEstimate({p._x, p._y, p._z - H});
    return Math::Vector3D<double>(dx, dy, dz).normalized();
}

Math::Vector3D<double> RayTracer::AFractal::rotateInverse(const Math::Vector3D<double>& v) const
{
    double rx = _rotation._x * M_PI / 180.0;
    double ry = _rotation._y * M_PI / 180.0;
    double rz = _rotation._z * M_PI / 180.0;

    double y1 = std::cos(rx) * v._y + std::sin(rx) * v._z;
    double z1 = -std::sin(rx) * v._y + std::cos(rx) * v._z;
    double x1 = v._x;

    double x2 = std::cos(ry) * x1 + std::sin(ry) * z1;
    double z2 = -std::sin(ry) * x1 + std::cos(ry) * z1;
    double y2 = y1;

    double x3 = std::cos(rz) * x2 - std::sin(rz) * y2;
    double y3 = std::sin(rz) * x2 + std::cos(rz) * y2;
    double z3 = z2;

    return {x3, y3, z3};
}

Math::Vector3D<double> RayTracer::AFractal::rotate(const Math::Vector3D<double>& v) const
{
    double rx = _rotation._x * M_PI / 180.0;
    double ry = _rotation._y * M_PI / 180.0;
    double rz = _rotation._z * M_PI / 180.0;

    double x1 = std::cos(rz) * v._x + std::sin(rz) * v._y;
    double y1 = -std::sin(rz) * v._x + std::cos(rz) * v._y;
    double z1 = v._z;

    double x2 = std::cos(ry) * x1 - std::sin(ry) * z1;
    double z2 = std::sin(ry) * x1 + std::cos(ry) * z1;
    double y2 = y1;

    double y3 = std::cos(rx) * y2 - std::sin(rx) * z2;
    double z3 = std::sin(rx) * y2 + std::cos(rx) * z2;
    double x3 = x2;

    return {x3, y3, z3};
}