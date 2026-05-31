/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PrimitiveGroup
*/

#include "PrimitiveGroup.hpp"

void RayTracer::PrimitiveGroup::init(const libconfig::Setting& /*setting*/) { return; }

void RayTracer::PrimitiveGroup::addPrimitive(std::unique_ptr<IPrimitive> primitive)
{
    _children.push_back(std::move(primitive));
}

bool RayTracer::PrimitiveGroup::isGroup() const { return true; }

bool RayTracer::PrimitiveGroup::hits(const Ray& ray, HitRecord& hitRecord) const
{
    bool hitAnything = false;
    HitRecord tempRec;
    double closest = std::numeric_limits<double>::max();

    Ray localRay(Point3D(ray._origin._x - _position._x, ray._origin._y - _position._y,
                         ray._origin._z - _position._z),
                 ray._direction);

    double rx = -_rotation._x * M_PI / 180.0;
    double ry = -_rotation._y * M_PI / 180.0;
    double rz = -_rotation._z * M_PI / 180.0;

    auto rotateX = [](Point3D p, double a) -> Point3D {
        return {p._x, p._y * cos(a) - p._z * sin(a), p._y * sin(a) + p._z * cos(a)};
    };
    auto rotateY = [](Point3D p, double a) -> Point3D {
        return {p._x * cos(a) + p._z * sin(a), p._y, -p._x * sin(a) + p._z * cos(a)};
    };
    auto rotateZ = [](Point3D p, double a) -> Point3D {
        return {p._x * cos(a) - p._y * sin(a), p._x * sin(a) + p._y * cos(a), p._z};
    };

    Point3D ro = localRay._origin;
    ro = rotateX(ro, rx);
    ro = rotateY(ro, ry);
    ro = rotateZ(ro, rz);

    Point3D rd = {localRay._direction._x, localRay._direction._y, localRay._direction._z};
    rd = rotateX(rd, rx);
    rd = rotateY(rd, ry);
    rd = rotateZ(rd, rz);

    Ray rotatedRay(ro, Vector3D(rd._x, rd._y, rd._z));

    for (size_t i = 0; i < _children.size(); i++) {
        const Ray& usedRay = (i == 0) ? ray : rotatedRay;
        if (_children[i]->hits(usedRay, tempRec)) {
            if (tempRec.distance < closest && tempRec.distance > 0.001) {
                closest = tempRec.distance;
                hitRecord = tempRec;

                if (i != 0) {
                    double fx = _rotation._x * M_PI / 180.0;
                    double fy = _rotation._y * M_PI / 180.0;
                    double fz = _rotation._z * M_PI / 180.0;
                    Point3D n = {hitRecord.normal._x, hitRecord.normal._y, hitRecord.normal._z};
                    n = rotateX(n, fx);
                    n = rotateY(n, fy);
                    n = rotateZ(n, fz);
                    hitRecord.normal = Vector3D(n._x, n._y, n._z);

                    hitRecord.p._x += _position._x;
                    hitRecord.p._y += _position._y;
                    hitRecord.p._z += _position._z;
                }
                hitAnything = true;
            }
        }
    }

    return hitAnything;
}
