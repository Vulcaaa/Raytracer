/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** ALight - Abstract Light base class implementation
*/

#include "ALight.hpp"

#include "ConfigUtils.hpp"

void RayTracer::ALight::parseCommonProperties(const libconfig::Setting& setting)
{
    ConfigUtils::parseColor(setting, "color", _color);
}
