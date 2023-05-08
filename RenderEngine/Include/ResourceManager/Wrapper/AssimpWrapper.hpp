#pragma once

#ifndef RENDERENGINE_ASSIMPWRAPPER
#define RENDERENGINE_ASSIMPWRAPPER

#include "ResourceManager/Assets/Mesh/Mesh.hpp"
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

        static bool ProcessMesh(const aiScene* _scene, Mesh& _output);

    public:
        static bool LoadMesh(const std::string& _filePath, Mesh& _output);
    };
}

#endif