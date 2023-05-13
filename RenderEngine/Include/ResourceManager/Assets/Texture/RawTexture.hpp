#pragma once

#ifndef RENDERENGINE_RAWTEXTURE
#define RENDERENGINE_RAWTEXTURE

#include <string>

namespace RenderEngine::Assets
{
    /**
     * @brief Texture class implementation
    */
    struct RawTexture
    {
        int width;
        int height;
        int imageSize;
        int channels;

        uint32_t textureCount;

        char* pixels;

        RawTexture() = default;
        ~RawTexture() = default;
    };
}

#endif