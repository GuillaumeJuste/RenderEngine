#pragma once

#ifndef RENDERENGINE_CUBEMAP
#define RENDERENGINE_CUBEMAP

#include "ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/ITexture.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Cubemap class implementation
    */
    struct Cubemap : public IAsset
    {
        int width;
        int height;
        int imageSize;

        RenderEngine::Rendering::ITexture* iTexture;

        Cubemap() = default;
        ~Cubemap();
    };
}

#endif