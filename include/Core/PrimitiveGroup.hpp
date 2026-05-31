/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PrimitiveGroup
*/

/// @file PrimitiveGroup.hpp
/// @brief PrimitiveGroup class definition

#pragma once

#include "APrimitive.hpp"
#include "Point3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a PrimitiveGroup, which is a collection of primitives
    /// that can be treated as a single entity
    class PrimitiveGroup : public APrimitive {
        public:
        PrimitiveGroup() = default;

        /// @brief Position of the group in 3D space.
        Point3D _position;

        /// @brief Rotation of the group in 3D space.
        Vector3D _rotation;

        /// @brief Determines if a ray intersects the group and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the group
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the group
        /// @return True if the ray intersects the group, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the group with settings from a configuration file
        /// @param setting The configuration settings for the group
        void init(const libconfig::Setting& setting) override;

        void addPrimitive(std::unique_ptr<IPrimitive> primitive);

        bool isGroup() const override;

        std::vector<std::unique_ptr<IPrimitive>>& getChildren() { return _children; }

        private:
        /// @brief List of primitives that belong to this group
        std::vector<std::unique_ptr<IPrimitive>> _children;
    };
}  // namespace RayTracer
