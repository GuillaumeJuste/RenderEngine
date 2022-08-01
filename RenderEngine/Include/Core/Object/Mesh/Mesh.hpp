#pragma once

#ifndef RENDERENGINE_MESH
#define RENDERENGINE_MESH

#include "Core/Object/Object.hpp"
#include "Core/Vertrex/Vertex.hpp"
#include "Engine/BufferObject/BufferObject.hpp"
#include <vector>

namespace RenderEngine::Core
{
    class Mesh : public Object
    {
    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        RenderEngine::Vulkan::BufferObject vertexBufferObject;
        RenderEngine::Vulkan::BufferObject indexBufferObject;

    public:
        Mesh() = default;
        ~Mesh() = default;

        static void InitializeMesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices, Mesh* _output);

        const RenderEngine::Vulkan::BufferObject* GetVertexBufferObject() const;
        const RenderEngine::Vulkan::BufferObject* GetIndexBufferObject() const;
    };
}

#endif