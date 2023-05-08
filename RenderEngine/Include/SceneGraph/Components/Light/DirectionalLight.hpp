#pragma once

#ifndef RENDERENGINE_DIRECTIONALLIGHT
#define RENDERENGINE_DIRECTIONALLIGHT

#include "SceneGraph/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "SceneGraph/Components/Light/Light.hpp"

namespace RenderEngine::SceneGraph
{
    /**
     * @brief Directional Light class implementation
    */
    class DirectionalLight : public Component, public Light
    {
    public:
        DirectionalLight() = default;
        ~DirectionalLight() = default;

        void Initialize();
        void Start();
        void Update();
    };
}

#endif