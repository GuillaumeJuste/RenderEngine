#pragma once

#ifndef RENDERENGINE_STBIWRAPPER
#define RENDERENGINE_STBIWRAPPER

#include "ResourceManager/Assets/Texture/RawTexture.hpp"
using namespace RenderEngine::Assets;

namespace RenderEngine::Wrapper
{
    class StbiWrapper
    {
    private:
        StbiWrapper() = default;

    public:

        static bool LoadTexture(const std::string& _filePath, RawTexture& _output);
    };
}

#endif