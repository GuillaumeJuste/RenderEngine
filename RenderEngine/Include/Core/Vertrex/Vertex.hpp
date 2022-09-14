#pragma once

#ifndef RENDERENGINE_VERTEX
#define RENDERENGINE_VERTEX

#include "Collections/Space.hpp"
#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include <array>

namespace RenderEngine::Core
{
    /**
     * @brief Vertex data storing struct
    */
    struct Vertex 
    {
        /**
         * @brief vertex position
        */
        Mathlib::Vec2 position;

        /**
         * @brief vertex color
        */
        Mathlib::Vec3 color;
    };
}

#endif