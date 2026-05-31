/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Obj
*/

/// @file Obj.cpp
/// @brief Implementation of the Obj class.

#include "Obj.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>
#include <numbers>
#include <sstream>

#include "ConfigUtils.hpp"
#include "RayTracerException.hpp"

bool RayTracer::ObjTriangle::hits(const Ray& ray, HitRecord& rec) const
{
    const double epsilon = 1e-6;

    // 1. Define the triangle's edges
    Math::Vector3D<double> edge1 = _v1 - _v0;
    Math::Vector3D<double> edge2 = _v2 - _v0;

    // 2. Calculate determinant to check if ray is parallel to the triangle
    Math::Vector3D<double> pvec = ray._direction.cross(edge2);

    double det = edge1.dot(pvec);
    // If determinant is close to 0, ray is parallel to the triangle
    if (std::abs(det) < epsilon) return false;

    double invDet = 1.0 / det;

    // 3. Calculate U parameter (barycentric coordinate) and test bounds
    Math::Vector3D<double> tvec = ray._origin - _v0;
    double u = tvec.dot(pvec) * invDet;

    // If U is outside [0, 1], the hit is outside the triangle
    if (u < 0.0 || u > 1.0) return false;

    // 4. Calculate V parameter and test bounds
    Math::Vector3D<double> qvec = tvec.cross(edge1);
    double v = ray._direction.dot(qvec) * invDet;

    // If V is outside or U+V > 1, the hit is outside the triangle
    if (v < 0.0 || u + v > 1.0) return false;

    // 5. Calculate T (distance to intersection point)
    double t = edge2.dot(qvec) * invDet;

    // If T is negative, the triangle is behind the camera
    if (t <= epsilon) return false;

    // 6. Valid hit -> Save the hit info
    rec.distance = t;
    rec.p = ray._origin + (ray._direction * t);
    rec.u = u;
    rec.v = v;

    // Calculate the face normal using cross product of edges
    Math::Vector3D<double> normal = edge1.cross(edge2);
    rec.normal = normal.normalized();

    return true;
}

Math::Point3D<double> RayTracer::ObjTriangle::getCentroid() const
{
    return {(_v0._x + _v1._x + _v2._x) / 3.0, (_v0._y + _v1._y + _v2._y) / 3.0,
            (_v0._z + _v1._z + _v2._z) / 3.0};
}

double RayTracer::BVHNode::hitBounds(const Ray& ray) const
{
    double invDx = 1.0 / ray._direction._x;
    double invDy = 1.0 / ray._direction._y;
    double invDz = 1.0 / ray._direction._z;

    double t0x = (_boundsMin._x - ray._origin._x) * invDx;
    double t1x = (_boundsMax._x - ray._origin._x) * invDx;
    if (invDx < 0.0) std::swap(t0x, t1x);

    double t0y = (_boundsMin._y - ray._origin._y) * invDy;
    double t1y = (_boundsMax._y - ray._origin._y) * invDy;
    if (invDy < 0.0) std::swap(t0y, t1y);

    if (t0x > t1y || t0y > t1x) return std::numeric_limits<double>::max();
    if (t0y > t0x) t0x = t0y;
    if (t1y < t1x) t1x = t1y;

    double t0z = (_boundsMin._z - ray._origin._z) * invDz;
    double t1z = (_boundsMax._z - ray._origin._z) * invDz;
    if (invDz < 0.0) std::swap(t0z, t1z);

    if (t0x > t1z || t0z > t1x) return std::numeric_limits<double>::max();
    if (t0z > t0x) t0x = t0z;
    if (t1z < t1x) t1x = t1z;

    if (t1x < 0.0) return std::numeric_limits<double>::max();
    return t0x > 0.0 ? t0x : t1x;
}

void RayTracer::Obj::updateNodeBounds(int nodeIdx)
{
    BVHNode& node = _bvhNodes[nodeIdx];
    node._boundsMin = {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max()};
    node._boundsMax = {std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(),
                       std::numeric_limits<double>::lowest()};

    for (int i = 0; i < node._triangleCount; ++i) {
        const ObjTriangle& leafTri = _triangles[node._firstTriangleIdx + i];

        node._boundsMin._x =
            std::min({node._boundsMin._x, leafTri._v0._x, leafTri._v1._x, leafTri._v2._x});
        node._boundsMin._y =
            std::min({node._boundsMin._y, leafTri._v0._y, leafTri._v1._y, leafTri._v2._y});
        node._boundsMin._z =
            std::min({node._boundsMin._z, leafTri._v0._z, leafTri._v1._z, leafTri._v2._z});

        node._boundsMax._x =
            std::max({node._boundsMax._x, leafTri._v0._x, leafTri._v1._x, leafTri._v2._x});
        node._boundsMax._y =
            std::max({node._boundsMax._y, leafTri._v0._y, leafTri._v1._y, leafTri._v2._y});
        node._boundsMax._z =
            std::max({node._boundsMax._z, leafTri._v0._z, leafTri._v1._z, leafTri._v2._z});
    }
}

