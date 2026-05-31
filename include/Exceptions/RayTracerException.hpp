/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** RayTracerException
*/

/// @file RayTracerException.hpp
/// @brief RayTracerException class definition

#pragma once

#include <exception>
#include <string>

/// @brief Namespace for the RayTracer project
namespace RayTracer {
    /// @brief Custom exception class for RayTracer-related errors
    class RayTracerException : public std::exception {
        public:
        /// @brief Constructs a RayTracerException object
        /// @param message The error message describing the exception
        RayTracerException(const std::string& message);

        /// @brief Retrieves the error message associated with the exception
        /// @return A C-style string representing the error message
        [[nodiscard]] const char* what() const noexcept override;

        private:
        /// @brief The error message describing the exception
        std::string _message;
    };
}  // namespace RayTracer
