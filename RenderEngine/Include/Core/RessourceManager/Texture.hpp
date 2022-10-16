#pragma once

#ifndef RENDERENGINE_TEXTURE
#define RENDERENGINE_TEXTURE

#include <string>
#include <vector>

namespace RenderEngine::Core
{
    /**
     * @brief Mesh class implementation
    */
    struct Texture
    {
        std::string filePath;

        int width;
        int height;
        int imageSize;

        std::vector<char> pixels;

        Texture() = default;
        ~Texture() = default;

        bool operator==(const Texture& _rhs) const;
    };
}

#endif