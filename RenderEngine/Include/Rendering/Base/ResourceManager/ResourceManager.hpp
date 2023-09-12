#pragma once

#ifndef TMP_RENDERENGINE_RESSOURCEMANAGER
#define TMP_RENDERENGINE_RESSOURCEMANAGER

#include <string>
#include <forward_list>
#include "Rendering/Base/ResourceManager/Assets/Mesh/Mesh.hpp"
#include "Rendering/Base/ResourceManager/Assets/Texture/Texture.hpp"
#include "Rendering/Base/ResourceManager/Assets/Shader/Shader.hpp"
#include "Rendering/Base/ResourceManager/Assets/AssetManager.hpp"
#include "SceneGraph/Scene/Skybox.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec2.hpp"

#include "AssetLoader/AssetLoader.hpp"

using namespace RenderEngine::Assets;
using namespace Loader;

namespace RenderEngine::Rendering
{
    /**
     * @brief Resource manager class implementation
    */
    class ResourceManager
    {
    private:
        /// Mesh asset manager
        AssetManager<Mesh> meshManager;

        /// texture asset manager
        AssetManager<Texture> textureManager;

        /// Shader asset manager
        AssetManager<Shader> shaderManager;

        virtual IBuffer* CreateVertexBufferObject(const Loader::RawMesh& _input) = 0;
        virtual IBuffer* CreateIndexBufferObject(const Loader::RawMesh& _input) = 0;

        virtual ITexture* CreateTexture(const Loader::RawTexture& _input, bool _generateMipMap = true) = 0;
        virtual IShader* CreateShader(const Loader::RawShader& _input) = 0;
        virtual bool CreateCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, bool _generateMipmap, RenderEngine::Assets::Texture* _output,
            RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader) = 0;
        virtual bool CreatePrefilteredCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, RenderEngine::Assets::Texture* _output,
            RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader) = 0;

        virtual std::vector<char> GetTextureContent(RenderEngine::Assets::Texture* _texture, uint32_t _imageTotalSize) = 0;

    public:
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
         * @param _format texture format
         * @param _computeMipmap should compute the mipmaps
         * @return Pointer to the resulting texture
        */
        Texture* LoadTexture(std::string _filePath, TextureFormat _format, bool _computeMipmap = true);

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
        Shader* LoadShader(std::string _filePath, ShaderStage _shaderStage);

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
         * @param _format texture format
         * @param _computeMipmap should compute mipmap
         * @return Pointer to the resulting texture
        */
        Texture* LoadCubemap(const CubemapImportInfos& _filePaths, std::string _assetName, TextureFormat _format, bool _computeMipmap = true);

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