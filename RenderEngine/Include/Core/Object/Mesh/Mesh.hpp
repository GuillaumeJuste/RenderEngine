#pragma once

#ifndef RENDERENGINE_MESH
#define RENDERENGINE_MESH

#include "Core/Object/Object.hpp"
#include "Core/Vertrex/Vertex.hpp"
#include "Vulkan/BufferObject/BufferObject.hpp"
#include <vector>

namespace RenderEngine::Core
{
    /**
     * @brief class storing Mesh data
    */
    class Mesh : public Object
    {
    private:
        /**
         * @brief mesh list of vertices
        */
        std::vector<Vertex> vertices;

        /**
         * @brief mesh list of indices
        */
        std::vector<uint32_t> indices;

        RenderEngine::Vulkan::BufferObject vertexBufferObject;
        RenderEngine::Vulkan::BufferObject indexBufferObject;

    public:
        Mesh() = default;
        ~Mesh() = default;

        /**
         * @brief Mesh initialize function
         * @param _vertices Mesh list of vertices
         * @param _indices Mesh list of indices
         * @param _output Mesh to fill with vertices and indices
        */
        static void InitializeMesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices, Mesh* _output);

        const RenderEngine::Vulkan::BufferObject* GetVertexBufferObject() const;
        const RenderEngine::Vulkan::BufferObject* GetIndexBufferObject() const;
    };
}

#endif