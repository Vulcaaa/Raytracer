/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Camera
*/

/// @file Camera.cpp
/// @brief Implementation of the Camera class.

#include "Camera.hpp"

#include "ConfigUtils.hpp"

void RayTracer::Camera::init(const libconfig::Setting& setting)
{
    ConfigUtils::parsePoint3D(setting, "position", _position, false);
    ConfigUtils::parseVector3D(setting, "rotation", _rotation, false);

    _fieldOfView = 90.0;
    ConfigUtils::getAsDouble(setting, "fieldOfView", _fieldOfView);
}
