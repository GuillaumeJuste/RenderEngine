#pragma once

#ifndef RENDERENGINE_LIGHT
#define RENDERENGINE_LIGHT

#include "Core/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Core
{
    /**
     * @brief Component class implementation
    */
    class Light : public Component
    {
    public:
        Mathlib::Vec3 color;
        Mathlib::Vec3 ambient;
        Mathlib::Vec3 diffuse;
        Mathlib::Vec3 specular;

        Light() = default;
        ~Light() = default;

        void Initialize();
        void Start();
        void Update();
    };
}

#endif