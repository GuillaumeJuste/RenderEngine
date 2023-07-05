#pragma once

#ifndef RENDERENGINE_SPOTLIGHT
#define RENDERENGINE_SPOTLIGHT

#include "SceneGraph/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "SceneGraph/Components/Light/Light.hpp"

namespace RenderEngine::SceneGraph
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
        void Update(double _deltaTime);
        void FixedUpdate(double _deltaTime);
    };
}

#endif