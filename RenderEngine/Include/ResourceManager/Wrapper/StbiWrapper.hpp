#pragma once

#ifndef RENDERENGINE_STBIWRAPPER
#define RENDERENGINE_STBIWRAPPER

#include "ResourceManager/Assets/Texture/RawTexture.hpp"
#include "ResourceManager/Assets/Cubemap/RawCubemap.hpp"
#include "ResourceManager/Assets/Cubemap/CubemapImportInfos.hpp"
#include <string>

using namespace RenderEngine::Assets;

namespace RenderEngine::Wrapper
{
    class StbiWrapper
    {
    private:
        StbiWrapper() = default;

    public:

        static bool LoadTexture(const std::string& _filePath, RawTexture& _output);

        static bool LoadCubemap(const CubemapImportInfos& _importInfos, RawCubemap& _output);

        static void FreeImage(char* image);
    };
}

#endif