#pragma once

#ifndef RENDERENGINE_RAWSHADER
#define RENDERENGINE_RAWSHADER

#include <vector>

namespace RenderEngine::Assets
{
    enum SHADER_STAGE
    {
        VERTEX,
        FRAGMENT,
        COMPUTE,
        GEOMETRY
    };
    /**
     * @brief Texture class implementation
    */
    struct RawShader
    {
        std::vector<char> shaderCode;

        SHADER_STAGE stage = SHADER_STAGE::VERTEX;

        RawShader() = default;
        ~RawShader() = default;
    };
}

#endif