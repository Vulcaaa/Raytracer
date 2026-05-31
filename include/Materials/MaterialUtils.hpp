/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** MaterialUtils
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>

#include "Vector3D.hpp"

namespace RayTracer {
    class MaterialUtils {
        public:
        /// @brief Checks if a point is in shadow
        /// @param hit The hit record
        /// @param lightDir The direction to the light
        /// @param primitives The list of primitives in the scene
        /// @return True if the point is in shadow, false otherwise
        static bool isInShadow(const HitRecord& rec, const Math::Vector3D<double>& lightDir,
                               const std::vector<std::unique_ptr<IPrimitive>>& primitives)
        {
            Ray shadowRay(rec.p + rec.normal * 0.001, lightDir);
            HitRecord tempRec;
            for (const auto& prim : primitives) {
                if (prim->hits(shadowRay, tempRec) && tempRec.distance > 0.001) return true;
            }
            return false;
        }

        static double randomDouble()
        {
            thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
            thread_local std::mt19937 generator;
            return distribution(generator);
        }

        static double schlick(double cosine, double ior)
        {
            double r0 = (1 - ior) / (1 + ior);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow(1 - cosine, 5);
        }

        static Math::Vector3D<double> reflect(const Math::Vector3D<double>& v,
                                              const Math::Vector3D<double>& n)
        {
            return v - n * 2.0 * v.dot(n);
        }

        static Math::Vector3D<double> refract(const Math::Vector3D<double>& uv,
                                              const Math::Vector3D<double>& n, double ratio)
        {
            double cosTheta = std::min<double>((-uv).dot(n), 1.0);
            Math::Vector3D<double> rPerp = (uv + n * cosTheta) * ratio;
            Math::Vector3D<double> rParallel = n * -std::sqrt(std::abs(1.0 - rPerp.dot(rPerp)));
            return rPerp + rParallel;
        }

        static Math::Vector3D<double> randomUnitVector()
        {
            double a = randomDouble() * 2.0 * std::numbers::pi;
            double z = randomDouble() * 2.0 - 1.0;
            double r = std::sqrt(1.0 - z * z);
            return {r * std::cos(a), r * std::sin(a), z};
        }

        static Math::Vector3D<double> randomInUnitSphere()
        {
            while (true) {
                Math::Vector3D<double> p(randomDouble() * 2.0 - 1.0, randomDouble() * 2.0 - 1.0,
                                         randomDouble() * 2.0 - 1.0);
                if (p.dot(p) < 1.0) return p;
            }
        }
    };
}  // namespace RayTracer