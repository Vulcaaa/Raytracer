/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Box.cpp
*/

#include "Box.hpp"

#include <cmath>
#include <memory>

#include "ConfigUtils.hpp"
#include "RayTracerException.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {

    static std::pair<double, double> uv_from_point(Vector3D point)
    {
        double ax = std::abs(point._x);
        double ay = std::abs(point._y);
        double az = std::abs(point._z);

        if (ax >= ay && ax >= az) {
            if (point._x > 0)
                return {(1 + point._z) / 2, (point._y + 1) / 2};
            else
                return {(1 - point._z) / 2, (point._y + 1) / 2};
        }
        if (ay >= ax && ay >= az) {
            if (point._y > 0)
                return {(1 + point._x) / 2, (point._z + 1) / 2};
            else
                return {(1 + point._x) / 2, (1 - point._z) / 2};
        }
        if (az >= ax && az >= ay) {
            if (point._z > 0) return {(1 + point._x) / 2, (point._y + 1) / 2};
        }
        return {(1 - point._x) / 2, (point._y + 1) / 2};
    }

    bool Box::hits(const Ray& ray, HitRecord& rec) const
    {
        rec.primitive = this;
        rec.material = _material.get();

        Math::Point3D<double> center = {(_min._x + _max._x) * 0.5, (_min._y + _max._y) * 0.5,
                                        (_min._z + _max._z) * 0.5};

        Math::Vector3D<double> localOrigin = {
            ray._origin._x - center._x, ray._origin._y - center._y, ray._origin._z - center._z};
        Math::Vector3D<double> localDir = ray._direction;

        localOrigin.rotateZ(-_rotation._z);
        localOrigin.rotateY(-_rotation._y);
        localOrigin.rotateX(-_rotation._x);

        localDir.rotateZ(-_rotation._z);
        localDir.rotateY(-_rotation._y);
        localDir.rotateX(-_rotation._x);

        Math::Point3D<double> localMin = {_min._x - center._x, _min._y - center._y,
                                          _min._z - center._z};
        Math::Point3D<double> localMax = {_max._x - center._x, _max._y - center._y,
                                          _max._z - center._z};

        double txmin = (localMin._x - localOrigin._x) / localDir._x;
        double txmax = (localMax._x - localOrigin._x) / localDir._x;
        double tymin = (localMin._y - localOrigin._y) / localDir._y;
        double tymax = (localMax._y - localOrigin._y) / localDir._y;
        double tzmin = (localMin._z - localOrigin._z) / localDir._z;
        double tzmax = (localMax._z - localOrigin._z) / localDir._z;

        double txEntry = std::fmin(txmin, txmax);
        double tyEntry = std::fmin(tymin, tymax);
        double tzEntry = std::fmin(tzmin, tzmax);

        int face = 0;
        double tmin = txEntry;
        if (tyEntry > tmin) {
            tmin = tyEntry;
            face = 1;
        }
        if (tzEntry > tmin) {
            tmin = tzEntry;
            face = 2;
        }

        double tmax = std::fmin(std::fmin(std::fmax(txmin, txmax), std::fmax(tymin, tymax)),
                                std::fmax(tzmin, tzmax));

        if (tmax <= tmin) return false;

        rec.distance = (tmin >= 0) ? tmin : tmax;
        if (rec.distance < 0) return false;

        Math::Vector3D<double> localNormal;
        if (face == 0)
            localNormal = (localDir._x < 0) ? Math::Vector3D<double>{1, 0, 0}
                                            : Math::Vector3D<double>{-1, 0, 0};
        else if (face == 1)
            localNormal = (localDir._y < 0) ? Math::Vector3D<double>{0, 1, 0}
                                            : Math::Vector3D<double>{0, -1, 0};
        else
            localNormal = (localDir._z < 0) ? Math::Vector3D<double>{0, 0, 1}
                                            : Math::Vector3D<double>{0, 0, -1};

        localNormal.rotateX(_rotation._x);
        localNormal.rotateY(_rotation._y);
        localNormal.rotateZ(_rotation._z);

        rec.normal = localNormal;

        // UV Mapping
        Vector3D localHitPoint = localOrigin + localDir * rec.distance;
        Vector3D p = localHitPoint;

        p._x = (p._x - localMin._x) / (localMax._x - localMin._x) * 2 - 1;
        p._y = (p._y - localMin._y) / (localMax._y - localMin._y) * 2 - 1;
        p._z = (p._z - localMin._z) / (localMax._z - localMin._z) * 2 - 1;

        std::pair<double, double> uv = uv_from_point(p);
        rec.u = uv.first;
        rec.v = uv.second;

        rec.p = ray._origin + ray._direction * rec.distance;
        return true;
    }

    void Box::init(const libconfig::Setting& setting)
    {
        ConfigUtils::parsePoint3D(setting, "position", _min, true);

        ConfigUtils::parseVector3D(setting, "rotation", _rotation, false);

        ConfigUtils::parsePoint3D(setting, "dimension", _max, true);
        if (_max._x < 0 || _max._y < 0 || _max._z < 0)
            throw RayTracerException("Box's dimension cannot be negatives.");

        _max = {_max._x + _min._x, _max._y + _min._y, _max._z + _min._z};

        if (setting.exists("material")) {
            std::string name = setting["material"];
            _materialName = name;
        }
    }

    extern "C" {
    IPrimitive* entryPoint() { return new Box(); }
    }
}  // namespace RayTracer
