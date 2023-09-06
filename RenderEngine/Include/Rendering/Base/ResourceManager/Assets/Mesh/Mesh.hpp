#pragma once

#ifndef RENDERENGINE_MESH
#define RENDERENGINE_MESH

#include "Rendering/Base/ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/IBuffer.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief Mesh struct implementation
    */
    struct Mesh : public IAsset
    {
        /// index buffer
        RenderEngine::Rendering::IBuffer* indexBuffer;

        /// vertex buffer
        RenderEngine::Rendering::IBuffer* vertexBuffer;

        /// number of indices
        size_t indiceCount;

        /// default constructor
        Mesh() = default;

        /// destructor
        ~Mesh();

        /// clear buffers
        void Clean();
    };
}

#endif