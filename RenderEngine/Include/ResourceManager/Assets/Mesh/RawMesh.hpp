#pragma once

#ifndef RENDERENGINE_RAWMESH
#define RENDERENGINE_RAWMESH

#include "ResourceManager/Assets/Mesh/Vertex.hpp"
#include <vector>

namespace RenderEngine::Assets
{
    /**
     * @brief Mesh class implementation
    */
    struct RawMesh
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

        RawMesh() = default;
        ~RawMesh() = default;
    };
}

#endif