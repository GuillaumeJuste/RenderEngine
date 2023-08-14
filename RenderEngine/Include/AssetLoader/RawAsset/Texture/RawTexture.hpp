#pragma once

#ifndef RENDERENGINE_RAWTEXTURE
#define RENDERENGINE_RAWTEXTURE

#include <string>
#include <vector>

namespace Loader
{
    /**
     * @brief raw texture data struct
    */
    struct RawTexture
    {
        // is the texture data stored valid
        bool isValid = false;

        /// image width
        int width = 0;

        /// image height
        int height = 0;

        /// image channels counts
        int channels = 4;

        /// size of the image (width * height * channels)
        int imageSize = 0;

        ///image mip map levels
        uint32_t mipLevels = 1;

        /// number of image hold as data
        unsigned int imageCount = 1;

        /// is the image hdr
        bool isHdr = false;

       ///image data
       std::vector<char> data;

        /// default constructor
        RawTexture() = default;

        /// default destructor
        ~RawTexture() = default;
    };
}

#endif