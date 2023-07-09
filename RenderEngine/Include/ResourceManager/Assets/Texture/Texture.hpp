#pragma once

#ifndef RENDERENGINE_TEXTURE
#define RENDERENGINE_TEXTURE

#include "ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/ITexture.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Texture class implementation
    */
    struct Texture : public IAsset
    {
        int width;
        int height;
        int channels = 4;
        uint32_t imageSize;
        uint32_t mipLevels;
        uint32_t imageCount = 1;
        bool isHDR = false;

        RenderEngine::Rendering::ITexture* iTexture;

        Texture() = default;
        ~Texture();

        static uint32_t ComputeTotalSize(int _width, int _height, int _channels, int _mipmapLevels);
    };
}

#endif