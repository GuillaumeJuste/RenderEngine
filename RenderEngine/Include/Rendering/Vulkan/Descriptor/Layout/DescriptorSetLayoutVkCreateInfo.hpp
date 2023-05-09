#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETLAYOUTVKCREATEINFO
#define RENDERENGINE_DESCRIPTORSETLAYOUTVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"


namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorSetLayoutVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		DescriptorDataList descriptorDatas;

		DescriptorSetLayoutVkCreateInfo() = default;
	};
}

#endif