/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Plane
*/

/// @file Plane.hpp
/// @brief Plane class definition

#pragma once

#include <string>

#include "APrimitive.hpp"
#include "Point3D.hpp"

struct QuadraticCoeffs {
    double a, b, c;
};
struct CubicCoeffs {
    double a, b, c, d;
};
struct QuarticCoeffs {
    double a, b, c, d, e;
};

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    /// @brief Represents a torus in 3D space
    class Torus : public APrimitive {
        public:
        /// @brief Constructs a torus object
        Torus() = default;

        /// @brief Position of the torus in 3D space
        Point3D _position;

        /// @brief Rotation of the torus in 3D space
        Math::Vector3D<double> _rotation;

        /// @brief Outer (length between center and center tube) radius of the torus
        double _outer_radius;

        /// @brief Inner (radius of the tube) radius of the torus
        double _inner_radius;

        /// @brief Résout une équation quadratique : ax^2 + bx + c = 0.
        /// @param coeffs Structure contenant les coefficients a, b et c.
        /// @param roots Tableau pour stocker les racines réelles trouvées.
        /// @return Nombre de racines réelles trouvées (0, 1 ou 2).
        int solveQuadratic(const QuadraticCoeffs& coeffs, double roots[2]) const;

        /// @brief Résout une équation cubique : ax^3 + bx^2 + cx + d = 0.
        /// @param coeffs Structure contenant les coefficients a, b, c et d.
        /// @param roots Tableau pour stocker les racines réelles trouvées.
        /// @return Nombre de racines réelles trouvées (1, 2 ou 3).
        int solveCubic(const CubicCoeffs& coeffs, double roots[3]) const;

        /// @brief Résout une équation quartique : ax^4 + bx^3 + cx^2 + dx + e = 0.
        /// @param coeffs Structure contenant les coefficients a, b, c, d et e.
        /// @param roots Tableau pour stocker les racines réelles trouvées.
        /// @return Nombre de racines réelles trouvées (0 à 4).
        int solveQuartic(const QuarticCoeffs& coeffs, double roots[4]) const;

        /// @brief Determines if a ray intersects the torus and fills the hit record with
        /// intersection details
        /// @param ray The ray to test for intersection with the torus
        /// @param hitRecord The hit record to be filled with intersection details if the ray hits
        /// the torus
        /// @return True if the ray intersects the torus, false otherwise
        [[nodiscard]] bool hits(const Ray& ray, HitRecord& hitRecord) const override;

        /// @brief Initializes the torus with settings from a configuration file
        /// @param setting The configuration settings for the torus
        void init(const libconfig::Setting& setting) override;
    };
}  // namespace RayTracer
