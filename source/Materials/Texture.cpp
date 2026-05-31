/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Texture
*/

#include "Texture.hpp"

#include <cmath>
#include <iostream>
#include <numbers>

#include "ConfigUtils.hpp"
#include "IPrimitive.hpp"
#include "MaterialUtils.hpp"
#include "RayTracerException.hpp"

void RayTracer::Texture::init(const libconfig::Setting& setting)
{
    if (setting.exists("name"))
        _name = static_cast<const char*>(setting["name"]);
    else
        throw RayTracerException("Texture material must have a name");

    if (!setting.lookupValue("filepath", _filepath))
        throw RayTracerException("Texture: Missing 'filepath' parameter.");

    // Load the image using SFML
    if (!_image.loadFromFile(_filepath))
        throw RayTracerException("Texture: Failed to load image " + _filepath);
    _width = _image.getSize().x;
    _height = _image.getSize().y;

    if (!ConfigUtils::getAsDouble(setting, "strength", _strength)) _strength = 0.0;

    if (!ConfigUtils::getAsDouble(setting, "step", _step)) _step = 1.0;
}

bool RayTracer::Texture::scatter(const Ray& /*rayIn*/, HitRecord& rec,
                                 Math::Vector3D<double>& attenuation, Ray& scattered) const
{
    Math::Vector3D<double> scatterDir = rec.normal + MaterialUtils::randomUnitVector();

    if (scatterDir.dot(scatterDir) < 1e-8) scatterDir = rec.normal;

    scattered = Ray(rec.p, scatterDir);

    if (_width > 0 && _height > 0) {
        // Apply UV mapping
        double u = std::fmod(rec.u, 1.0);
        double v = std::fmod(rec.v, 1.0);
        if (u < 0) u += 1.0;
        if (v < 0) v += 1.0;

        unsigned int i = static_cast<unsigned int>(u * _width);
        unsigned int j = static_cast<unsigned int>((1.0 - v) * _height - 0.001);

        if (i >= static_cast<unsigned int>(_width)) i = _width - 1;
        if (j >= static_cast<unsigned int>(_height)) j = _height - 1;

        unsigned int iNext = (i + _step < static_cast<unsigned int>(_width)) ? i + _step : i;
        unsigned int jNext = (j + _step < static_cast<unsigned int>(_height)) ? j + _step : j;

        sf::Color currentPixel = _image.getPixel(i, j);
        double lCurrentPixel = getLuminance(currentPixel);
        double lRightPixel = getLuminance(_image.getPixel(iNext, j));
        double lDownPixel = getLuminance(_image.getPixel(i, jNext));

        double du = (lCurrentPixel - lRightPixel) * _strength;
        double dv = (lCurrentPixel - lDownPixel) * _strength;

        Math::Vector3D<double> tangent(rec.normal._z, 0.0, -rec.normal._x);
        if (tangent.dot(tangent) < 1e-6) tangent = Math::Vector3D<double>(1, 0, 0);
        tangent = tangent.normalized();

        Math::Vector3D<double> bitangent = rec.normal.cross(tangent).normalized();

        Math::Vector3D<double> worldNormal =
            (rec.normal + (tangent * du) + (bitangent * dv)).normalized();
        rec.normal = worldNormal;

        Math::Vector3D<double> sDir = worldNormal + MaterialUtils::randomUnitVector();
        if (sDir.dot(sDir) < 1e-8) sDir = worldNormal;

        scattered = Ray(rec.p, sDir);

        attenuation = Math::Vector3D<double>(currentPixel.r / 255.0, currentPixel.g / 255.0,
                                             currentPixel.b / 255.0);
    } else
        attenuation = Math::Vector3D<double>(1, 0, 1);

    return true;
}

Math::Vector3D<double> RayTracer::Texture::getColor(double u, double v) const
{
    if (_width > 0 && _height > 0) {
        u = std::fmod(u, 1.0);
        v = std::fmod(v, 1.0);
        if (u < 0) u += 1.0;
        if (v < 0) v += 1.0;

        unsigned int i = static_cast<unsigned int>(u * _width);
        unsigned int j = static_cast<unsigned int>((1.0 - v) * _height - 0.001);

        if (i >= static_cast<unsigned int>(_width)) i = _width - 1;
        if (j >= static_cast<unsigned int>(_height)) j = _height - 1;

        sf::Color pixel = _image.getPixel(i, j);
        return Math::Vector3D<double>(pixel.r, pixel.g, pixel.b);
    }
    return Math::Vector3D<double>(255, 0, 255);
}

double RayTracer::Texture::getLuminance(sf::Color pixel) const
{
    return (0.2126 * pixel.r + 0.7152 * pixel.g + 0.0722 * pixel.b) / 255.0;
}

extern "C" {
RayTracer::IMaterial* entryPoint() { return new RayTracer::Texture(); }
}
