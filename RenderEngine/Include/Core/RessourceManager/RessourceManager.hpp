#pragma once

#ifndef RENDERENGINE_RESSOURCEMANAGER
#define RENDERENGINE_RESSOURCEMANAGER

#include <string>
#include <forward_list>
#include "Core/RessourceManager/Mesh.hpp"
#include "Core/RessourceManager/Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace RenderEngine::Core
{
    class RessourceManager
    {
    private:
        Assimp::Importer importer;

        std::forward_list<Mesh> meshes;
        std::forward_list<Texture> textures;

        void ProcessMesh(const aiScene* _scene, Mesh* _output);

    public:

        Mesh* LoadMesh(std::string _filePath, std::string _name);
        Mesh* GetMesh(std::string _name);
        bool DeleteMesh(Mesh* _mesh);

        Texture* LoadTexture(std::string _filePath);
        Texture* GetTexture(std::string _filePath);
        bool DeleteTexture(Texture* _texture);

        void Cleanup();
    };
}

#endif