/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** RayTracerException
*/

/// @file RayTracerException.cpp
/// @brief Implementation of the RayTracerException class.

#include "RayTracerException.hpp"

RayTracer::RayTracerException::RayTracerException(const std::string& message) : _message(message) {}

const char* RayTracer::RayTracerException::what() const noexcept { return _message.c_str(); }
