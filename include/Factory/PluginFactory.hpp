/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PluginFactory
*/

/// @file PluginFactory.hpp
/// @brief Generic factory for creating plugins (primitives, lights, etc.)

#pragma once

#include <memory>
#include <string>

#include "DLLoader.hpp"
#include "RayTracerException.hpp"

namespace RayTracer {
    /// @brief Generic factory class for creating plugins from configuration settings
    /// @tparam T The base type of the plugin (e.g., IPrimitive, ILight)
    template <typename T>
    class PluginFactory {
        public:
        /// @brief Creates a plugin by loading the specified shared library and initializing it
        /// @param libPath The path to the shared library to load
        /// @param setting The configuration settings for the plugin, used to initialize it
        /// @return A unique pointer to the created plugin
        [[nodiscard]] static std::unique_ptr<T> create(const std::string& libPath,
                                                       const libconfig::Setting& setting)
        {
            DLLoader<T> loader(libPath);

            auto instance = loader.getInstance();
            if (!instance)
                throw RayTracerException("PluginFactory: Failed to load plugin at " + libPath);

            instance->init(setting);

            return instance;
        }
    };
}  // namespace RayTracer
