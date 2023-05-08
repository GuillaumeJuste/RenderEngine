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

        RessourceManager() = default;

        std::string CheckFilePath(const std::string _filePath);

        void ProcessMesh(const aiScene* _scene, Mesh* _output);

    public:
        RessourceManager(RessourceManager& other) = delete;
        void operator=(const RessourceManager&) = delete;

        static RessourceManager* GetInstance();

        Mesh* LoadMesh(std::string _filePath);
        Mesh* LoadMesh(std::vector<Vertex> _vertices, std::vector<uint16_t> _indices);
        Mesh* GetMesh(std::string _filepath);
        bool DeleteMesh(Mesh* _mesh);

        Texture* LoadTexture(std::string _filePath);
        Texture* GetTexture(std::string _filePath);
        bool DeleteTexture(Texture* _texture);
    };
}

#endif