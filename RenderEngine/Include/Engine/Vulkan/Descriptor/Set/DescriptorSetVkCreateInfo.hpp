#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETVKCREATEINFO
#define RENDERENGINE_DESCRIPTORSETVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"
#include "Engine/Vulkan/Descriptor/Pool/DescriptorPool.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorSetVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;

		DescriptorSetLayout descriptorSetLayout;

		DescriptorPool* descriptorPool;

		size_t frameCount;

		DescriptorSetVkCreateInfo() = default;
	};
}

#endif