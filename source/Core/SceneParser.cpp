/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** SceneParser
*/

#include "SceneParser.hpp"

#include <string>

#include "AmbientLight.hpp"
#include "ConfigUtils.hpp"
#include "PluginFactory.hpp"
#include "Point3D.hpp"
#include "PrimitiveGroup.hpp"
#include "RayTracerException.hpp"
#include "Vector3D.hpp"

void RayTracer::SceneParser::setMaterialstoPrimitives(
    std::vector<std::unique_ptr<IPrimitive>>& primitives,
    const std::map<std::string, std::shared_ptr<IMaterial>>& materials)
{
    for (auto& primitive : primitives) {
        if (primitive->isGroup()) {
            auto* group = static_cast<PrimitiveGroup*>(primitive.get());
            setMaterialstoPrimitives(group->getChildren(), materials);
        } else if (!primitive->getMaterialName().empty()) {
            if (primitive->getMaterial() != nullptr) continue;
            auto it = materials.find(primitive->getMaterialName());
            if (it != materials.end())
                primitive->setMaterial(it->second);
            else
                throw RayTracerException("SceneParser: Material '" + primitive->getMaterialName() +
                                         "' not found for primitive.");
        }
    }
}

void RayTracer::SceneParser::add_primitiveGroup_lights(
    const libconfig::Setting& setting, SceneData& data,
    std::vector<std::unique_ptr<IPrimitive>>& primitives,
    std::map<std::string, std::shared_ptr<IMaterial>> materials,
    std::vector<std::unique_ptr<ILight>>& lights)
{
    PrimitiveGroup group;

    ConfigUtils::parsePoint3D(setting, "position", group._position, true);

    ConfigUtils::parseVector3D(setting, "rotation", group._rotation, false);

    for (auto& p : data.primitives) group.addPrimitive(std::move(p));

    primitives.push_back(std::make_unique<RayTracer::PrimitiveGroup>(std::move(group)));

    for (auto& [name, mat] : data.materials) materials.emplace(name, mat);

    for (auto& l : data.lights) lights.push_back(std::move(l));
}

void RayTracer::SceneParser::parse_import(
    const libconfig::Setting& importSetting, std::vector<std::unique_ptr<IPrimitive>>& primitives,
    std::map<std::string, std::shared_ptr<IMaterial>> materials,
    std::vector<std::unique_ptr<ILight>>& lights)
{
    for (const auto& setting : importSetting) {
        std::string scenePath;
        if (!setting.lookupValue("filepath", scenePath))
            throw RayTracerException("Import Scene: parameter 'filepath' not found");
        SceneData data = parse(scenePath);
        add_primitiveGroup_lights(setting, data, primitives, materials, lights);
    }
}

