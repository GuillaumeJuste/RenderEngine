#pragma once

#ifndef RENDERENGINE_RESSOURCEMANAGER
#define RENDERENGINE_RESSOURCEMANAGER

#include <string>
#include <forward_list>
#include "Core/RessourceManager/MeshData.hpp"
#include "Core/Object/Components/MeshRenderer/Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace RenderEngine::Core
{
    class RessourceManager
    {
    private:
        Assimp::Importer importer;

        std::forward_list<MeshData> meshes;

        void ProcessMesh(const aiScene* _scene, MeshData* _output);

    public:

        Mesh* LoadMesh(std::string _name, std::string _filePath);
        Mesh* GetMesh(std::string _name);
        bool DeleteMesh(Mesh* _mesh);

        void Cleanup();
    };
}

#endif