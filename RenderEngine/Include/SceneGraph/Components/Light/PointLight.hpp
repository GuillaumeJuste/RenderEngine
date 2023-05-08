#pragma once

#ifndef RENDERENGINE_POINTLIGHT
#define RENDERENGINE_POINTLIGHT

#include "SceneGraph/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "SceneGraph/Components/Light/Light.hpp"

namespace RenderEngine::SceneGraph
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