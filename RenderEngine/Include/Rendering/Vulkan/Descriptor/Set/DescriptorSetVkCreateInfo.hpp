#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETVKCREATEINFO
#define RENDERENGINE_DESCRIPTORSETVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"
#include "Rendering/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"


namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorSetVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;

		DescriptorSetLayout descriptorSetLayout;

		DescriptorPool descriptorPool;

		DescriptorDataList descriptorDatas;

		size_t frameCount;

		DescriptorSetVkCreateInfo() = default;
	};
}

#endif