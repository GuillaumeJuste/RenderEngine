#pragma once

#ifndef RENDERENGINE_VERTEX
#define RENDERENGINE_VERTEX

#include "Collections/Space.hpp"
#include "Vulkan/Misc/VulkanBaseInclude.hpp"
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

        /**
         * @brief Get Vertex struct binding description
         * @return Vertex struct binding description
        */
        static VkVertexInputBindingDescription GetBindingDescription();

        /**
         * @brief Get Vertex struct attribute description
         * @return Vertex struct attribute description
        */
        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
    };
}

#endif