/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Renderer
*/

/// @file Renderer.cpp
/// @brief Implementation of the Renderer class.

#include "Renderer.hpp"

#include <omp.h>

#include <fstream>

#include "ConfigUtils.hpp"
#include "MaterialUtils.hpp"
#include "RayTracerException.hpp"

void RayTracer::Renderer::init(const libconfig::Setting& setting)
{
    if (!setting.exists("resolution")) throw RayTracerException("Renderer: Missing resolution.");
    const libconfig::Setting& res = setting["resolution"];
    if (!res.lookupValue("width", _width) || !res.lookupValue("height", _height))
        throw RayTracerException("Renderer: Invalid or missing resolution width/height.");

    if (setting.exists("background")) {
        std::string bgName = setting["background"];
        _backgroundMaterialName = bgName;
    } else
        _backgroundMaterialName = "";

    setting.lookupValue("samples", _samples);
    setting.lookupValue("maxDepth", _maxDepth);
}

void RayTracer::Renderer::computeRows(const Camera& camera,
                                      const std::vector<std::unique_ptr<IPrimitive>>& primitives,
                                      const std::vector<std::unique_ptr<ILight>>& lights,
                                      std::vector<uint8_t>& fullBuffer) const
{
    double ratio = static_cast<double>(_width) / _height;

#pragma omp parallel for schedule(dynamic, 8) collapse(2)

    for (int y = _height - 1; y >= 0; y--) {
        for (int x = 0; x < _width; x++) {
            Math::Vector3D<double> pixelColor(0.0, 0.0, 0.0);
            for (int s = 0; s < _samples; s++) {
                double u = (x + MaterialUtils::randomDouble()) / (_width - 1);
                double v = (y + MaterialUtils::randomDouble()) / (_height - 1);
                Ray r = camera.ray(u, v, ratio);
                pixelColor += computeRayColor(r, _maxDepth, primitives, lights);
            }
            pixelColor = pixelColor / static_cast<double>(_samples);

            int row = (_height - 1 - y);
            int idx = (row * _width + x) * 3;
            fullBuffer[idx] = static_cast<uint8_t>(std::clamp(pixelColor._x, 0.0, 255.0));
            fullBuffer[idx + 1] = static_cast<uint8_t>(std::clamp(pixelColor._y, 0.0, 255.0));
            fullBuffer[idx + 2] = static_cast<uint8_t>(std::clamp(pixelColor._z, 0.0, 255.0));
        }
    }
}

void RayTracer::Renderer::render(const Camera& camera,
                                 const std::vector<std::unique_ptr<IPrimitive>>& primitives,
                                 const std::vector<std::unique_ptr<ILight>>& lights,
                                 const std::string& filename) const
{
    std::vector<uint8_t> fullBuffer(_width * _height * 3);
    std::ofstream outFile(filename);

    if (!outFile.is_open())
        throw RayTracerException("Renderer: Cannot open output file " + filename);

    outFile << "P6\n" << _width << ' ' << _height << "\n255\n";
    computeRows(camera, primitives, lights, fullBuffer);

    outFile.write(reinterpret_cast<const char*>(fullBuffer.data()), fullBuffer.size());
    outFile.close();
}

Math::Vector3D<double> RayTracer::Renderer::computeRayColor(
    const Ray& ray, int depth, const std::vector<std::unique_ptr<IPrimitive>>& primitives,
    const std::vector<std::unique_ptr<ILight>>& lights) const
{
    if (depth <= 0) return {0.0, 0.0, 0.0};
    bool hitAnything = false;
    HitRecord tempRec;
    HitRecord closestRec;
    double closest = std::numeric_limits<double>::max();
    const IPrimitive* hitPrimitive = nullptr;

    for (const auto& primitive : primitives) {
        if (primitive->hits(ray, tempRec)) {
            if (tempRec.distance < closest && tempRec.distance > 0.001) {
                closest = tempRec.distance;
                closestRec = tempRec;
                hitPrimitive = primitive.get();
                hitAnything = true;
            }
        }
    }

    if (hitAnything) {
        Ray scattered;
        Math::Vector3D<double> attenuation;
        if (closestRec.material)
            closestRec.material->scatter(ray, closestRec, attenuation, scattered);

        Math::Vector3D<double> totalLight(0.0, 0.0, 0.0);
        for (const auto& light : lights) {
            if (!light->castsShadow() ||
                !MaterialUtils::isInShadow(closestRec, light->getDirection(), primitives))
                totalLight += light->computeLight(closestRec);
        }
        totalLight._x = std::clamp(totalLight._x, 0.0, 1.0);
        totalLight._y = std::clamp(totalLight._y, 0.0, 1.0);
        totalLight._z = std::clamp(totalLight._z, 0.0, 1.0);

        Math::Vector3D<double> baseColor = (hitPrimitive->getColor() / 255.0) * totalLight;
        if (closestRec.material) {
            if (closestRec.material->isReflective()) {
                return computeRayColor(scattered, depth - 1, primitives, lights) * attenuation;
            } else if (closestRec.material->isRefractive()) {
                auto refractiveColor =
                    computeRayColor(scattered, depth - 1, primitives, lights) * attenuation;
                double t = closestRec.material->getRefractive();
                return refractiveColor * t + baseColor * 255.0 * (1.0 - t);
            } else {
                Math::Vector3D<double> specular =
                    closestRec.material->computeSpecular(ray, closestRec, lights, primitives);
                return attenuation * totalLight * 255.0 + specular * 255.0;
            }
        }
        return baseColor * 255.0;
    }

    if (_backgroundMaterial) {
        Math::Vector3D<double> unitDir = ray._direction.normalized();
        double theta = std::acos(-unitDir._y);
        double phi = std::atan2(-unitDir._z, unitDir._x) + std::numbers::pi;

        double u = phi / (2.0 * std::numbers::pi);
        double v = theta / std::numbers::pi;

        return _backgroundMaterial->getColor(u, v);
    }
    return {0.0, 0.0, 0.0};
}