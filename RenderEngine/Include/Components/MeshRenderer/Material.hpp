#pragma once

#ifndef RENDERENGINE_MATERIAL
#define RENDERENGINE_MATERIAL

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "Rendering/Base/ResourceManager/Assets/Texture/Texture.hpp"
#include "Rendering/Base/ResourceManager/Assets/Shader/ShaderDescriptorSet.hpp"
#include "Rendering/Base/ResourceManager/Assets/Shader/Shader.hpp"
#include "Rendering/Base/Enum/PolygonDrawMode.hpp"
#include "Rendering/Base/Enum/FrontFace.hpp"

using namespace RenderEngine::Assets;
using namespace RenderEngine::Rendering;

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

        /// Vertex Shader
        Shader* vertexShader = nullptr;

        /// Vertex Shader Descriptor set
        ShaderDescriptorSet vertexShaderDescriptorSet;

        /// Fragment Shader
        Shader* fragmentShader = nullptr;
        
        /// Fragment Shader Descriptor set
        ShaderDescriptorSet fragmentShaderDescriptorSet;

        /// vertex draw order
        FrontFace frontFace = FrontFace::CLOCKWISE;

        /// mesh draw mode
        PolygonDrawMode drawMode = PolygonDrawMode::FILL;

        /// outline width
        float lineWidth = 1.0f;
    };
}

#endif