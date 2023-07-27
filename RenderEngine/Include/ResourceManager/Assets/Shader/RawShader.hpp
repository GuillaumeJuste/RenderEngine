#pragma once

#ifndef RENDERENGINE_RAWSHADER
#define RENDERENGINE_RAWSHADER

#include <vector>
#include "ResourceManager/Assets/Shader/ShaderStage.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief raw shader data structure
    */
    struct RawShader
    {
        /// compiled shader
        std::vector<char> shaderCode;

        /// shader stage
        ShaderStage stage = ShaderStage::VERTEX;

        /// default constructor
        RawShader() = default;

        /// default destructor
        ~RawShader() = default;
    };
}

#endif