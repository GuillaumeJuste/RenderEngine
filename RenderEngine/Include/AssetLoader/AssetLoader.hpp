#pragma once

#ifndef RENDERENGINE_ASSETLOADER
#define RENDERENGINE_ASSETLOADER

#include <string>
#include <vector>
#include "AssetLoader/RawAsset/Shader/RawShader.hpp"
#include "AssetLoader/RawAsset/Texture/RawTexture.hpp"
#include "AssetLoader/RawAsset/Texture/CubemapImportInfos.hpp"
#include "AssetLoader/RawAsset/Mesh/RawMesh.hpp"

namespace Loader
{
    /**
     * @brief AssetLoader class implementation
    */
    class AssetLoader
    {
    private:
        /**
         * @brief Read shader file and fill _output with it's content
         * @param _shaderFilePath filepath to the shader
         * @param _output shader data struct
         * @return true if shader was read
        */
        static bool ReadShaderFile(const std::string& _shaderFilePath, RawShader& _output);

    public:
        /// constructor
        AssetLoader() = default;

        /**
         * @brief Load a mesh
         * @param _filePath file path to the mesh
         * @return struct holding loaded mesh data
        */
        static RawMesh LoadMesh(std::string _filePath);

        /**
         * @brief Load a texture
         * @param _filePath file path to the texture
         * @param _format texture format
         * @return struct holding loaded texture data
        */
        static RawTexture LoadTexture(std::string _filePath, TextureFormat _format);

        /**
        * @brief Load a cube map
        * @param _filePaths file path for 6 faces
        * @param _format texture format
        * @return struct holding loaded cubemap data
       */
        static RawTexture LoadCubemap(const CubemapImportInfos& _filePaths, TextureFormat _format);

        /**
         * @brief Load a shader
         * @param _filePath file path to the shader
         * @param _shaderStage stage of the shader (vertex, fragment...)
         * @return struct holding loaded shader data
        */
        static RawShader LoadShader(std::string _filePath, ShaderStage _shaderStage);
    };
}

#endif