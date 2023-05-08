#pragma once

#ifndef RENDERENGINE_SKYBOX
#define RENDERENGINE_SKYBOX

#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"

using namespace RenderEngine::Assets;

namespace RenderEngine::Core
{
    /**
     * @brief Scene skybox struct
    */
    struct Skybox
    {
        Mesh* mesh;
        Texture* front;
        Texture* back;
        Texture* top;
        Texture* bottom;
        Texture* right;
        Texture* left;
    };
}
#endif