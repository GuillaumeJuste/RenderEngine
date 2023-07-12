#pragma once

#ifndef RENDERENGINE_SHADER
#define RENDERENGINE_SHADER

#include "ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/IShader.hpp"
#include "ResourceManager/Assets/Shader/RawShader.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Shader struct implementation
    */
    struct Shader : public IAsset
    {
        /// shader handle
        RenderEngine::Rendering::IShader* iShader;

        /// shader stage
        SHADER_STAGE stage = SHADER_STAGE::VERTEX;

        /// default constructor
        Shader() = default;

        /// destructor
        ~Shader();
    };
}

#endif