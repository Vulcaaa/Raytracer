/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** TangleCube
*/

/// @file TangleCube.hpp
/// @brief TangleCube class definition

#pragma once

#include "AFractal.hpp"

namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    class TangleCube : public AFractal {
        public:
        TangleCube() = default;

        void init(const libconfig::Setting& setting) override;

        [[nodiscard]] double distanceEstimate(const Math::Vector3D<double>& p) const override;

        private:
        double _constant;
    };
}  // namespace RayTracer