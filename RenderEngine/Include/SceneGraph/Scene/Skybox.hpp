#pragma once

#ifndef RENDERENGINE_SKYBOX
#define RENDERENGINE_SKYBOX

#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Shader/Shader.hpp"

using namespace RenderEngine::Assets;

namespace RenderEngine::SceneGraph
{
    /**
     * @brief Scene skybox struct
    */
    struct Skybox
    {
        Mesh* mesh;
        Texture* cubemap;
        Texture* irradianceMap;
        Texture* prefilterMap;
        Texture* BRDFlut;

        Shader* vertexShader;
        Shader* fragmentShader;
    };
}
#endif