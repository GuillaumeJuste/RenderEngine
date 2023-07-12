#pragma once

#ifndef RENDERENGINE_MATERIAL
#define RENDERENGINE_MATERIAL

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"

using namespace RenderEngine::Assets;

namespace RenderEngine::Component
{
    /**
     * @brief Material struct
    */
    struct Material
    {
        /// Albedo
        Texture* albedo = nullptr;
        
        /// Metalness map
        Texture* metalnessMap = nullptr;
        
        /// Roughness map
        Texture* roughnessMap = nullptr;
        
        /// Normal map
        Texture* normalMap = nullptr;
        
        /// Ambient Occlusion map
        Texture* ambientOcclusionMap = nullptr;

        /// Shininess constant
        float shininess = 0.f;

        /// Ambiant constant
        Mathlib::Vec3 ambient = Mathlib::Vec3::Zero;
        
        /// Diffuse constant
        Mathlib::Vec3 diffuse = Mathlib::Vec3::Zero;
        
        /// Specular constant
        Mathlib::Vec3 specular = Mathlib::Vec3::Zero;
    };
}

#endif