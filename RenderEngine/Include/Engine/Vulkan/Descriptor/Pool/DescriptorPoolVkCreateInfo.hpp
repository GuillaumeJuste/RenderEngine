#pragma once

#ifndef RENDERENGINE_DESCRIPTORPOOLVKCREATEINFO
#define RENDERENGINE_DESCRIPTORPOOLVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorData.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorPoolVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;

		size_t frameCount;

		std::vector<DescriptorData> descriptorDatas;

		DescriptorPoolVkCreateInfo() = default;
	};
}

#endif