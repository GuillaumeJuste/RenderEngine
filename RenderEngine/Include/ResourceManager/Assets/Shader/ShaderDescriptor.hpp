#pragma once

#ifndef RENDERENGINE_SHADERDESCRIPTOR
#define RENDERENGINE_SHADERDESCRIPTOR

#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "ResourceManager/Assets/Shader/ShaderStage.hpp"

namespace RenderEngine::Assets
{
    struct BufferData
    {
        void* data = nullptr;
        uint32_t size = 0u;
    };

    enum class DescriptorType : uint32_t
    {
        UNKNOWN = 0,
        UNIFORM_BUFFER,
        CAMERA_BUFFER,
        MESHRENDERER_MATERIAL_BUFFER,
        POINT_LIGHT_BUFFER,
        DIRECTIONAL_LIGHT_BUFFER,
        SPOT_LIGHT_BUFFER,
        CUSTOM_BUFFER,
        SKYBOX_CUBEMAP,
        SKYBOX_IRRADIANCE,
        SKYBOX_PREFILTERED,
        SKYBOX_BRDF_LUT,
        MESHRENDERER_MATERIAL_ALBEDO,
        MESHRENDERER_MATERIAL_METALNESS,
        MESHRENDERER_MATERIAL_ROUGHNESS,
        MESHRENDERER_MATERIAL_NORMALMAP,
        MESHRENDERER_MATERIAL_AOMAP,
        CUSTOM_TEXTURE
    };

    /**
     * @brief ShaderDescriptor data struct
    */
    struct ShaderDescriptor
    {
        ShaderStage shaderStage = ShaderStage::VERTEX;
        DescriptorType descriptorType = DescriptorType::UNKNOWN;
        uint32_t set = 0u;
        uint32_t binding = 0u;
        union
        {
            BufferData bufferData;
            Texture* texture;
        };


        /// default constructor
        ShaderDescriptor() = default;

        /// destructor
        ~ShaderDescriptor() = default;
    };
}

#endif