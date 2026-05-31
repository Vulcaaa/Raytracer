/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** SceneParser
*/

/// @file SceneParser.hpp
/// @brief SceneParser class for parsing scene files

#pragma once

#include <libconfig.h++>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Camera.hpp"
#include "ILight.hpp"
#include "IMaterial.hpp"
#include "IPrimitive.hpp"
#include "PrimitiveGroup.hpp"
#include "Renderer.hpp"

/// @brief Namespace for the Ray Tracer project
namespace RayTracer {
    /// @brief Struct to hold the parsed scene data
    struct SceneData {
        /// @brief Camera object representing the camera in the scene
        Camera camera;
        /// @brief Renderer object representing the renderer configuration
        Renderer renderer;
        /// @brief List of primitives in the scene
        std::vector<std::unique_ptr<IPrimitive>> primitives;
        /// @brief List of lights in the scene
        std::vector<std::unique_ptr<ILight>> lights;
        /// @brief Map of meterials available in the scene, indexed by their name
        std::map<std::string, std::shared_ptr<IMaterial>> materials;
    };

    /// @brief SceneParser class responsible for parsing scene files
    class SceneParser {
        public:
        /// @brief Parses a scene file and returns the scene data
        /// @param filePath The path to the scene file
        /// @return The parsed scene data
        SceneData parse(const std::string& filePath);

        private:
        void setMaterialstoPrimitives(
            std::vector<std::unique_ptr<IPrimitive>>& primitives,
            const std::map<std::string, std::shared_ptr<IMaterial>>& materials);

        void parse_import(const libconfig::Setting& importSetting,
                          std::vector<std::unique_ptr<IPrimitive>>& primitives,
                          std::map<std::string, std::shared_ptr<IMaterial>> materials,
                          std::vector<std::unique_ptr<ILight>>& lights);

        void add_primitiveGroup_lights(const libconfig::Setting& setting, SceneData& data,
                                       std::vector<std::unique_ptr<IPrimitive>>& primitives,
                                       std::map<std::string, std::shared_ptr<IMaterial>> materials,
                                       std::vector<std::unique_ptr<ILight>>& lights);
    };
}  // namespace RayTracer
