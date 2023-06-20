#pragma once

#ifndef RENDERENGINE_RESSOURCEMANAGER
#define RENDERENGINE_RESSOURCEMANAGER

#include <string>
#include <forward_list>
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "ResourceManager/Assets/Shader/Shader.hpp"
#include "ResourceManager/Assets/Shader/RawShader.hpp"
#include "ResourceManager/Assets/Cubemap/CubemapImportInfos.hpp"
#include "ResourceManager/Assets/Cubemap/Cubemap.hpp"
#include "ResourceManager/Assets/AssetManager.hpp"
#include "Rendering/Base/Interface/IRenderContext.hpp"

using namespace RenderEngine::Assets;
using namespace RenderEngine::Rendering;

namespace RenderEngine
{
    class ResourceManager
    {
    private:
        IRenderContext* renderContext;
        
        AssetManager<Mesh> meshManager;
        AssetManager<Texture> textureManager;
        AssetManager<Shader> shaderManager;
        AssetManager<Cubemap> cubemapManager;

        bool ReadShaderFile(const std::string& _shaderFilePath, RawShader& _output);

    public:
        ResourceManager(IRenderContext* _renderContext);

        Mesh* LoadMesh(std::string _filePath);
        Mesh* GetMesh(std::string _filePath);
        bool UnloadMesh(Mesh* _mesh);

        Texture* LoadTexture(std::string _filePath, bool _isHDR = false, bool _computeMipmap = true);
        Texture* GetTexture(std::string _filePath);
        bool UnloadTexture(Texture* _texture);

        Shader* LoadShader(std::string _filePath, SHADER_STAGE _shaderStage);
        Shader* GetShader(std::string _filePath);
        bool UnloadShader(Shader* _texture);

        Cubemap* LoadCubemap(CubemapImportInfos _filePaths, bool _computeMipmap = true);
        Cubemap* LoadCubemap(std::string _filePath, bool _computeMipmap = true);
        Cubemap* GetCubemap(const CubemapImportInfos& _filePaths);
        bool UnloadCubemap(Cubemap* _cubemap);

        void Clean();
    };
}

#endif