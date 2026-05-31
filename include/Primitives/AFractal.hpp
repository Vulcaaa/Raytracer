/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Fractal
*/

/// @file AFractal.hpp
/// @brief AFractal class definition

#pragma once

#include "APrimitive.hpp"

namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    class AFractal : public APrimitive {
        public:
        AFractal() = default;

        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Estimates the distance from a point in space to the fractal surface,
        ///     used by the ray marching algorithm to determine step size
        /// @param p The point in the fractal's local space to evaluate
        /// @return The estimated minimum distance from p to the fractal surface
        [[nodiscard]] virtual double distanceEstimate(const Math::Vector3D<double>& p) const = 0;

        protected:
        /// @brief Computes the surface normal at a point on the fractal using numerical
        ///     gradient estimation (central differences on the distance estimator)
        /// @param p The point on the fractal surface in local space
        /// @return The normalized surface normal vector at p
        [[nodiscard]] Math::Vector3D<double> computeNormal(const Math::Vector3D<double>& p) const;

        Math::Vector3D<double> rotateInverse(const Math::Vector3D<double>& v) const;

        Math::Vector3D<double> rotate(const Math::Vector3D<double>& v) const;

        /// @brief Position of the fractal's center in a 3D space
        Point3D _position;

        /// @brief Rotation of the fractal in a 3D space
        Point3D _rotation;

        /// @brief Uniform scale factor applied to the fractal geometry
        double _scale;

        /// @brief Maximum number of iterations for the distance estimator loop
        double _iterations;
    };
}  // namespace RayTracer