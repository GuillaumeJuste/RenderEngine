#pragma once

#ifndef RENDERENGINE_SHADERSTAGE
#define RENDERENGINE_SHADERSTAGE

#include <chrono>
#include <ctime>
#include "Utils/Event/Event.hpp"

namespace RenderEngine::Assets
{
    /// Supported shader stages
    enum ShaderStage
    {
        VERTEX,
        FRAGMENT,
        COMPUTE,
        GEOMETRY
    };
}

#endif