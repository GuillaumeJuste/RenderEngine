#pragma once

#ifndef RENDERENGINE_SHADERDESCRIPTOR
#define RENDERENGINE_SHADERDESCRIPTOR

#include "Rendering/Base/ResourceManager/Assets/Texture/Texture.hpp"
#include "AssetLoader/RawAsset/Shader/ShaderStage.hpp"
using namespace Loader;

namespace RenderEngine::Assets
{
    /// custom buffer data structure
    struct BufferData
    {
        /// pointer adress
        void* data = nullptr;
        /// buffer size
        uint32_t size = 0u;
    };

    /// descriptor type
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
        /// shader stage

        ShaderStage shaderStage = ShaderStage::VERTEX;

        /// descriptor type
        DescriptorType descriptorType = DescriptorType::UNKNOWN;

        /// descriptor set
        uint32_t set = 0u;

        /// descriptor binding
        uint32_t binding = 0u;

        /// custom buffer / texture holder
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