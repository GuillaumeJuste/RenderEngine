#pragma once

#ifndef RENDERENGINE_TEXTURE
#define RENDERENGINE_TEXTURE

#include "ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/ITexture.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Texture struct implementation
    */
    struct Texture : public IAsset
    {

        /// image width
        int width;
        
        /// image height
        int height;
        
        /// image channels counts
        int channels = 4;
        
        /// size of the image (width * height * channels)
        uint32_t imageSize;
        
        ///image mip map levels
        uint32_t mipLevels;
        
        /// number of image hold as data
        uint32_t imageCount = 1;
        
        /// is the image hdr
        bool isHDR = false;

        /// handle to engine texture
        RenderEngine::Rendering::ITexture* iTexture;

        /// default constructor
        Texture() = default;

        /// destructor
        ~Texture();

        /**
         * @brief Compute the total size of the texture
         * including all faces of a cubemap and their mipmaps
         * @param _texture texture to compute the size from
         * @return texture total size
        */
        static uint32_t ComputeTotalSize(Texture* _texture);
    };
}

#endif