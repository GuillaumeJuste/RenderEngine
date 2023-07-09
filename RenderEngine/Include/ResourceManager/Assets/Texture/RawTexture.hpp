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
        int width = 0;
        int height = 0;
        int imageSize = 0;
        int channels = 4;
        uint32_t mipLevels = 1;
        unsigned int imageCount = 1;

        bool isHdr = false;

        union
        {
            char* dataC ;
            float* dataF;
        };

        RawTexture() = default;
        ~RawTexture() = default;
    };
}

#endif