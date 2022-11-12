#pragma once

#ifndef RENDERENGINE_SPOTLIGHT
#define RENDERENGINE_SPOTLIGHT

#include "Core/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "Core/Components/Light/Light.hpp"

namespace RenderEngine::Core
{
    /**
     * @brief Spot Light class implementation
    */
    class SpotLight : public Component, public Light
    {
    public:
        float range;
        float cutOff;

        SpotLight() = default;
        ~SpotLight() = default;

        void Initialize();
        void Start();
        void Update();
    };
}

#endif