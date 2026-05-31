/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Point3D
*/

/// @file Point3D.hpp
/// @brief Point3D class definition

#pragma once

#include "Vector3D.hpp"

/// @brief Namespace for the Math library
namespace Math {
    /// @brief Represents a point in 3D space
    /// @tparam T The type of the coordinates (default is double)
    template <typename T = double>
    class Point3D {
        public:
        /// @brief Constructs a Point3D object
        /// @param x The x-coordinate of the point (default is 0)
        /// @param y The y-coordinate of the point (default is 0)
        /// @param z The z-coordinate of the point (default is 0)
        Point3D(T x = T{}, T y = T{}, T z = T{}) : _x(x), _y(y), _z(z) {}

        /// @brief The x-coordinate of the point
        T _x;
        /// @brief The y-coordinate of the point
        T _y;
        /// @brief The z-coordinate of the point
        T _z;

        // Operators

        /// @brief Adds a vector to the point, resulting in a new point
        /// @param other The vector to be added to the point
        /// @return A new Point3D object representing the result of adding the vector to the point
        [[nodiscard]] inline Point3D operator+(const Vector3D<T>& other) const
        {
            return Point3D(_x + other._x, _y + other._y, _z + other._z);
        }

        /// @brief Subtracts a vector from the point, resulting in a new point
        /// @param other The vector to be subtracted from the point
        /// @return A new Point3D object representing the result of subtracting the vector from the
        /// point
        [[nodiscard]] inline Point3D operator-(const Vector3D<T>& other) const
        {
            return Point3D(_x - other._x, _y - other._y, _z - other._z);
        }

        /// @brief Subtracts another point from this point, resulting in a vector
        /// @param other The point to be subtracted from this point
        /// @return A new Vector3D object representing the result of subtracting the other point
        /// from this point
        [[nodiscard]] inline Math::Vector3D<T> operator-(const Point3D& other) const
        {
            return Math::Vector3D<T>(_x - other._x, _y - other._y, _z - other._z);
        }

        /// @brief Computes the dot product of this point and another vector
        /// @param other The other vector
        /// @return The dot product
        [[nodiscard]] inline T dot(const Vector3D<T>& other) const
        {
            return _x * other._x + _y * other._y + _z * other._z;
        }

        /// @brief Translates the point by a given vector, modifying the point's coordinates
        /// @param v The vector by which to translate the point
        inline void translate(const Vector3D<T>& v)
        {
            _x += v._x;
            _y += v._y;
            _z += v._z;
        }
    };
}  // namespace Math
