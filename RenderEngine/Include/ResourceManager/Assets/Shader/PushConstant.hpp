#pragma once

#ifndef RENDERENGINE_PUSHCONSTANT
#define RENDERENGINE_PUSHCONSTANT

#include "AssetLoader/RawAsset/Shader/ShaderStage.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief shader PushConstant data struct
    */
    struct PushConstant
    {
        /// shader stage
        Loader::ShaderStage shaderStage = Loader::ShaderStage::VERTEX;
        
        /// push constant block size
        uint32_t size = 0u;

        /// push constant block offset
        uint32_t Offset = 0u;

        /// default constructor
        PushConstant() = default;

        /// destructor
        ~PushConstant() = default;
    };
}

#endif