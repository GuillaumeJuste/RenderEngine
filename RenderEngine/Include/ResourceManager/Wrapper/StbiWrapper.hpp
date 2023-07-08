#pragma once

#ifndef RENDERENGINE_STBIWRAPPER
#define RENDERENGINE_STBIWRAPPER

#include "ResourceManager/Assets/Texture/RawTexture.hpp"
#include "ResourceManager/Assets/Texture/CubemapImportInfos.hpp"
#include <string>

using namespace RenderEngine::Assets;

namespace RenderEngine::Wrapper
{
    class StbiWrapper
    {
    private:
        StbiWrapper() = default;

    public:

        static bool LoadTexture(const std::string& _filePath, bool _isHDR, bool _computeMipmap, RawTexture& _output);

        static bool LoadCubemap(const CubemapImportInfos& _importInfos, bool _computeMipmap, RawTexture& _output);

        static void FreeImage(char* image);
        static void FreeImage(float* image);
    };
}

#endif