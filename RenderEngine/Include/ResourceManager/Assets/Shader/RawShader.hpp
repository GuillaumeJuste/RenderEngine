#pragma once

#ifndef RENDERENGINE_RAWSHADER
#define RENDERENGINE_RAWSHADER

#include <vector>

namespace RenderEngine::Assets
{
    /// Supported shader stages
    enum SHADER_STAGE
    {
        VERTEX,
        FRAGMENT,
        COMPUTE,
        GEOMETRY
    };
    /**
     * @brief raw shader data structure
    */
    struct RawShader
    {
        /// compiled shader
        std::vector<char> shaderCode;

        /// shader stage
        SHADER_STAGE stage = SHADER_STAGE::VERTEX;

        /// default constructor
        RawShader() = default;

        /// default destructor
        ~RawShader() = default;
    };
}

#endif