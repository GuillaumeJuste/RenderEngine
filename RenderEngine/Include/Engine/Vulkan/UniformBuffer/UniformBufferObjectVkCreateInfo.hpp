#pragma once

#ifndef RENDERENGINE_UNIFORMBUFFEROBJECTVKCREATEINFO
#define RENDERENGINE_UNIFORMBUFFEROBJECTVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/UniformBuffer/UniformBufferData.hpp"


namespace RenderEngine::Engine::Vulkan
{
	struct UniformBufferObjectVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		UniformBufferData uniformBufferData;

		UniformBufferObjectVkCreateInfo() = default;
	};
}

#endif