/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Sierpinski
*/

/// @file Sierpinski.hpp
/// @brief Sierpinski class definition

#pragma once

#include "AFractal.hpp"

namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    class Sierpinski : public AFractal {
        public:
        Sierpinski() = default;

        void init(const libconfig::Setting& setting) override;

        [[nodiscard]] double distanceEstimate(const Math::Vector3D<double>& p) const override;
    };
}  // namespace RayTracer