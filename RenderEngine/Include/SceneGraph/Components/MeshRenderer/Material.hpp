#pragma once

#ifndef RENDERENGINE_MATERIAL
#define RENDERENGINE_MATERIAL

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"

using namespace RenderEngine::Assets;

namespace RenderEngine::SceneGraph
{
    struct Material
    {
        Texture* texture = nullptr;
        Texture* metalnessMap = nullptr;
        Texture* roughnessMap = nullptr;
        Texture* normalMap = nullptr;
        Texture* ambientOcclusionMap = nullptr;

        float shininess = 0.f;
        Mathlib::Vec3 ambient = Mathlib::Vec3::Zero;
        Mathlib::Vec3 diffuse = Mathlib::Vec3::Zero;
        Mathlib::Vec3 specular = Mathlib::Vec3::Zero;
    };
}

#endif