#pragma once

#ifndef RENDERENGINE_VERTEX
#define RENDERENGINE_VERTEX

#include "Collections/Space.hpp"
#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include <array>

namespace RenderEngine::Core
{
    struct Vertex 
    {
        Mathlib::Vec2 pos;
        Mathlib::Vec3 color;

        static VkVertexInputBindingDescription GetBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
    };
}

#endif