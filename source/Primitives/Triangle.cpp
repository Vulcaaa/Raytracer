/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Triangle
*/

/// @file Triangle.cpp
/// @brief Implementation of the Triangle class.

#include "Triangle.hpp"

#include <cmath>
#include <memory>

#include "ConfigUtils.hpp"
#include "RayTracerException.hpp"

bool RayTracer::Triangle::hits(const Ray& ray, HitRecord& rec) const
{
    const double epsilon = 1e-6;

    rec.primitive = this;
    rec.material = _material.get();
    // Calcul des arêtes
    Vector3D edge1 = _v1 - _v0;
    Vector3D edge2 = _v2 - _v0;

    Vector3D pvec = ray._direction.cross(edge2);

    double det = edge1.dot(pvec);

    // "det" proche de 0 = rayon parallèle au triangle
    if (std::abs(det) < epsilon) return false;

    double invDet = 1.0 / det;

    Vector3D tvec = ray._origin - _v0;
    double u = tvec.dot(pvec) * invDet;

    // "u" en dehors du triangle
    if (u < 0.0 || u > 1.0) return false;

    Vector3D qvec = tvec.cross(edge1);
    double v = ray._direction.dot(qvec) * invDet;
    // "v" en dehors du triangle
    if (v < 0.0 || u + v > 1.0) return false;

    double t = edge2.dot(qvec) * invDet;

    // Vérifie que l'intersection est devant la caméra
    if (t <= epsilon) return false;

    // Remplir le HitRecord
    rec.distance = t;
    rec.p = ray._origin + (ray._direction * t);
    rec.u = u;
    rec.v = v;

    // Calculer la normale du triangle
    Vector3D normal = edge1.cross(edge2).normalized();

    // Orienter vers le rayon entrant
    if (ray._direction.dot(normal) > 0) normal = -normal;

    rec.normal = normal;

    return true;
}

void RayTracer::Triangle::init(const libconfig::Setting& setting)
{
    ConfigUtils::parsePoint3D(setting, "v0", _v0, true);
    ConfigUtils::parsePoint3D(setting, "v1", _v1, true);
    ConfigUtils::parsePoint3D(setting, "v2", _v2, true);

    Math::Vector3D<double> translation;
    ConfigUtils::parseVector3D(setting, "translation", translation, false);

    Math::Vector3D<double> rotation;
    ConfigUtils::parseVector3D(setting, "rotation", rotation, false);

    auto applyTransform = [&](Point3D& p) {
        Vector3D vec(p._x, p._y, p._z);
        vec.rotateX(rotation._x);
        vec.rotateY(rotation._y);
        vec.rotateZ(rotation._z);
        p = Point3D(vec._x, vec._y, vec._z) + translation;
    };

    applyTransform(_v0);
    applyTransform(_v1);
    applyTransform(_v2);

    if ((_v0._x == _v1._x && _v0._y == _v1._y && _v0._z == _v1._z) ||
        (_v0._x == _v2._x && _v0._y == _v2._y && _v0._z == _v2._z) ||
        (_v1._x == _v2._x && _v1._y == _v2._y && _v1._z == _v2._z)) {
        throw RayTracer::RayTracerException("Triangle: Vertices cannot be the same point.");
    }

    Vector3D edge1 = _v1 - _v0;
    Vector3D edge2 = _v2 - _v0;
    Vector3D normal = edge1.cross(edge2);

    if (normal._x == 0.0 && normal._y == 0.0 && normal._z == 0.0)
        throw RayTracer::RayTracerException("Triangle: Vertices cannot be collinear.");

    if (setting.exists("material")) {
        std::string name = setting["material"];
        _materialName = name;
    }
}

extern "C" {
RayTracer::IPrimitive* entryPoint() { return new RayTracer::Triangle(); }
}
