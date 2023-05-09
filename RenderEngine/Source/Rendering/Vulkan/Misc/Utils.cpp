#include "Rendering/Vulkan/Misc/Utils.hpp"
#include <iostream>

namespace RenderEngine::Rendering
{
    uint32_t FindMemoryType(VkPhysicalDevice _physicalDevice, uint32_t _typeFilter, VkMemoryPropertyFlags _properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((_typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & _properties) == _properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }
}