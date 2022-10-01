#pragma once

#ifndef RENDERENGINE_DESCRIPTORPOOLVKCREATEINFO
#define RENDERENGINE_DESCRIPTORPOOLVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorPoolVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;

		DescriptorPoolVkCreateInfo() = default;
	};
}

#endif