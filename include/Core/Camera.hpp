/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Camera
*/

/// @file Camera.hpp
/// @brief Camera class definition

#pragma once

#include <cmath>
#include <libconfig.h++>

#include "Point3D.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    /// @brief Represents a camera in 3D space for ray tracing
    class Camera {
        public:
        /// @brief Default constructor
        Camera() = default;

        /// @brief Construct a Camera object
        /// @param position Position of the camera in 3D space
        /// @param rotation Rotation of the camera in degrees
        /// @param fieldOfView Field of view of the camera in degrees
        Camera(const Math::Point3D<double>& position, const Math::Vector3D<double>& rotation,
               double fieldOfView)
            : _position(position), _rotation(rotation), _fieldOfView(fieldOfView)
        {
        }

        /// @brief Generates a ray from the camera through a point on the screen
        /// @param u The horizontal coordinate on the screen (0 to 1)
        /// @param v The vertical coordinate on the screen (0 to 1)
        /// @param ratio The aspect ratio of the screen (width / height)
        /// @return A Ray object representing the ray from the camera through the specified point on
        /// the screen
        [[nodiscard]] inline Ray ray(double u, double v, double ratio) const
        {
            double scale = std::tan(_fieldOfView * 0.5 * M_PI / 180.0);

            double px = (2.0 * u - 1.0) * ratio * scale;
            double py = (2.0 * v - 1.0) * scale;

            Math::Vector3D<double> direction(px, py, 1.0);
            direction.rotateX(_rotation._x);
            direction.rotateY(_rotation._y);
            direction.rotateZ(_rotation._z);
            direction = direction.normalized();

            return Ray(_position, direction);
        }

        /// @brief Initializes the camera with settings from a configuration file
        /// @param setting The configuration settings for the camera
        void init(const libconfig::Setting& setting);

        private:
        /// @brief Position of the camera in 3D space
        Math::Point3D<double> _position;
        /// @brief Rotation of the camera in degrees
        Math::Vector3D<double> _rotation;
        /// @brief Field of view of the camera in degrees
        double _fieldOfView;
    };
}  // namespace RayTracer
