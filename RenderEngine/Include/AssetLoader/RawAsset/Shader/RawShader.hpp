#pragma once

#ifndef RENDERENGINE_RAWSHADER
#define RENDERENGINE_RAWSHADER

#include <vector>
#include "AssetLoader/RawAsset/SHader/ShaderStage.hpp"

namespace Loader
{
    /**
     * @brief raw shader data structure
    */
    struct RawShader
    {
        // is the shader code stored valid
        bool isValid = false;

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