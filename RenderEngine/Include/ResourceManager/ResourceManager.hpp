#pragma once

#ifndef RENDERENGINE_RESSOURCEMANAGER
#define RENDERENGINE_RESSOURCEMANAGER

#include <string>
#include <forward_list>
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "ResourceManager/Assets/Shader/Shader.hpp"
#include "ResourceManager/Assets/Shader/RawShader.hpp"
#include "ResourceManager/Assets/Texture/CubemapImportInfos.hpp"
#include "ResourceManager/Assets/AssetManager.hpp"
#include "Rendering/Base/Interface/IRenderContext.hpp"
#include "SceneGraph/Scene/Skybox.hpp"

using namespace RenderEngine::Assets;
using namespace RenderEngine::Rendering;

namespace RenderEngine
{
    /**
     * @brief Resource manager class implementation
    */
    class ResourceManager
    {
    private:
        /**
         * @brief reference to render context using assets
        */
        IRenderContext* renderContext;
        
        /// Mesh asset manager
        AssetManager<Mesh> meshManager;

        /// texture asset manager
        AssetManager<Texture> textureManager;

        /// Shader asset manager
        AssetManager<Shader> shaderManager;

        /**
         * @brief Read shader file and fill _output with it's content
         * @param _shaderFilePath filepath to the shader
         * @param _output shader data struct
         * @return true if shader was read
        */
        bool ReadShaderFile(const std::string& _shaderFilePath, RawShader& _output);

    public:
        /// constructor
        ResourceManager(IRenderContext* _renderContext);

        /**
         * @brief Load a mesh
         * @param _filePath file path to the mesh
         * @return Pointer to the resulting mesh
        */
        Mesh* LoadMesh(std::string _filePath);

        /**
         * @brief Get mesh from file path if it was already loaded
         * @param _filePath 
         * @return Matching mesh
        */
        Mesh* GetMesh(std::string _filePath);

        /**
         * @brief Unload a mesh
         * @param _mesh mesh to unload
         * @return true if the mesh was removed
        */
        bool UnloadMesh(Mesh* _mesh);

        /**
         * @brief Load a texture
         * @param _filePath file path to the texture
         * @param _isHDR is the texture .hdr
         * @param _computeMipmap should compute the mipmaps
         * @return Pointer to the resulting texture
        */
        Texture* LoadTexture(std::string _filePath, bool _isHDR = false, bool _computeMipmap = true);

        /**
         * @brief Get texture from file path if it was already loaded
         * @param _filePath 
         * @return Matching texture
        */
        Texture* GetTexture(std::string _filePath);

        /**
         * @brief Unload a texture
         * @param _texture texture to unload
         * @return true if the texture was removed
        */
        bool UnloadTexture(Texture* _texture);

        /**
         * @brief Load a shader
         * @param _filePath file path to the shader
         * @param _shaderStage stage of the shader (vertex, fragment...)
         * @return Pointer to the resulting shader
        */
        Shader* LoadShader(std::string _filePath, SHADER_STAGE _shaderStage);
        
        /**
         * @brief Get shader from file path if it was already loaded
         * @param _filePath 
         * @return Matching shader
        */
        Shader* GetShader(std::string _filePath);
        
        /**
         * @brief Unload a shader
         * @param _shader shader to unload
         * @return true if the shader was removed
        */
        bool UnloadShader(Shader* _shader);

        /**
         * @brief Load a cube map
         * @param _filePaths file path for 6 faces
         * @param _assetName output texture name
         * @param _computeMipmap should compute mipmap
         * @return Pointer to the resulting texture
        */
        Texture* LoadCubemap(const CubemapImportInfos& _filePaths, std::string _assetName, bool _computeMipmap = true);

        /**
         * @brief Create a cube map from a 2D texture
         * @param _texture texture to create the cubemap from
         * @param _generatedTextureSize output cubemap face dimention
         * @param _computeMipmap should compute mimap
         * @return Pointer to the resulting texture
        */
        Texture* CubemapFromTexture(Texture* _texture, Mathlib::Vec2 _generatedTextureSize, bool _computeMipmap = false);

        /**
         * @brief Get cubemap from file path if it was already loaded
         * @param _filePath 
         * @return Matching texture
        */
        Texture* GetCubemap(std::string _filePath);

        /**
         * @brief Unload a cubemap
         * @param _cubemap cubemap to unload
         * @return true if the cubemap was removed
        */
        bool UnloadCubemap(Texture* _cubemap);

        /**
         * @brief Generate cubemap, irradiance map and prefiltered map for a skybox from a 2D texture
         * @param _texture texture to create the cubemaps from
         * @param _generatedTextureSize output cubemap face dimention
         * @param _output skybox to fill
        */
        void CreateSkyboxFromTexture(Texture* _texture, Mathlib::Vec2 _generatedTextureSize, RenderEngine::SceneGraph::Skybox* _output);

        /**
         * @brief export a texture as an asset
         * @param _texture texture to export
        */
        void SaveAsset(Texture* _texture);

        /**
         * @brief import a .asset file and fill texture from saved data
         * @param _filePath file path to the texture
         * @return pointer to the generated texture
        */
        Texture* LoadAsset(std::string _filePath);

        /**
         * @brief clean the resources
        */
        void Clean();
    };
}

#endif