#pragma once

#ifndef RENDERENGINE_STBIWRAPPER
#define RENDERENGINE_STBIWRAPPER

#include "AssetLoader/RawAsset/Texture/RawTexture.hpp"
#include "AssetLoader/RawAsset/Texture/CubemapImportInfos.hpp"
#include <string>

using namespace Loader;

namespace Wrapper
{
    /**
     * @brief wrapper for the stb library functions
    */
    class StbiWrapper
    {
    private:

        /// default constructor
        StbiWrapper() = default;

        static int GetSTBIFormat(TextureFormat _format);

    public:
        /**
         * @brief Load a texture from a file path and fill _output with its datas
         * @param _filePath file path to the texture
         * @param _format texture format
         * @param _output output raw texture to fill
         * @return true if texture was loaded
        */
        static bool LoadTexture(const std::string& _filePath, TextureFormat _format, RawTexture& _output);

        /**
         * @brief Load a cubemap from a file path and fill _output with its datas
         * @param _importInfos file pathes to the cubemap faces textures
         * @param _format texture format
         * @param _output output raw texture to fill
         * @return true if cubemap was loaded
        */
        static bool LoadCubemap(const CubemapImportInfos& _importInfos, TextureFormat _format, RawTexture& _output);

        /**
         * @brief free memory allocated by stb
         * @param image pointer to the allocated data
        */
        static void FreeImage(char* image);
    };
}

#endif