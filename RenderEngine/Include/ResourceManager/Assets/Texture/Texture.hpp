#pragma once

#ifndef RENDERENGINE_TEXTURE
#define RENDERENGINE_TEXTURE

#include <string>
#include <vector>

namespace RenderEngine::Assets
{
    /**
     * @brief Texture class implementation
    */
    struct Texture
    {
        std::string filePath;

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