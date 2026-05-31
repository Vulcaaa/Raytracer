/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Obj
*/

/// @file Obj.hpp
/// @brief Obj class definition for parsing .OBJ 3D models

#pragma once

#include <string>
#include <vector>

#include "APrimitive.hpp"
#include "IMaterial.hpp"
#include "Point3D.hpp"

namespace RayTracer {
    /// @brief Lightweight triangle structure for internal OBJ use
    struct ObjTriangle {
        /// @brief Vertices of the triangle
        Math::Point3D<double> _v0;
        /// @brief Vertices of the triangle
        Math::Point3D<double> _v1;
        /// @brief Vertices of the triangle
        Math::Point3D<double> _v2;
        /// @brief Material of the triangle
        IMaterial* _material;

        /// @brief Determines if a ray intersects the triangle and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the triangle
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the triangle
        /// @return True if the ray intersects the triangle, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const;
        /// @brief Computes the centroid of the triangle, which can be used for BVH construction
        /// @return The centroid point of the triangle
        [[nodiscard]] Math::Point3D<double> getCentroid() const;
    };

    /// @brief Node for the Bounding Volume Hierarchy
    struct BVHNode {
        /// @brief Minimum corner of the bounding box
        Math::Point3D<double> _boundsMin;
        /// @brief Maximum corner of the bounding box
        Math::Point3D<double> _boundsMax;
        /// @brief Index of the left child node in the BVH nodes array (-1 if leaf)
        int _leftNode = -1;
        /// @brief Index of the right child node in the BVH nodes array (-1 if leaf)
        int _rightNode = -1;
        /// @brief Index of the first triangle in the triangles array (valid if leaf)
        int _firstTriangleIdx = 0;
        /// @brief Number of triangles in this node (valid if leaf)
        int _triangleCount = 0;

        /// @brief Determines if this node is a leaf node (contains triangles) or an internal node
        /// (contains child nodes)
        /// @return True if this node is a leaf node, false if it is an internal node
        [[nodiscard]] bool isLeaf() const { return _triangleCount > 0; }
        /// @brief Computes the intersection of a ray with the bounding box of this BVH node
        /// @param ray The ray to test for intersection with the bounding box
        /// @return The distance from the ray origin to the intersection point with the bounding
        /// box, or a negative value if there is no intersection
        [[nodiscard]] double hitBounds(const Ray& ray) const;
    };

    /// @brief Represents a 3D model loaded from an .OBJ file
    class Obj : public APrimitive {
        public:
        Obj() = default;

        /// @brief Determines if a ray intersects the model by testing its BVH
        /// @param ray The ray to test for intersection
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// @return True if the ray intersects the model, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the model with settings from a configuration file (loads the .obj
        /// file)
        /// @param setting The configuration settings
        void init(const libconfig::Setting& setting) override;

        private:
        /// @brief Triangles that make up the model, parsed from the .OBJ file
        std::vector<ObjTriangle> _triangles;

        /// @brief BVH nodes array
        std::vector<BVHNode> _bvhNodes;
        int _rootNodeIdx = 0;
        int _nodesUsed = 0;

        /// @brief Builds the Bounding Volume Hierarchy
        void buildBVH();

        /// @brief Updates the bounding box for a BVH node based on its triangles
        void updateNodeBounds(int nodeIdx);

        /// @brief Subdivides a BVH node recursively
        void subdivide(int nodeIdx);

        /// @brief Recursive internal method to traverse the BVH and find intersections
        bool hitBVHNode(int nodeIdx, const Ray& ray, HitRecord& hitRecord,
                        double& closestSoFar) const;

        /// @brief Loads an .obj file and parses its vertices and faces into triangles
        /// @param filepath The path to the .obj file
        /// @param scale An optional scaling factor
        /// @param translation An optional translation vector
        /// @param rotation An optional rotation vector (Euler angles in degrees)
        void loadObjFile(const std::string& filepath, double scale,
                         const Math::Vector3D<double>& translation,
                         const Math::Vector3D<double>& rotation);

        /// @brief Parses a vertex from an input stream and applies transformations
        /// @param iss The input stream containing the vertex data
        /// @param vertices The list of vertices parsed so far (used for indexing faces)
        /// @param scale The scaling factor to apply to the vertex
        /// @param translation The translation vector to translate the vertex
        /// @param rotation The rotation vector (Euler angles in degrees) to rotate the vertex
        void parseVertex(std::istringstream& iss, std::vector<Math::Point3D<double>>& vertices,
                         double scale, const Math::Vector3D<double>& translation,
                         const Math::Vector3D<double>& rotation);

        /// @brief Parses a face from an input stream
        void parseFace(std::istringstream& iss, const std::vector<Math::Point3D<double>>& vertices);

        /// @brief Helper to parse vertex index with optional texture/normal coordinates
        int parseVertexIndex(const std::string& s) const;

        /// @brief Helper to get a vertex by index, handling negative indices
        Math::Point3D<double> getVertex(int idx,
                                        const std::vector<Math::Point3D<double>>& vertices) const;
    };
}  // namespace RayTracer
