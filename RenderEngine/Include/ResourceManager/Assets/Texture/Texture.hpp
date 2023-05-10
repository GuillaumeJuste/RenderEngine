#pragma once

#ifndef RENDERENGINE_TEXTURE
#define RENDERENGINE_TEXTURE

#include "ResourceManager/Assets/IAsset.hpp"
#include <vector>

namespace RenderEngine::Assets
{
    /**
     * @brief Texture class implementation
    */
    struct Texture : public IAsset
    {
        using IAsset::IAsset;

        int width;
        int height;
        int imageSize;

        char* pixels;

        Texture() = default;
        ~Texture() = default;

        bool operator==(const Texture& _rhs) const;
    };
}

#endif