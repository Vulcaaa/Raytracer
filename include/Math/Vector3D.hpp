/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Vector3D
*/

/// @file Vector3D.hpp
/// @brief Vector3D class definition

#pragma once

#include <cmath>
#include <numbers>

#include "RayTracerException.hpp"

/// @brief Namespace for the Math library
namespace Math {
    /// @brief Represents a vector in 3D space
    /// @tparam T The type of the coordinates (default is double)
    template <typename T = double>
    class Vector3D {
        public:
        /// @brief Constructs a Vector3D object
        /// @param x The x-coordinate of the vector (default is 0)
        /// @param y The y-coordinate of the vector (default is 0)
        /// @param z The z-coordinate of the vector (default is 0)
        Vector3D(T x = T{}, T y = T{}, T z = T{}) : _x(x), _y(y), _z(z) {}

        /// @brief The x-coordinate of the vector
        T _x;
        /// @brief The y-coordinate of the vector
        T _y;
        /// @brief The z-coordinate of the vector
        T _z;

        /// @brief Computes the length (magnitude) of the vector
        /// @return The length of the vector
        [[nodiscard]] inline T length() const { return std::sqrt(_x * _x + _y * _y + _z * _z); }

        /// @brief Computes the dot product of this vector and another vector
        /// @param other The other vector
        /// @return The dot product
        [[nodiscard]] inline T dot(const Vector3D& other) const
        {
            return _x * other._x + _y * other._y + _z * other._z;
        }

        /// @brief Computes the cross product of this vector and another vector
        /// @param other The other vector
        /// @return The cross product
        [[nodiscard]] inline Vector3D cross(const Vector3D& other) const
        {
            return Vector3D(_y * other._z - _z * other._y, _z * other._x - _x * other._z,
                            _x * other._y - _y * other._x);
        }

        [[nodiscard]] inline Vector3D normalized() const
        {
            T len = std::sqrt(_x * _x + _y * _y + _z * _z);
            if (len == 0) return *this;
            return Vector3D(_x / len, _y / len, _z / len);
        }

        /// @brief Rotates the vector around the X axis
        /// @param angle The angle of rotation in radians
        inline void rotateX(double angle)
        {
            double radians = angle * (std::numbers::pi / 180.0);
            double cos_a = std::cos(radians);
            double sin_a = std::sin(radians);
            T old_y = _y;
            _y = old_y * cos_a - _z * sin_a;
            _z = old_y * sin_a + _z * cos_a;
        }

        /// @brief Rotates the vector around the Y axis
        /// @param angle The angle of rotation in radians
        inline void rotateY(double angle)
        {
            double radians = angle * (std::numbers::pi / 180.0);
            double cos_a = std::cos(radians);
            double sin_a = std::sin(radians);
            T old_x = _x;
            _x = old_x * cos_a + _z * sin_a;
            _z = -old_x * sin_a + _z * cos_a;
        }

        /// @brief Rotates the vector around the Z axis
        /// @param angle The angle of rotation in radians
        inline void rotateZ(double angle)
        {
            double radians = angle * (std::numbers::pi / 180.0);
            double cos_a = std::cos(radians);
            double sin_a = std::sin(radians);
            T old_x = _x;
            _x = old_x * cos_a - _y * sin_a;
            _y = old_x * sin_a + _y * cos_a;
        }

        // Operator with vector

        /// @brief Adds two vectors element-wise
        /// @param other The vector to add
        /// @return The resulting vector
        [[nodiscard]] inline Vector3D operator+(const Vector3D& other) const
        {
            return Vector3D(_x + other._x, _y + other._y, _z + other._z);
        }

        /// @brief Subtracts a vector from this vector element-wise
        /// @param other The vector to subtract
        /// @return The resulting vector
        [[nodiscard]] inline Vector3D operator-(const Vector3D& other) const
        {
            return Vector3D(_x - other._x, _y - other._y, _z - other._z);
        }

        [[nodiscard]] inline Math::Vector3D<double> operator-() const { return {-_x, -_y, -_z}; }

        /// @brief Multiplies two vectors element-wise
        /// @param other The vector to multiply
        /// @return The resulting vector
        [[nodiscard]] inline Vector3D operator*(const Vector3D& other) const
        {
            return Vector3D(_x * other._x, _y * other._y, _z * other._z);
        }

