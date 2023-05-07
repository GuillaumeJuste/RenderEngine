#pragma once

#ifndef RENDERENGINE_SKYBOX
#define RENDERENGINE_SKYBOX

#include "Core/RessourceManager/Texture.hpp"
#include "Core/RessourceManager/Mesh.hpp"

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