RayTracer::SceneData RayTracer::SceneParser::parse(const std::string& filePath)
{
    try {
        libconfig::Config cfg;
        cfg.readFile(filePath.c_str());

        const libconfig::Setting& root = cfg.getRoot();

        if (!root.exists("camera"))
            throw RayTracerException("SceneParser: Missing camera configuration.");
        if (!root.exists("renderer"))
            throw RayTracerException("SceneParser: Missing renderer configuration.");

        Camera cam;
        cam.init(root["camera"]);

        Renderer renderer;
        renderer.init(root["renderer"]);

        std::map<std::string, std::shared_ptr<IMaterial>> materials;
        std::vector<std::unique_ptr<IPrimitive>> primitives;
        std::vector<std::unique_ptr<ILight>> lights;

        if (root.exists("materials")) {
            const libconfig::Setting& materialsSetting = root["materials"];

            for (const auto& mat : materialsSetting) {
                std::string typeName = mat.getName();

                for (int j = 0; j < mat.getLength(); ++j) {
                    std::string pluginPath = "./plugins/raytracer_" + typeName + ".so";
                    materials[std::string(mat[j]["name"])] =
                        PluginFactory<IMaterial>::create(pluginPath, mat[j]);
                }
            }
        }

        if (root.exists("primitives")) {
            const libconfig::Setting& primitivesSetting = root["primitives"];

            for (const auto& listSetting : primitivesSetting) {
                std::string typeName = listSetting.getName();

                if (typeName == "import") {
                    parse_import(listSetting, primitives, materials, lights);
                    continue;
                }
                for (const auto& setting : listSetting) {
                    if (setting.exists("children")) {
                        auto primitivesGroup = std::make_unique<PrimitiveGroup>();

                        ConfigUtils::parsePoint3D(setting, "position", primitivesGroup->_position,
                                                  true);
                        std::string pluginPath = "./plugins/raytracer_" + typeName + ".so";
                        auto primitive = PluginFactory<IPrimitive>::create(pluginPath, setting);
                        primitivesGroup->addPrimitive(std::move(primitive));

                        const libconfig::Setting& childrenSetting = setting["children"];
                        for (const auto& c : childrenSetting) {
                            std::string childTypeName = c.getName();

                            if (childTypeName == "translation") {
                                Math::Point3D<double> translation;
                                ConfigUtils::parsePoint3D(childrenSetting, "translation",
                                                          translation, false);
                                primitivesGroup->_position._x += translation._x;
                                primitivesGroup->_position._y += translation._y;
                                primitivesGroup->_position._z += translation._z;
                                continue;
                            } else if (childTypeName == "rotation") {
                                ConfigUtils::parseVector3D(childrenSetting, "rotation",
                                                           primitivesGroup->_rotation, false);
                                continue;
                            }
                            std::string childPluginPath =
                                "./plugins/raytracer_" + childTypeName + ".so";
                            for (const auto& childSetting : c) {
                                primitivesGroup->addPrimitive(PluginFactory<IPrimitive>::create(
                                    childPluginPath, childSetting));
                            }
                        }
                        primitives.push_back(std::move(primitivesGroup));
                        continue;
                    }
                    std::string pluginPath = "./plugins/raytracer_" + typeName + ".so";
                    primitives.push_back(PluginFactory<IPrimitive>::create(pluginPath, setting));
                }
            }
        }

        if (root.exists("lights")) {
            const libconfig::Setting& lightsSetting = root["lights"];

            for (const auto& lightSetting : lightsSetting) {
                std::string typeName = lightSetting.getName();

                if (lightSetting.isList() || lightSetting.isArray() || lightSetting.isGroup()) {
                    for (const auto& setting : lightSetting) {
                        std::string pluginPath = "./plugins/raytracer_" + typeName + ".so";
                        lights.push_back(PluginFactory<ILight>::create(pluginPath, setting));
                    }
                } else {
                    std::string pluginPath = "./plugins/raytracer_" + typeName + ".so";
                    lights.push_back(PluginFactory<ILight>::create(pluginPath, lightsSetting));
                }
            }
        }
        setMaterialstoPrimitives(primitives, materials);

        if (!renderer.getBackgroundMaterialName().empty()) {
            auto it = materials.find(renderer.getBackgroundMaterialName());
            if (it != materials.end())
                renderer.setBackgroundMaterial(it->second);
            else
                throw RayTracerException("SceneParser: Background Material '" +
                                         renderer.getBackgroundMaterialName() + "' not found.");
        }
        return SceneData{std::move(cam), std::move(renderer), std::move(primitives),
                         std::move(lights), std::move(materials)};

    } catch (const libconfig::FileIOException& fioex) {
        throw RayTracerException("I/O error while reading file: " + filePath);
    } catch (const libconfig::ParseException& pex) {
        std::string file = pex.getFile() ? pex.getFile() : "unknown file";
        throw RayTracerException("Parse error at " + file + ":" + std::to_string(pex.getLine()) +
                                 " - " + pex.getError());
    } catch (const libconfig::SettingNotFoundException& nfex) {
        throw RayTracerException("Missing setting in configuration file: " +
                                 std::string(nfex.getPath()));
    } catch (const libconfig::SettingTypeException& stex) {
        throw RayTracerException("Invalid setting type in configuration file: " +
                                 std::string(stex.getPath()));
    }
}
