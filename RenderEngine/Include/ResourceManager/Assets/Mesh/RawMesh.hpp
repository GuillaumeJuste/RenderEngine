#pragma once

#ifndef RENDERENGINE_RAWMESH
#define RENDERENGINE_RAWMESH

#include "ResourceManager/Assets/Mesh/Vertex.hpp"
#include <vector>

namespace RenderEngine::Assets
{
    /**
     * @brief raw mesh struct implementation
    */
    struct RawMesh
    {
        
        /// list of mesh vertices
        std::vector<Vertex> vertices;

        
        /// list of mesh indices
        std::vector<uint16_t> indices;

        /// mesh name
        std::string name;

        /// default constructor
        RawMesh() = default;

        /// default destructor
        ~RawMesh() = default;
    };
}

#endif