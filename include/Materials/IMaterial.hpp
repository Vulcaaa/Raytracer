/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** IMaterial
*/

#pragma once

#include <libconfig.h++>
#include <memory>
#include <vector>

#include "Ray.hpp"
#include "Vector3D.hpp"

namespace RayTracer {
    struct HitRecord;
    class ILight;
    class IPrimitive;

    class IMaterial {
        public:
        virtual ~IMaterial() = default;

        /// @brief Initializes the material with settings from a configuration file
        /// @param setting The configuration settings for the given material
        virtual void init(const libconfig::Setting& setting) = 0;

        /// @brief computes the scattered ray and attenuation for a given ray and hit record
        /// @param rayIn Incoming ray that hits the material
        /// @param rec Hit record for the intersection
        /// @param attenuation Output parameter for the attenuation (color reduction) of the
        /// scattered ray
        /// @param scattered Output parameter for the scattered ray
        /// @return True if the ray was scattered, false otherwise
        virtual bool scatter(const Ray& rayIn, HitRecord& rec, Math::Vector3D<double>& attenuation,
                             Ray& scattered) const = 0;

        /// @brief returns the name of the material
        /// @return The name of the material
        [[nodiscard]] virtual inline std::string getName() const = 0;

        /// @brief returns the color of the material
        /// @param u The u texture coordinate (default 0.0)
        /// @param v The v texture coordinate (default 0.0)
        /// @return the color of the material
        [[nodiscard]] virtual Math::Vector3D<double> getColor(double u = 0.0,
                                                              double v = 0.0) const = 0;

        /// @brief Set color to the given parameter
        /// @param color color to set
        virtual void setColor(const Math::Vector3D<double>& color) = 0;

        virtual Math::Vector3D<double> computeSpecular(
            const Ray& ray, const HitRecord& rec,
            const std::vector<std::unique_ptr<ILight>>& lights,
            const std::vector<std::unique_ptr<IPrimitive>>& primitives) const = 0;

        virtual bool isRefractive() const = 0;

        virtual double getRefractive() const = 0;

        virtual bool isReflective() const = 0;

        virtual double getReflective() const = 0;

        virtual bool hasSpecular() const = 0;

        virtual double getShininess() const = 0;

        virtual double getSpecularStrength() const = 0;

        protected:
        /// @brief Name of the material
        std::string _name;

        /// @brief Color of the lambertian material
        Math::Vector3D<double> _color;
    };
}  // namespace RayTracer