void RayTracer::Obj::subdivide(int nodeIdx)
{
    BVHNode& node = _bvhNodes[nodeIdx];
    if (node._triangleCount <= 2) return;

    Math::Vector3D<double> extent = {node._boundsMax._x - node._boundsMin._x,
                                     node._boundsMax._y - node._boundsMin._y,
                                     node._boundsMax._z - node._boundsMin._z};

    int axis = 0;
    if (extent._y > extent._x) axis = 1;
    if (extent._z > extent._x && extent._z > extent._y) axis = 2;

    double splitPos = 0;
    if (axis == 0)
        splitPos = node._boundsMin._x + extent._x * 0.5;
    else if (axis == 1)
        splitPos = node._boundsMin._y + extent._y * 0.5;
    else
        splitPos = node._boundsMin._z + extent._z * 0.5;

    int i = node._firstTriangleIdx;
    int j = i + node._triangleCount - 1;
    while (i <= j) {
        Math::Point3D<double> centroid = _triangles[i].getCentroid();
        double centroidAxis = (axis == 0) ? centroid._x : ((axis == 1) ? centroid._y : centroid._z);

        if (centroidAxis < splitPos)
            i++;
        else
            std::swap(_triangles[i], _triangles[j--]);
    }

    int leftCount = i - node._firstTriangleIdx;
    if (leftCount == 0 || leftCount == node._triangleCount) return;

    int leftChildIdx = _nodesUsed++;
    int rightChildIdx = _nodesUsed++;

    _bvhNodes[leftChildIdx]._firstTriangleIdx = node._firstTriangleIdx;
    _bvhNodes[leftChildIdx]._triangleCount = leftCount;
    _bvhNodes[rightChildIdx]._firstTriangleIdx = i;
    _bvhNodes[rightChildIdx]._triangleCount = node._triangleCount - leftCount;

    node._leftNode = leftChildIdx;
    node._rightNode = rightChildIdx;
    node._triangleCount = 0;

    updateNodeBounds(leftChildIdx);
    updateNodeBounds(rightChildIdx);

    subdivide(leftChildIdx);
    subdivide(rightChildIdx);
}

void RayTracer::Obj::buildBVH()
{
    if (_triangles.empty()) return;

    _bvhNodes.resize(_triangles.size() * 2 - 1);
    _rootNodeIdx = 0;
    _nodesUsed = 1;

    _bvhNodes[_rootNodeIdx]._firstTriangleIdx = 0;
    _bvhNodes[_rootNodeIdx]._triangleCount = _triangles.size();

    updateNodeBounds(_rootNodeIdx);
    subdivide(_rootNodeIdx);
}

bool RayTracer::Obj::hitBVHNode(int nodeIdx, const Ray& ray, HitRecord& hitRecord,
                                double& closestSoFar) const
{
    const BVHNode& node = _bvhNodes[nodeIdx];

    if (node.isLeaf()) {
        bool hitAnything = false;
        HitRecord tempRec;
        for (int i = 0; i < node._triangleCount; ++i) {
            if (_triangles[node._firstTriangleIdx + i].hits(ray, tempRec)) {
                if (tempRec.distance < closestSoFar) {
                    closestSoFar = tempRec.distance;
                    hitRecord = tempRec;
                    hitAnything = true;
                }
            }
        }
        return hitAnything;
    }

    double tLeft = _bvhNodes[node._leftNode].hitBounds(ray);
    double tRight = _bvhNodes[node._rightNode].hitBounds(ray);

    int first = node._leftNode;
    int second = node._rightNode;

    if (tLeft > tRight) {
        std::swap(tLeft, tRight);
        std::swap(first, second);
    }

    bool hitAnything = false;
    if (tLeft < closestSoFar) hitAnything |= hitBVHNode(first, ray, hitRecord, closestSoFar);
    if (tRight < closestSoFar) hitAnything |= hitBVHNode(second, ray, hitRecord, closestSoFar);

    return hitAnything;
}

bool RayTracer::Obj::hits(const Ray& ray, HitRecord& hitRecord) const
{
    if (_triangles.empty() || _bvhNodes.empty()) return false;

    double closestSoFar = std::numeric_limits<double>::max();

    if (_bvhNodes[_rootNodeIdx].hitBounds(ray) >= closestSoFar) {
        return false;
    }

    bool hitAnything = hitBVHNode(_rootNodeIdx, ray, hitRecord, closestSoFar);

    if (hitAnything) {
        hitRecord.normal = hitRecord.normal.normalized();
        hitRecord.material = _material.get();
        hitRecord.primitive = this;
    }

    return hitAnything;
}

