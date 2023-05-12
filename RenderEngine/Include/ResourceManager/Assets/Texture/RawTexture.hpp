#pragma once

#ifndef RENDERENGINE_RAWTEXTURE
#define RENDERENGINE_RAWTEXTURE

#include "ResourceManager/Assets/IAsset.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Texture class implementation
    */
    struct RawTexture : public IAsset
    {
        using IAsset::IAsset;

        int width;
        int height;
        int imageSize;

        uint32_t textureCount;

        char* pixels;

        RawTexture() = default;
        ~RawTexture() = default;
    };
}

#endif