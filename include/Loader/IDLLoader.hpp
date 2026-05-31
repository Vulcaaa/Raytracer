/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** IDLLoader
*/

/// @file IDLLoader.hpp
/// @brief An interface for a class that loads and manages shared libraries

#pragma once

#include <memory>
#include <string>

namespace RayTracer {
    /// @brief An interface for a class that loads and manages shared libraries
    /// @tparam T The type of the module to load
    template <typename T>
    class IDLLoader {
        public:
        /// @brief Virtual destructor
        virtual ~IDLLoader() = default;

        /// @brief Get an instance of the loaded module
        /// @param entryPointName The name of the entry point function to call (default is
        /// "entryPoint")
        /// @return A pointer to the instance of the loaded module, or nullptr if there was an error
        virtual std::unique_ptr<T> getInstance(
            const std::string& entryPointName = "entryPoint") = 0;
    };
}  // namespace RayTracer
