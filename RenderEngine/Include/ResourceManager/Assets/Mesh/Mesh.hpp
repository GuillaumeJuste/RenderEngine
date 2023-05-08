#pragma once

#ifndef RENDERENGINE_MESH
#define RENDERENGINE_MESH

#include "ResourceManager/Assets/Mesh/Vertex.hpp"
#include <vector>

namespace RenderEngine::Assets
{
    /**
     * @brief Mesh class implementation
    */
    struct Mesh
    {
        /**
        * @brief mesh list of vertices
       */
        std::vector<Vertex> vertices;

        /**
         * @brief mesh list of indices
        */
        std::vector<uint16_t> indices;

        std::string name;

        std::string filePath;

        Mesh() = default;
        ~Mesh() = default;

        bool operator==(const Mesh& _rhs) const;
    };
}

#endif