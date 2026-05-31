/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Manger Sponge
*/

/// @file MangerSponge.hpp
/// @brief Manger Sponge class definition

#pragma once

#include "AFractal.hpp"

namespace RayTracer {
    using Point3D = Math::Point3D<double>;
    using Vector3D = Math::Vector3D<double>;

    class MengerSponge : public AFractal {
        public:
        MengerSponge() = default;

        void init(const libconfig::Setting& setting) override;

        [[nodiscard]] double distanceEstimate(const Math::Vector3D<double>& p) const override;

        private:
        double crossDE(const Math::Vector3D<double>& p) const;

        double boxDE(const Math::Vector3D<double>& p, double size) const;
    };
}  // namespace RayTracer