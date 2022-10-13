#pragma once

#ifndef RENDERENGINE_MESHDATA
#define RENDERENGINE_MESHDATA

#include "Core/Object/Components/MeshRenderer/Mesh.hpp"

namespace RenderEngine::Core
{
    struct MeshData
    {
        std::string filePath;
        Mesh mesh;

        bool operator==(const MeshData& _rhs) const;
    };
}

#endif