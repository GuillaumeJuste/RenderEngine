#pragma once

#ifndef RENDERENGINE_POINTLIGHT
#define RENDERENGINE_POINTLIGHT

#include "Core/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "Core/Components/Light/Light.hpp"

namespace RenderEngine::Core
{
    /**
     * @brief Point Light class implementation
    */
    class PointLight : public Component, public Light
    {
    public:
        float range;

        PointLight() = default;
        ~PointLight() = default;

        void Initialize();
        void Start();
        void Update();
    };
}

#endif