#pragma once

#ifndef RENDERENGINE_LIGHT
#define RENDERENGINE_LIGHT

#include "SceneGraph/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Component
{
    /**
     * @brief Light class implementation
    */
    class Light
    {
    public:
        /// Light color
        Mathlib::Vec3 color;
        
        /// Light intensity
        float intensity = 1.0f;

        /// default constructor
        Light() = default;

        /// default destructor
        ~Light() = default;
    };
}

#endif