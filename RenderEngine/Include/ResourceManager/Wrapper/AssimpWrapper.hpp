#pragma once

#ifndef RENDERENGINE_ASSIMPWRAPPER
#define RENDERENGINE_ASSIMPWRAPPER

#include "ResourceManager/Assets/Mesh/RawMesh.hpp"
using namespace RenderEngine::Assets;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace RenderEngine::Wrapper
{
    class AssimpWrapper
    {
    private:
        static Assimp::Importer importer;

        AssimpWrapper() = default;

        static bool ProcessMesh(const aiScene* _scene, RawMesh& _output);

    public:
        static bool LoadMesh(const std::string& _filePath, RawMesh& _output);
    };
}

#endif