/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ConfigUtils
*/

/// @file ConfigUtils.hpp
/// @brief Configuration utilities

#pragma once

#include <libconfig.h++>
#include <string>

#include "Point3D.hpp"
#include "RayTracerException.hpp"
#include "Vector3D.hpp"

namespace RayTracer {
    namespace ConfigUtils {
        /// @brief Helper function to extract a double value from a libconfig setting
        /// @param setting The libconfig setting to extract the value from
        /// @param key The key of the value to extract
        /// @param value The variable to store the extracted value
        /// @return True if the value was successfully extracted, false otherwise
        inline bool getAsDouble(const libconfig::Setting& setting, const std::string& key,
                                double& value)
        {
            int intValue = 0;
            if (setting.lookupValue(key.c_str(), intValue)) {
                value = static_cast<double>(intValue);
                return true;
            }

            float floatValue = 0.0f;
            if (setting.lookupValue(key.c_str(), floatValue)) {
                value = static_cast<double>(floatValue);
                return true;
            }

            return setting.lookupValue(key.c_str(), value);
        }

        /// @brief Helper function to parse a Vector3D from a libconfig setting
        /// @param setting The libconfig setting to extract the value from
        /// @param key The key of the value to extract
        /// @param value The variable to store the extracted value
        /// @param required Whether the parameter is required (throws an exception if true and the
        /// parameter is missing)
        /// @note Default value is (0, 0, 0) if the parameter is not required and missing
        inline void parseVector3D(const libconfig::Setting& setting, const std::string& key,
                                  Math::Vector3D<double>& value, bool required = false)
        {
            double x = 0, y = 0, z = 0;
            if (setting.exists(key.c_str())) {
                const libconfig::Setting& v = setting[key.c_str()];
                getAsDouble(v, "x", x);
                getAsDouble(v, "y", y);
                getAsDouble(v, "z", z);
                value = Math::Vector3D<double>(x, y, z);
            } else {
                if (required) throw RayTracerException("Missing required parameter: " + key);
            }
            value = Math::Vector3D<double>(x, y, z);
        }

        /// @brief Helper function to parse a Point3D from a libconfig setting
        /// @param setting The libconfig setting to extract the value from
        /// @param key The key of the value to extract
        /// @param value The variable to store the extracted value
        /// @param required Whether the parameter is required (throws an exception if true and the
        /// parameter is missing)
        /// @note Default value is (0, 0, 0) if the parameter is not required and missing
        inline void parsePoint3D(const libconfig::Setting& setting, const std::string& key,
                                 Math::Point3D<double>& value, bool required)
        {
            double x = 0, y = 0, z = 0;
            if (setting.exists(key.c_str())) {
                const libconfig::Setting& v = setting[key.c_str()];
                getAsDouble(v, "x", x);
                getAsDouble(v, "y", y);
                getAsDouble(v, "z", z);
            } else {
                if (required) throw RayTracerException("Missing required parameter: " + key);
            }
            value = Math::Point3D<double>(x, y, z);
        }

        /// @brief Helper function to parse a color from a libconfig setting
        /// @param setting The libconfig setting to extract the value from
        /// @param key The key of the value to extract
        /// @param value The variable to store the extracted value
        /// @note Default color is white (255, 255, 255) if the parameter is missing
        inline void parseColor(const libconfig::Setting& setting, const std::string& key,
                               Math::Vector3D<double>& value)
        {
            double r = 255, g = 255, b = 255;
            if (setting.exists(key.c_str())) {
                const libconfig::Setting& c = setting[key.c_str()];
                getAsDouble(c, "r", r);
                getAsDouble(c, "g", g);
                getAsDouble(c, "b", b);
                value = Math::Vector3D<double>(r, g, b);
            } else
                value = Math::Vector3D<double>(r, g, b);
        }
    }  // namespace ConfigUtils
}  // namespace RayTracer
