#pragma once

#ifndef RENDERENGINE_RAWTEXTURE
#define RENDERENGINE_RAWTEXTURE

#include <string>

namespace RenderEngine::Assets
{
    /**
     * @brief raw texture data struct
    */
    struct RawTexture
    {
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

        /**
         * @brief image data
         * hdr type = dataF
         * other types = dataC
        */
        union
        {
            char* dataC;
            float* dataF;
        };

        /// default constructor
        RawTexture() = default;

        /// default destructor
        ~RawTexture() = default;
    };
}

#endif