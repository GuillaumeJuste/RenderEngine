#pragma once

#ifndef RENDERENGINE_SHADER
#define RENDERENGINE_SHADER

#include "ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/IShader.hpp"
#include "ResourceManager/Assets/Shader/RawShader.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Texture class implementation
    */
    struct Shader : public IAsset
    {
        RenderEngine::Rendering::IShader* iShader;

        SHADER_STAGE stage = SHADER_STAGE::VERTEX;

        Shader() = default;
        ~Shader();
    };
}

#endif