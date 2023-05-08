#pragma once

#ifndef RENDERENGINE_CUBEMAP
#define RENDERENGINE_CUBEMAP

#include <string>
#include <vector>
#include "ResourceManager/Assets/Texture/Texture.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief CubeMap struct implementation
    */
    struct CubeMap : public Texture
    {
        CubeMap() = default;
        ~CubeMap() = default;
    };
}

#endif