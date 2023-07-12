#pragma once

#ifndef RENDERENGINE_STBIWRAPPER
#define RENDERENGINE_STBIWRAPPER

#include "ResourceManager/Assets/Texture/RawTexture.hpp"
#include "ResourceManager/Assets/Texture/CubemapImportInfos.hpp"
#include <string>

using namespace RenderEngine::Assets;

namespace RenderEngine::Wrapper
{
    /**
     * @brief wrapper for the stb library functions
    */
    class StbiWrapper
    {
    private:

        /// default constructor
        StbiWrapper() = default;

    public:
        /**
         * @brief Load a texture from a file path and fill _output with its datas
         * @param _filePath file path to the texture
         * @param _isHDR is the texture hdr
         * @param _output output raw texture to fill
         * @return true if texture was loaded
        */
        static bool LoadTexture(const std::string& _filePath, bool _isHDR, RawTexture& _output);

        /**
         * @brief Load a cubemap from a file path and fill _output with its datas
         * @param _importInfos file pathes to the cubemap faces textures
         * @param _output output raw texture to fill
         * @return true if cubemap was loaded
        */
        static bool LoadCubemap(const CubemapImportInfos& _importInfos, RawTexture& _output);

        /**
         * @brief free memory allocated by stb
         * @param image pointer to the allocated data
        */
        static void FreeImage(char* image);

        /**
         * @brief free memory allocated by stb
         * @param image image pointer to the allocated data
        */
        static void FreeImage(float* image);
    };
}

#endif