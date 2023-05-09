#pragma once

#ifndef RENDERENGINE_DESCRIPTORPOOLVKCREATEINFO
#define RENDERENGINE_DESCRIPTORPOOLVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"


namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorPoolVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;

		size_t frameCount;

		DescriptorDataList descriptorDatas;

		DescriptorPoolVkCreateInfo() = default;
	};
}

#endif