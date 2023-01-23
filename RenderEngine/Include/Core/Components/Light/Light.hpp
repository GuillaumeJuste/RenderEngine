#pragma once

#ifndef RENDERENGINE_LIGHT
#define RENDERENGINE_LIGHT

#include "Core/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Core
{
    /**
     * @brief Light class implementation
    */
    class Light
    {
    public:
        Mathlib::Vec3 color;
        float intensity = 1.0f;

        Light() = default;
        ~Light() = default;
    };
}

#endif