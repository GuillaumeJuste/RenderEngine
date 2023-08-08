#pragma once

#ifndef RENDERENGINE_SHADER
#define RENDERENGINE_SHADER

#include "ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/IShader.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Shader struct implementation
    */
    struct Shader : public IAsset
    {
        /// shader handle
        RenderEngine::Rendering::IShader* iShader;

        /// default constructor
        Shader() = default;

        /// destructor
        ~Shader();
    };
}

#endif