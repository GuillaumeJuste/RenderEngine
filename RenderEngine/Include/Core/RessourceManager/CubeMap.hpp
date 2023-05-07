#pragma once

#ifndef RENDERENGINE_CUBEMAP
#define RENDERENGINE_CUBEMAP

#include <string>
#include <vector>
#include "Core/RessourceManager/Texture.hpp"

namespace RenderEngine::Core
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