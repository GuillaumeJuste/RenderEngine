#pragma once

#ifndef RENDERENGINE_MESH
#define RENDERENGINE_MESH

#include "ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/IBuffer.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Mesh class implementation
    */
    struct Mesh : public IAsset
    {
        RenderEngine::Rendering::IBuffer* indexBuffer;
        RenderEngine::Rendering::IBuffer* vertexBuffer;

        size_t indiceCount;

        Mesh() = default;
        ~Mesh();

        void Clean();
    };
}

#endif