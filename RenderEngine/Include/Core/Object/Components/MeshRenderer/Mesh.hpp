#pragma once

#ifndef RENDERENGINE_MESH
#define RENDERENGINE_MESH

#include "Core/Object/Object.hpp"
#include "Core/Vertrex/Vertex.hpp"
#include <vector>

namespace RenderEngine::Core
{
    /**
     * @brief Mesh class implementation
    */
    class Mesh
    {
    private:
       


    public:
        /**
        * @brief mesh list of vertices
       */
        std::vector<Vertex> vertices;

        /**
         * @brief mesh list of indices
        */
        std::vector<uint16_t> indices;

        std::string name;

        Mesh() = default;
        ~Mesh() = default;
    };
}

#endif