        /// @brief Divides this vector by another vector element-wise
        /// @param other The vector to divide by
        /// @return The resulting vector
        [[nodiscard]] inline Vector3D operator/(const Vector3D& other) const
        {
            if (other._x == 0 || other._y == 0 || other._z == 0)
                throw RayTracer::RayTracerException("Vector3D: Division by zero (vector).");
            return Vector3D(_x / other._x, _y / other._y, _z / other._z);
        }

        // Operator ...= with vector

        /// @brief Adds a vector to this vector element-wise
        /// @param other The vector to add
        /// @return Reference to the modified vector
        inline Vector3D& operator+=(const Vector3D& other)
        {
            _x += other._x;
            _y += other._y;
            _z += other._z;
            return *this;
        }

        /// @brief Subtracts a vector from this vector element-wise
        /// @param other The vector to subtract
        /// @return Reference to the modified vector
        inline Vector3D& operator-=(const Vector3D& other)
        {
            _x -= other._x;
            _y -= other._y;
            _z -= other._z;
            return *this;
        }

        /// @brief Multiplies this vector by another vector element-wise
        /// @param other The vector to multiply
        /// @return Reference to the modified vector
        inline Vector3D& operator*=(const Vector3D& other)
        {
            _x *= other._x;
            _y *= other._y;
            _z *= other._z;
            return *this;
        }

        /// @brief Divides this vector by another vector element-wise
        /// @param other The vector to divide by
        /// @return Reference to the modified vector
        inline Vector3D& operator/=(const Vector3D& other)
        {
            if (other._x == 0 || other._y == 0 || other._z == 0)
                throw RayTracer::RayTracerException("Vector3D: Division by zero (vector).");
            _x /= other._x;
            _y /= other._y;
            _z /= other._z;
            return *this;
        }

        // Operator with scalar

        /// @brief Adds a scalar to each element of the vector
        /// @param scalar The scalar to add
        /// @return The resulting vector
        [[nodiscard]] inline Vector3D operator+(T scalar) const
        {
            return Vector3D(_x + scalar, _y + scalar, _z + scalar);
        }

        /// @brief Subtracts a scalar from each element of the vector
        /// @param scalar The scalar to subtract
        /// @return The resulting vector
        [[nodiscard]] inline Vector3D operator-(T scalar) const
        {
            return Vector3D(_x - scalar, _y - scalar, _z - scalar);
        }

        /// @brief Multiplies each element of the vector by a scalar
        /// @param scalar The scalar to multiply
        /// @return The resulting vector
        [[nodiscard]] inline Vector3D operator*(T scalar) const
        {
            return Vector3D(_x * scalar, _y * scalar, _z * scalar);
        }

        /// @brief Divides each element of the vector by a scalar
        /// @param scalar The scalar to divide by
        /// @return The resulting vector
        [[nodiscard]] inline Vector3D operator/(T scalar) const
        {
            if (scalar == 0)
                throw RayTracer::RayTracerException("Vector3D: Division by zero (scalar).");
            return Vector3D(_x / scalar, _y / scalar, _z / scalar);
        }

        // Operator ...= with scalar

        /// @brief Adds a scalar to each element of this vector
        /// @param scalar The scalar to add
        /// @return Reference to the modified vector
        inline Vector3D& operator+=(T scalar)
        {
            _x += scalar;
            _y += scalar;
            _z += scalar;
            return *this;
        }

        /// @brief Subtracts a scalar from each element of this vector
        /// @param scalar The scalar to subtract
        /// @return Reference to the modified vector
        inline Vector3D& operator-=(T scalar)
        {
            _x -= scalar;
            _y -= scalar;
            _z -= scalar;
            return *this;
        }

        /// @brief Multiplies each element of this vector by a scalar
        /// @param scalar The scalar to multiply
        /// @return Reference to the modified vector
        inline Vector3D& operator*=(T scalar)
        {
            _x *= scalar;
            _y *= scalar;
            _z *= scalar;
            return *this;
        }

        /// @brief Divides each element of this vector by a scalar
        /// @param scalar The scalar to divide by
        /// @return Reference to the modified vector
        inline Vector3D& operator/=(T scalar)
        {
            if (scalar == 0)
                throw RayTracer::RayTracerException("Vector3D: Division by zero (scalar).");
            _x /= scalar;
            _y /= scalar;
            _z /= scalar;
            return *this;
        }
    };
}  // namespace Math
