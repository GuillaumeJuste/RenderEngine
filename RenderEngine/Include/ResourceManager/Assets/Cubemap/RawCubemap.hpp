#pragma once

#ifndef RENDERENGINE_RAWCUBEMAP
#define RENDERENGINE_RAWCUBEMAP

#include <string>
#include "ResourceManager/Assets/Texture/RawTexture.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief RawCubemap class implementation
    */
    struct RawCubemap : public RawTexture
    {
        RawCubemap() = default;
        ~RawCubemap() = default;
    };
}

#endif