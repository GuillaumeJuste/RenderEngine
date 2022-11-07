#pragma once

#ifndef RENDERENGINE_POINTLIGHT
#define RENDERENGINE_POINTLIGHT

#include "Core/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Core
{
    /**
     * @brief Component class implementation
    */
    class PointLight : public Component
    {
    public:
        Mathlib::Vec3 color;

        float range;

        float ambient;
        float diffuse;
        float specular;

        PointLight() = default;
        ~PointLight() = default;

        void Initialize();
        void Start();
        void Update();
    };
}

#endif