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
        uint32_t mipLevels = 1;
        unsigned int imageCount = 1;

        bool isHdr = false;

        union
        {
            char* dataC;
            float* dataF;
        };

        RawTexture() = default;
        ~RawTexture() = default;
    };
}

#endif