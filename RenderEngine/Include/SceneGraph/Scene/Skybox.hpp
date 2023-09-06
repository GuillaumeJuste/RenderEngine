#pragma once

#ifndef RENDERENGINE_SKYBOX
#define RENDERENGINE_SKYBOX

#include "Rendering/Base/ResourceManager/Assets/Texture/Texture.hpp"
#include "Rendering/Base/ResourceManager/Assets/Mesh/Mesh.hpp"
#include "Rendering/Base/ResourceManager/Assets/Shader/Shader.hpp"
#include "Rendering/Base/ResourceManager/Assets/Shader/ShaderDescriptorSet.hpp"

using namespace RenderEngine::Assets;

namespace RenderEngine::SceneGraph
{
    /**
     * @brief Scene skybox struct
    */
    struct Skybox
    {
        /**
         * @brief mesh on which the skybox will be applied
        */
        Mesh* mesh;

        /**
         * @brief skybox's cubemap
        */
        Texture* cubemap;

        /**
         * @brief skybox's irradiance cubemap
        */
        Texture* irradianceMap;

        /**
         * @brief skybox's prefiltered cubemap
        */
        Texture* prefilterMap;

        /**
         * @brief BRDF look up table
        */
        Texture* BRDFlut;

        /**
         * @brief skybox's vertex shader
        */
        Shader* vertexShader;

        /// Vertex Shader Descriptor set
        ShaderDescriptorSet vertexShaderDescriptorSet;

        /**
         * @brief skybox's fragment shader
        */
        Shader* fragmentShader;

        /// Fragment Shader Descriptor set
        ShaderDescriptorSet fragmentShaderDescriptorSet;
    };
}
#endif