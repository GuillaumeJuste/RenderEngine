#pragma once

#ifndef RENDERENGINE_RESSOURCEMANAGER
#define RENDERENGINE_RESSOURCEMANAGER

#include <string>
#include <forward_list>
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "ResourceManager/Assets/AssetManager.hpp"

using namespace RenderEngine::Assets;

namespace RenderEngine
{
    class ResourceManager
    {
    private:
        AssetManager<Mesh> meshes;
        AssetManager<Texture> textures;

        ResourceManager() = default;

    public:
        ResourceManager(ResourceManager& other) = delete;
        void operator=(const ResourceManager&) = delete;

        static ResourceManager* GetInstance();

        Mesh* LoadMesh(std::string _filePath);
        Mesh* GetMesh(std::string _filePath);
        bool DeleteMesh(Mesh* _mesh);

        Texture* LoadTexture(std::string _filePath);
        Texture* GetTexture(std::string _filePath);
        bool DeleteTexture(Texture* _texture);
    };
}

#endif