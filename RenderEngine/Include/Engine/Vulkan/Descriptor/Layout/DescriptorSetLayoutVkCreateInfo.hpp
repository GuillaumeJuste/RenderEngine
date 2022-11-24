#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETLAYOUTVKCREATEINFO
#define RENDERENGINE_DESCRIPTORSETLAYOUTVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Descriptor/Set/BaseDescriptorSetData.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorSetLayoutVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		std::vector<BaseDescriptorSetData> descriptorSetDatas;

		DescriptorSetLayoutVkCreateInfo() = default;
	};
}

#endif