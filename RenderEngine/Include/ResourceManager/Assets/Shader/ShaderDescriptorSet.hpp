#pragma once

#ifndef RENDERENGINE_SHADERDESCRIPTORSET
#define RENDERENGINE_SHADERDESCRIPTORSET

#include <forward_list>
#include "ResourceManager/Assets/Shader/ShaderDescriptor.hpp"
#include "ResourceManager/Assets/Shader/PushConstant.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief ShaderDescriptorSet data struct
    */
    struct ShaderDescriptorSet
    {
        std::forward_list<ShaderDescriptor> shaderDescriptors;

        std::forward_list<PushConstant> pushConstants;

        /// default constructor
        ShaderDescriptorSet() = default;

        /// destructor
        ~ShaderDescriptorSet() = default;

        static ShaderDescriptorSet GenerateDefaultVertexShaderDescriptor();
        static ShaderDescriptorSet GenerateSkyboxVertexShaderDescriptor();
        static ShaderDescriptorSet GenerateFilterCubeVertexShaderDescriptor();

        static ShaderDescriptorSet GenerateDefaultFragmentShaderDescriptor();
        static ShaderDescriptorSet GenerateSkyboxFragmentShaderDescriptor();
        static ShaderDescriptorSet GenerateTextureToCubemapFragmentShaderDescriptor(Texture* _inputTexture);
        static ShaderDescriptorSet GeneratePhongFragmentShaderDescriptor();
        static ShaderDescriptorSet GenerateBlinnPhongFragmentShaderDescriptor();
        static ShaderDescriptorSet GenerateIrradianceConvolutionFragmentShaderDescriptor(Texture* _inputTexture);
        static ShaderDescriptorSet GeneratePrefilterEnvmapFragmentShaderDescriptor(Texture* _inputTexture);
        static ShaderDescriptorSet GeneratePBRFragmentShaderDescriptor();
    };
}

#endif