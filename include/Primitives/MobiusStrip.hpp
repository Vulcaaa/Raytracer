/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** MöbiusStrip
*/

/// @file MobiusStrip.hpp
/// @brief MobiusStrip class definition

#pragma once

#include "AFractal.hpp"

namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    class MobiusStrip : public AFractal {
        public:
        MobiusStrip() = default;

        void init(const libconfig::Setting& setting) override;

        [[nodiscard]] double distanceEstimate(const Math::Vector3D<double>& p) const override;

        private:
        double _radius;
        double _width;
        double _thickness;
        double _halfTwist;
    };
}  // namespace RayTracer