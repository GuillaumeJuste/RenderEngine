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
        int imageSize;

        RenderEngine::Rendering::ITexture* iTexture;

        Texture() = default;
        ~Texture();
    };
}

#endif