int RayTracer::Obj::parseVertexIndex(const std::string& s) const
{
    size_t pos = s.find('/');
    if (pos != std::string::npos) return std::stoi(s.substr(0, pos));
    return std::stoi(s);
}

Math::Point3D<double> RayTracer::Obj::getVertex(
    int idx, const std::vector<Math::Point3D<double>>& vertices) const
{
    if (idx > 0) return vertices[idx - 1];
    if (idx < 0) return vertices[vertices.size() + idx];
    throw RayTracerException("Obj: Invalid vertex index 0");
}

void RayTracer::Obj::parseVertex(std::istringstream& iss,
                                 std::vector<Math::Point3D<double>>& vertices, double scale,
                                 const Math::Vector3D<double>& position,
                                 const Math::Vector3D<double>& rotation)
{
    double x, y, z;
    iss >> x >> y >> z;

    // Scale
    x *= scale;
    y *= scale;
    z *= scale;

    // Convert rotation to radians
    double pi = std::numbers::pi;
    double rotX = rotation._x * (pi / 180.0);
    double rotY = rotation._y * (pi / 180.0);
    double rotZ = rotation._z * (pi / 180.0);

    // Rotate around X axis
    double tempY = y * std::cos(rotX) - z * std::sin(rotX);
    double tempZ = y * std::sin(rotX) + z * std::cos(rotX);
    y = tempY;
    z = tempZ;

    // Rotate around Y axis
    double tempX = x * std::cos(rotY) + z * std::sin(rotY);
    tempZ = -x * std::sin(rotY) + z * std::cos(rotY);
    x = tempX;
    z = tempZ;

    // Rotate around Z axis
    tempX = x * std::cos(rotZ) - y * std::sin(rotZ);
    tempY = x * std::sin(rotZ) + y * std::cos(rotZ);
    x = tempX;
    y = tempY;

    // Translate
    x += position._x;
    y += position._y;
    z += position._z;

    vertices.emplace_back(x, y, z);
}

void RayTracer::Obj::parseFace(std::istringstream& iss,
                               const std::vector<Math::Point3D<double>>& vertices)
{
    std::string v1_str, v2_str, v3_str;
    if (!(iss >> v1_str >> v2_str >> v3_str)) return;

    int i1 = parseVertexIndex(v1_str);
    int i2 = parseVertexIndex(v2_str);
    int i3 = parseVertexIndex(v3_str);

    try {
        ObjTriangle t;
        t._v0 = getVertex(i1, vertices);
        t._v1 = getVertex(i2, vertices);
        t._v2 = getVertex(i3, vertices);
        t._material = _material.get();
        _triangles.push_back(t);

        // Handle quads
        std::string v4_str;
        if (iss >> v4_str) {
            int i4 = parseVertexIndex(v4_str);
            ObjTriangle t2;
            t2._v0 = getVertex(i1, vertices);
            t2._v1 = getVertex(i3, vertices);
            t2._v2 = getVertex(i4, vertices);
            t2._material = _material.get();
            _triangles.push_back(t2);
        }
    } catch (const RayTracerException&) {
        // Ignore degenerate triangles
    }
}

void RayTracer::Obj::loadObjFile(const std::string& filepath, double scale,
                                 const Math::Vector3D<double>& position,
                                 const Math::Vector3D<double>& rotation)
{
    std::ifstream file(filepath);
    if (!file.is_open()) throw RayTracerException("Obj: Could not open file " + filepath);

    std::vector<Math::Point3D<double>> vertices;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
            parseVertex(iss, vertices, scale, position, rotation);
        else if (type == "f")
            parseFace(iss, vertices);
    }

    buildBVH();
}

void RayTracer::Obj::init(const libconfig::Setting& setting)
{
    if (setting.exists("material")) {
        std::string name = setting["material"];
        _materialName = name;
    }

    double scale = 1.0;
    ConfigUtils::getAsDouble(setting, "scale", scale);

    Math::Vector3D<double> translation;
    ConfigUtils::parseVector3D(setting, "translation", translation, false);

    Math::Vector3D<double> rotation;
    ConfigUtils::parseVector3D(setting, "rotation", rotation, false);

    if (setting.exists("file")) {
        std::string filepath;
        setting.lookupValue("file", filepath);
        loadObjFile(filepath, scale, translation, rotation);
    } else
        throw RayTracerException("Obj: Missing required parameter 'file'.");
}

extern "C" {
RayTracer::IPrimitive* entryPoint() { return new RayTracer::Obj(); }
}
